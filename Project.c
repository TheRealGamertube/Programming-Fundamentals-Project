#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LOGIN_ATTEMPTS 3
#define USER_FILE "User_Details.txt"
#define ADMIN_FILE "Admin_Details.txt"
#define ACCOUNT_FILE "Accounts.txt"
#define TRANSACTION_FILE "Transactions.txt"

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

static bool read_amount(double *amount)
{
    if (scanf(" %lf", amount) != 1)
    {
        clear_input();
        printf("Error! Invalid amount\n");
        return false;
    }
    if (*amount <= 0)
    {
        printf("Error! Amount must be greater than 0\n");
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

void add_account(const char *username, const char *type)
{
    FILE *fptr = open_for_append(ACCOUNT_FILE);
    if (fptr != NULL)
    {
        fprintf(fptr, "%s\n%s\n0.00\n", username, type);
        fclose(fptr);
    }
}

double get_balance(const char *username)
{
    FILE *fptr = fopen(ACCOUNT_FILE, "r");
    if (fptr == NULL)
        return -1;

    char line[100];
    int line_number = 0;
    bool id_matches = false;
    double balance = -1;

    while (fgets(line, sizeof(line), fptr) != NULL)
    {
        line[strcspn(line, "\n")] = '\0';
        int pos = line_number % 3;

        if (pos == 0)
            id_matches = (strcmp(line, username) == 0);
        else if (pos == 2 && id_matches)
        {
            balance = atof(line);
            break;
        }
        line_number++;
    }
    fclose(fptr);
    return balance;
}

void set_balance(const char *username, double new_balance)
{
    FILE *in = fopen(ACCOUNT_FILE, "r");
    FILE *out = fopen("Temp.txt", "w");
    if (in == NULL || out == NULL)
    {
        printf("Error! Could not update balance\n");
        return;
    }

    char line[100];
    int line_number = 0;
    bool id_matches = false;

    while (fgets(line, sizeof(line), in) != NULL)
    {
        line[strcspn(line, "\n")] = '\0';
        int pos = line_number % 3;

        if (pos == 0)
            id_matches = (strcmp(line, username) == 0);

        if (pos == 2 && id_matches)
            fprintf(out, "%.2f\n", new_balance);
        else
            fprintf(out, "%s\n", line);

        line_number++;
    }
    fclose(in);
    fclose(out);
    remove(ACCOUNT_FILE);
    rename("Temp.txt", ACCOUNT_FILE);
}

void get_account_type(const char *username, char *type)
{
    FILE *fptr = fopen(ACCOUNT_FILE, "r");
    strcpy(type, "Unknown");
    if (fptr == NULL)
        return;

    char line[100];
    int line_number = 0;
    bool id_matches = false;

    while (fgets(line, sizeof(line), fptr) != NULL)
    {
        line[strcspn(line, "\n")] = '\0';
        int pos = line_number % 3;

        if (pos == 0)
            id_matches = (strcmp(line, username) == 0);
        else if (pos == 1 && id_matches)
        {
            strcpy(type, line);
            break;
        }
        line_number++;
    }
    fclose(fptr);
}

void save_transaction(const char *username, const char *type, double amount,
                      double balance, const char *note)
{
    FILE *fptr = open_for_append(TRANSACTION_FILE);
    if (fptr != NULL)
    {
        fprintf(fptr, "%s|%s|%.2f|%.2f|%s\n", username, type, amount, balance, note);
        fclose(fptr);
    }
}

void check_balance(const char *username)
{
    double balance = get_balance(username);
    printf("Current balance: Rs. %.2f\n", balance);
}

void deposit(const char *username)
{
    double amount;

    printf("Enter amount to deposit: ");
    if (!read_amount(&amount))
        return;

    double balance = get_balance(username) + amount;
    set_balance(username, balance);
    save_transaction(username, "DEPOSIT", amount, balance, "Cash deposit");
    printf("Deposit successful. New balance: Rs. %.2f\n", balance);
}

void withdraw(const char *username)
{
    double amount;

    printf("Enter amount to withdraw: ");
    if (!read_amount(&amount))
        return;

    double balance = get_balance(username);
    if (amount > balance)
    {
        printf("Insufficient funds. Available balance: Rs. %.2f\n", balance);
        return;
    }

    balance = balance - amount;
    set_balance(username, balance);
    save_transaction(username, "WITHDRAW", amount, balance, "Cash withdrawal");
    printf("Withdrawal successful. New balance: Rs. %.2f\n", balance);
}

void transfer(const char *username)
{
    char recipient[50], password[20], answer;
    double amount;

    printf("Enter recipient's username: ");
    scanf("%49s", recipient);

    if (strcmp(recipient, username) == 0)
    {
        printf("Error! You cannot transfer money to yourself\n");
        return;
    }
    if (!record_exists(ACCOUNT_FILE, 3, recipient))
    {
        printf("Error! Recipient account not found\n");
        return;
    }

    printf("Enter amount to transfer: ");
    if (!read_amount(&amount))
        return;

    double my_balance = get_balance(username);
    if (amount > my_balance)
    {
        printf("Insufficient funds. Available balance: Rs. %.2f\n", my_balance);
        return;
    }

    printf("Enter your password to confirm: ");
    scanf("%19s", password);
    if (!check_credentials(USER_FILE, 5, 4, username, password))
    {
        printf("Incorrect password. Transfer cancelled\n");
        return;
    }

    printf("Transfer Rs. %.2f to %s? (y/n): ", amount, recipient);
    scanf(" %c", &answer);
    if (answer != 'y' && answer != 'Y')
    {
        printf("Transfer cancelled\n");
        return;
    }

    double their_balance = get_balance(recipient);
    my_balance = my_balance - amount;
    their_balance = their_balance + amount;
    set_balance(username, my_balance);
    set_balance(recipient, their_balance);

    char note[100];
    sprintf(note, "Transfer to %s", recipient);
    save_transaction(username, "TRANSFER_OUT", amount, my_balance, note);
    sprintf(note, "Transfer from %s", username);
    save_transaction(recipient, "TRANSFER_IN", amount, their_balance, note);

    printf("Transfer successful. New balance: Rs. %.2f\n", my_balance);
}

void transaction_history(const char *username)
{
    FILE *fptr = fopen(TRANSACTION_FILE, "r");
    if (fptr == NULL)
    {
        printf("No transactions yet\n");
        return;
    }

    char line[200], owner[50], type[20], note[100];
    double amount, balance;
    int count = 0;

    printf("\n%-14s %-12s %-12s %s\n", "Type", "Amount", "Balance", "Details");
    while (fgets(line, sizeof(line), fptr) != NULL)
    {
        if (sscanf(line, "%49[^|]|%19[^|]|%lf|%lf|%99[^\n]", owner, type, &amount, &balance, note) == 5)
        {
            if (strcmp(owner, username) == 0)
            {
                printf("%-14s %-12.2f %-12.2f %s\n", type, amount, balance, note);
                count++;
            }
        }
    }
    fclose(fptr);

    if (count == 0)
        printf("No transactions yet\n");
}

void account_details(const char *username)
{
    FILE *fptr = fopen(USER_FILE, "r");
    if (fptr == NULL)
    {
        printf("Error! Could not read account details\n");
        return;
    }

    char line[100], type[20];
    int line_number = 0;
    bool id_matches = false;

    printf("\n--- Account Details ---\n");
    printf("Username     : %s\n", username);

    while (fgets(line, sizeof(line), fptr) != NULL)
    {
        line[strcspn(line, "\n")] = '\0';
        int pos = line_number % 5;

        if (pos == 0)
            id_matches = (strcmp(line, username) == 0);
        else if (pos == 1 && id_matches)
            printf("First name   : %s\n", line);
        else if (pos == 2 && id_matches)
            printf("Last name    : %s\n", line);
        else if (pos == 3 && id_matches)
            printf("Age          : %s\n", line);

        line_number++;
    }
    fclose(fptr);

    get_account_type(username, type);
    printf("Account type : %s\n", type);
    printf("Balance      : Rs. %.2f\n", get_balance(username));
}

void change_password(const char *username)
{
    char old_password[20], new_password[20], confirm_password[20];

    printf("Enter your current password: ");
    scanf("%19s", old_password);
    if (!check_credentials(USER_FILE, 5, 4, username, old_password))
    {
        printf("Incorrect password\n");
        return;
    }

    printf("Enter your new password: ");
    scanf("%19s", new_password);
    printf("Confirm your new password: ");
    scanf("%19s", confirm_password);

    if (strcmp(new_password, confirm_password) != 0)
    {
        printf("Passwords don't match. Password not changed\n");
        return;
    }

    FILE *in = fopen(USER_FILE, "r");
    FILE *out = fopen("Temp.txt", "w");
    if (in == NULL || out == NULL)
    {
        printf("Error! Could not change password\n");
        return;
    }

    char line[100];
    int line_number = 0;
    bool id_matches = false;

    while (fgets(line, sizeof(line), in) != NULL)
    {
        line[strcspn(line, "\n")] = '\0';
        int pos = line_number % 5;

        if (pos == 0)
            id_matches = (strcmp(line, username) == 0);

        if (pos == 4 && id_matches)
            fprintf(out, "%s\n", new_password);
        else
            fprintf(out, "%s\n", line);

        line_number++;
    }
    fclose(in);
    fclose(out);
    remove(USER_FILE);
    rename("Temp.txt", USER_FILE);

    printf("Password changed successfully\n");
}

void user_menu(const char *username)
{
    bool logged_in = true;
    int choice;

    if (!record_exists(ACCOUNT_FILE, 3, username))
        add_account(username, "Savings");

    while (logged_in)
    {
        printf("\n--- Welcome, %s ---\n", username);
        printf("1.Check Balance\n2.Deposit\n3.Withdraw\n4.Transfer Money\n");
        printf("5.Transaction History\n6.Account Details\n7.Change Password\n8.Logout\n");

        if (!read_int(&choice))
        {
            printf("Error! Invalid Input\n");
            continue;
        }

        switch (choice)
        {
        case 1:
            check_balance(username);
            break;
        case 2:
            deposit(username);
            break;
        case 3:
            withdraw(username);
            break;
        case 4:
            transfer(username);
            break;
        case 5:
            transaction_history(username);
            break;
        case 6:
            account_details(username);
            break;
        case 7:
            change_password(username);
            break;
        case 8:
            printf("Logged out\n");
            logged_in = false;
            break;
        default:
            printf("Error! Invalid Input\n");
        }
    }
}

void get_user_details(const char *account_type)
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

            if (record_exists(USER_FILE, 5, username))
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

                FILE *fptr = open_for_append(USER_FILE);
                if (fptr != NULL)
                {
                    printf("The file is opened\n");
                    fprintf(fptr, "%s\n%s\n%s\n%i\n%s\n", username, first_name, last_name, age, password);
                    fclose(fptr);
                    add_account(username, account_type);
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

        if (record_exists(ADMIN_FILE, 2, employee_id))
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

            FILE *fptr = open_for_append(ADMIN_FILE);
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
            acc_flag = true;
            get_user_details("Current");
            break;
        case 2:
            acc_flag = true;
            get_user_details("Savings");
            break;
        default:
            printf("Error! Invalid Input\n");
        }
    }
}

void user_login()
{
    char username[50], password[20];

    FILE *fptr = fopen(USER_FILE, "r");
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

        if (check_credentials(USER_FILE, 5, 4, username, password))
        {
            printf("Login Successful\n");
            user_menu(username);
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

    FILE *fptr = fopen(ADMIN_FILE, "r");
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

        if (check_credentials(ADMIN_FILE, 2, 1, employee_id, password))
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
        printf("\n1.Create User Account\n2.Create Admin Account\n3.User Login\n4.Admin Login\n5.Exit\n");
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
