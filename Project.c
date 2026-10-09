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
            }
            else
            {
                printf("Passwords don't match\n");
            }
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
            break;
        case 3:
            flag = true;
            break;
        case 4:
            flag = true;
            break;
        case 5:
            printf("Exited\n");
            flag = true;
            break;
        }
    }
}
