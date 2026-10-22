#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool record_exists(const char *filename, int lines_per_record, const char *key)
{
    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL)
        return false;

    char line[100];
    int line_no = 0;
    bool found = false;

    while (fgets(line, sizeof(line), fptr) != NULL)
    {
        if (line_no % lines_per_record == 0)
        {
            line[strcspn(line, "\n")] = '\0';
            if (strcmp(line, key) == 0)
            {
                found = true;
                break;
            }
        }
        line_no++;
    }
    fclose(fptr);
    return found;
}

void get_user_details()
{
    char username[50], first_name[50], last_name[50], password[20], confirm_password[20];
    int age;
    bool check_pw_flag = false;

    printf("Enter your first name: ");
    scanf("%s", first_name);
    printf("Enter your last name: ");
    scanf("%s", last_name);

    printf("Enter your age: ");
    scanf(" %i", &age);

    if (age < 18)
    {
        printf("Ineligible for account creation\n");
    }
    else if (age > 130)
    {
        printf("Error!");
    }
    else
    {
        while (true)
        {
            printf("Choose a username: ");
            scanf("%s", username);

            if (record_exists("User_Details.txt", 5, username))
                printf("Username already taken, try another\n");
            else
                break;
        }

        while (check_pw_flag == false)
        {
            printf("Create a password: ");
            scanf("%s", password);

            printf("Confirm your password: ");
            scanf("%s", confirm_password);

            if (strcmp(password, confirm_password) == 0)
            {
                printf("Passwords match\n");
                check_pw_flag = true;

                FILE *fptr = NULL;
                int attempts = 0;
                while (fptr == NULL && attempts < 3)
                {
                    fptr = fopen("User_Details.txt", "a");
                    attempts++;
                }
                if (fptr != NULL)
                {
                    printf("The file is opened\n");
                    // put encryption here
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
        scanf("%s", employee_id);

        if (record_exists("Admin_Details.txt", 2, employee_id))
            printf("Employee ID already registered\n");
        else
            break;
    }

    while (check_pw_flag == false)
    {
        printf("Create a password: ");
        scanf("%s", password);

        printf("Confirm your password: ");
        scanf("%s", confirm_password);

        if (strcmp(password, confirm_password) == 0)
        {
            printf("Passwords match\n");
            check_pw_flag = true;

            FILE *fptr = NULL;
            int attempts = 0;
            while (fptr == NULL && attempts < 3)
            {
                fptr = fopen("Admin_Details.txt", "a");
                attempts++;
            }
            if (fptr != NULL)
            {
                printf("The file is opened\n");
                // put encryption here
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
        scanf(" %i", &acc_choice);

        switch (acc_choice)
        {
        case 1:
            acc_flag = true;
            get_user_details();
            break;

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
    printf("Enter your username: ");
    scanf("%s", username);
    printf("Enter your password: ");
    scanf("%s", password);

    FILE *fptr = fopen("User_Details.txt", "r");
    if (fptr == NULL)
    {
        printf("No accounts found\n");
        return;
    }

    char un[100], fn[100], ln[100], age[100], pw[100];
    bool found = false;

    while (fgets(un, sizeof(un), fptr) && fgets(fn, sizeof(fn), fptr) &&
           fgets(ln, sizeof(ln), fptr) && fgets(age, sizeof(age), fptr) &&
           fgets(pw, sizeof(pw), fptr))
    {
        un[strcspn(un, "\n")] = '\0';
        pw[strcspn(pw, "\n")] = '\0';

        if (strcmp(username, un) == 0 && strcmp(password, pw) == 0)
        {
            found = true;
            break;
        }
    }
    fclose(fptr);

    if (found)
        printf("Login Successful\n");
    else
        printf("Login failed\n");
}

void admin_login()
{
    char employee_id[50], password[20];
    printf("Enter your employee ID: ");
    scanf("%s", employee_id);
    printf("Enter your password: ");
    scanf("%s", password);

    FILE *fptr = fopen("Admin_Details.txt", "r");
    if (fptr == NULL)
    {
        printf("No admin accounts found\n");
        return;
    }

    char id[100], pw[100];
    bool found = false;

    while (fgets(id, sizeof(id), fptr) && fgets(pw, sizeof(pw), fptr))
    {
        id[strcspn(id, "\n")] = '\0';
        pw[strcspn(pw, "\n")] = '\0';

        if (strcmp(employee_id, id) == 0 && strcmp(password, pw) == 0)
        {
            found = true;
            break;
        }
    }
    fclose(fptr);

    if (found)
        printf("Login Successful\n");
    else
        printf("Login failed\n");
}

int main(void)
{
    bool flag = false;
    int choice;

    while (flag == false)
    {
        printf("1.Create User Account\n2.Create Admin Account\n3.User Login\n4.Admin Login\n5.Exit\n");
        scanf(" %i", &choice);

        switch (choice)
        {
        case 1:
            flag = true;
            create_user_account();
            break;
        case 2:
            flag = true;
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
        }
    }
    return 0;
}
