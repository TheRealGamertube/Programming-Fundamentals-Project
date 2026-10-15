#include <stdio.h>
#include <stdbool.h>
#include <string.h>

void get_user_details()
{
    char first_name[50], last_name[50], password[20], confirm_password[20];
    int age;
    bool check_pw_flag;

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
                FILE *fptr;
                fptr = fopen("User_Details.txt", "a");
                if (fptr != NULL)
                {
                    printf("The file is opened");
                    // put encryption here
                    fprintf(fptr, "%s\n%s\n%i\n%s\n", first_name, last_name, age, password);
                    fclose(fptr);
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
    bool check_pw_flag;
    printf("Enter your employee ID: ");
    scanf("%s", employee_id);

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
            FILE *fptr;
            fptr = fopen("Admin_Details.txt", "a");
            if (fptr != NULL)
            {
                printf("The file is opened");
                // put encryption here
                fprintf(fptr, "%s\n%s\n", employee_id, password);
                fclose(fptr);
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
    bool acc_flag;

    printf("Enter your preferred account type:\n1.Current\n2.Savings\n");
    scanf(" %i", &acc_choice);

    while (acc_flag == false)
    {
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
    char First_Name[50], Last_Name[50], password[20];
    printf("Enter your first name: ");
    scanf("%s", First_Name);
    printf("Enter your last name: ");
    scanf("%s", Last_Name);
    printf("Enter your password: ");
    scanf("%s", password);

    FILE *fptr;
    fptr = fopen("User_Details.txt", "r");
    if (fptr != NULL)
    {
        printf("File is opened\n");
        char buf[100];
        bool fName_flag = false, lName_flag = false, pass_flag = false;
        while (fgets(buf, sizeof(buf), fptr) != NULL)
        {
            buf[strcspn(buf, "\n")] = '\0';
            if (strcmp(First_Name, buf) == 0)
                fName_flag = true;
            if (strcmp(Last_Name, buf) == 0)
                lName_flag = true;
            if (strcmp(password, buf) == 0)
                pass_flag = true;
        }
        if (fName_flag && lName_flag && pass_flag)
        {
            printf("Login Successful\n");
        }
        else
        {
            printf("Login failed\n");
        }
    }
    fclose(fptr);
}
void admin_login()
{
    char employee_id[50], password[20];
    printf("Enter your employee ID: ");
    scanf("%s", employee_id);
    printf("Enter your password: ");
    scanf("%s", password);
    FILE *fptr;
    fptr = fopen("Admin_Details.txt", "r");
    if (fptr != NULL)
    {
        printf("File is opened\n");
        char buf[100];
        bool Id_flag = false, pass_flag = false;
        while (fgets(buf, sizeof(buf), fptr) != NULL)
        {
            buf[strcspn(buf, "\n")] = '\0';
            if (strcmp(employee_id, buf) == 0)
                Id_flag = true;
            if (strcmp(password, buf) == 0)
                pass_flag = true;
        }
        if (Id_flag && pass_flag)
        {
            printf("Login Successful\n");
        }
        else
        {
            printf("Login failed\n");
        }
    }
    fclose(fptr);
}

int main(void)
{
    bool flag;
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
}
