#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

int main(void)
// vigenere cipher encryption
{
    char word[50];
    char key[50];
    char salt[17];
    char result[100];
    int key_counter = 0; // counter for the current character of key
    int ascii_char, sum, salt_char;
    unsigned char enc_char;

    srand((unsigned int)time(NULL));

    printf("Enter a string to be encrypted: ");
    fgets(word, sizeof(word), stdin);
    printf("Enter a key for encryption: ");
    fgets(key, sizeof(key), stdin);

    for (int j = 0; j <= 15; j++)
    {
        salt_char = rand() % 26 + 'a';
        salt[j] = salt_char;
    }
    salt[16] = '\0';

    salt[strcspn(salt, "\n")] = '\0'; // removing any newline characters
    word[strcspn(word, "\n")] = '\0';

    strcpy(result, salt);
    strcat(result, word);
    strcat(result, salt);

    printf("%s", result);
    printf("\n");

    unsigned char enc_text[strlen(result) + 1];

    for (int i = 0; i < strlen(result); i++)
    {
        ascii_char = (unsigned char)result[i]; // no toupper, no - 65
        if (key_counter < strlen(key) - 2)
        {
            sum = (unsigned char)key[key_counter] + ascii_char; // no - 97
            enc_char = sum % 256;                               // no + 65
            key_counter = key_counter + 1;
            enc_text[i] = enc_char;
        }
        else
        {
            sum = (unsigned char)key[key_counter] + ascii_char;
            enc_char = sum % 256;
            key_counter = 0;
            enc_text[i] = enc_char;
        }
    }

    for (int i = 0; i < strlen(result); i++) // print as hex
    {
        printf("%02X", enc_text[i]);
    }
    printf("\n");
    return 0;
}