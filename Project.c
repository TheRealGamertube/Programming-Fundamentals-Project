#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LOGIN_ATTEMPTS 3

static void clear_input(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

static bool read_int(int *out)
{
    if (scanf(" %d", out) != 1)
    {
        clear_input();
        return false;
    }
    return true;
}

bool record_exists(const char *filename, int lines_per_record, const char *key)
{
    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL)
        return false;

    char line[100];
    int line_number = 0;
    bool found = false;

    while (fgets(line, sizeof(line), fptr) != NULL)
    {
        if (line_number % lines_per_record == 0)
        {
            line[strcspn(line, "\n")] = '\0';
            if (strcmp(line, key) == 0)
            {
                found = true;
                break;
            }
        }
        line_number++;
    }
    fclose(fptr);
    return found;
}

static bool check_credentials(const char *filename, int lines_per_record,
                              int password_index, const char *id, const char *password)
{
    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL)
        return false;

    char line[100];
    int line_number = 0;
    bool id_matches = false;
    bool ok = false;

    while (fgets(line, sizeof(line), fptr) != NULL)
    {
        line[strcspn(line, "\n")] = '\0';
        int pos = line_number % lines_per_record;

        if (pos == 0)
            id_matches = (strcmp(line, id) == 0);
        else if (pos == password_index && id_matches && strcmp(line, password) == 0)
        {
            ok = true;
            break;
        }
        line_number++;
    }
    fclose(fptr);
    return ok;
}

static FILE *open_for_append(const char *filename)
{
    FILE *fptr = NULL;
    int open_attempts = 0;
    while (fptr == NULL && open_attempts < 3)
    {
        fptr = fopen(filename, "a");
        open_attempts++;
    }
    return fptr;
}

void get_user_details()
{
    char username[50], first_name[50], last_name[50], password[20], confirm_password[20];
    int age;
    bool check_pw_flag = false;

    printf("Enter your first name: ");
    scanf("%49s", first_name);
    printf("Enter your last name: ");
    scanf("%49s", last_name);

    printf("Enter your age: ");
    if (!read_int(&age))
    {
        printf("Error! Invalid age\n");
        return;
    }

    if (age < 18)
    {
        printf("Ineligible for account creation\n");
    }
    else if (age > 130)
    {
        printf("Error!\n");
    }
    else
    {
        while (true)
        {
            printf("Choose a username: ");
            scanf("%49s", username);

            if (record_exists("User_Details.txt", 5, username))
                printf("Username already taken, try another\n");
            else
                break;
        }

        while (check_pw_flag == false)
        {
            printf("Create a password: ");
            scanf("%19s", password);

            printf("Confirm your password: ");
            scanf("%19s", confirm_password);

            if (strcmp(password, confirm_password) == 0)
            {
                printf("Passwords match\n");
                check_pw_flag = true;

                FILE *fptr = open_for_append("User_Details.txt");
                if (fptr != NULL)
                {
                    printf("The file is opened\n");
                    fprintf(fptr, "%s\n%s\n%s\n%i\n%s\n", username, first_name, last_name, age, password);
                    fclose(fptr);
                }
                else
                {
                    printf("Could not open file\n");
                }
            }
            else
            {
                printf("Passwords don't match\n");
            }
        }
    }
}

void get_admin_details()
{
    char employee_id[50], password[20], confirm_password[20];
    bool check_pw_flag = false;

    while (true)
    {
        printf("Enter your employee ID: ");
        scanf("%49s", employee_id);

        if (record_exists("Admin_Details.txt", 2, employee_id))
            printf("Employee ID already registered\n");
        else
            break;
    }

    while (check_pw_flag == false)
    {
        printf("Create a password: ");
        scanf("%19s", password);

        printf("Confirm your password: ");
        scanf("%19s", confirm_password);

        if (strcmp(password, confirm_password) == 0)
        {
            printf("Passwords match\n");
            check_pw_flag = true;

            FILE *fptr = open_for_append("Admin_Details.txt");
            if (fptr != NULL)
            {
                printf("The file is opened\n");
                fprintf(fptr, "%s\n%s\n", employee_id, password);
                fclose(fptr);
            }
            else
            {
                printf("Could not open file\n");
            }
        }
        else
        {
            printf("Passwords don't match\n");
        }
    }
}

void create_user_account()
{
    int acc_choice;
    bool acc_flag = false;

    while (acc_flag == false)
    {
        printf("Enter your preferred account type:\n1.Current\n2.Savings\n");
        if (!read_int(&acc_choice))
        {
            printf("Error! Invalid Input\n");
            continue;
        }

        switch (acc_choice)
        {
        case 1:
        case 2:
            acc_flag = true;
            get_user_details();
            break;

        default:
            printf("Error! Invalid Input\n");
        }
    }
}

void user_login()
{
    char username[50], password[20];

    FILE *fptr = fopen("User_Details.txt", "r");
    if (fptr == NULL)
    {
        printf("No accounts found\n");
        return;
    }
    fclose(fptr);

    printf("Enter your username: ");
    scanf("%49s", username);

    for (int attempt = 1; attempt <= MAX_LOGIN_ATTEMPTS; attempt++)
    {
        printf("Enter your password: ");
        scanf("%19s", password);

        if (check_credentials("User_Details.txt", 5, 4, username, password))
        {
            printf("Login Successful\n");
            return;
        }

        if (attempt < MAX_LOGIN_ATTEMPTS)
            printf("Login failed. (%i Attempts left) Please try again.\n",
                   MAX_LOGIN_ATTEMPTS - attempt);
    }
    printf("Login failed\n");
}

void admin_login()
{
    char employee_id[50], password[20];

    FILE *fptr = fopen("Admin_Details.txt", "r");
    if (fptr == NULL)
    {
        printf("No admin accounts found\n");
        return;
    }
    fclose(fptr);

    printf("Enter your employee ID: ");
    scanf("%49s", employee_id);

    for (int attempt = 1; attempt <= MAX_LOGIN_ATTEMPTS; attempt++)
    {
        printf("Enter your password: ");
        scanf("%19s", password);

        if (check_credentials("Admin_Details.txt", 2, 1, employee_id, password))
        {
            printf("Login Successful\n");
            return;
        }

        if (attempt < MAX_LOGIN_ATTEMPTS)
            printf("Login failed. (%i Attempts left) Please try again.\n",
                   MAX_LOGIN_ATTEMPTS - attempt);
    }
    printf("Login failed\n");
}

int main(void)
{
    bool flag = false;
    int choice;

    while (flag == false)
    {
        printf("1.Create User Account\n2.Create Admin Account\n3.User Login\n4.Admin Login\n5.Exit\n");
        if (!read_int(&choice))
        {
            printf("Error! Invalid Input\n");
            continue;
        }

        switch (choice)
        {
        case 1:
            create_user_account();
            break;
        case 2:
            get_admin_details();
            break;
        case 3:
            flag = true;
            user_login();
            break;
        case 4:
            flag = true;
            admin_login();
            break;
        case 5:
            printf("Exited\n");
            flag = true;
            break;
        default:
            printf("Error! Invalid Input\n");
        }
    }
    return 0;
}
