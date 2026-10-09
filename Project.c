#include <stdio.h>
#include <stdbool.h>

int main(void)
{
    bool flag;
    int choice;

    while (flag == false)
    {
        printf("1.Create User Account\n2.Create Admin Account\n3.User Login\n4.Admin Login\n5.Exit");
        scanf("%i", &choice);

        switch(choice)
        {
            case 1:
                flag = true;
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
                flag = false;
                break;

        }

    }
}