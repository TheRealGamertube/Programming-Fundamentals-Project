#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

void encrypt(char result[], char key[], unsigned char enc_text[])
{
    int key_counter = 0;
    int ascii_char, sum;
    unsigned char enc_char;

    for (int i = 0; i < strlen(result); i++)
    {
        ascii_char = (unsigned char)result[i];
        if (key_counter < strlen(key) - 2)
        {
            sum = (unsigned char)key[key_counter] + ascii_char;
            enc_char = sum % 256;
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

    for (int i = 0; i < strlen(result); i++)
    {
        printf("%02X", enc_text[i]);
    }
    printf("\n");
}

void decrypt(char enc_word[], char dec_text[], int length, char key[])
{
    int ascii_char, sum;
    int key_counter = 16 % (strlen(key) - 1);
    unsigned char dec_char;

    for (int i = 16; i < length + 16; i++)
    {
        ascii_char = (unsigned char)enc_word[i];
        sum = ascii_char - (unsigned char)key[key_counter];
        dec_char = (sum + 256) % 256;
        dec_text[i - 16] = dec_char;
        if (key_counter < strlen(key) - 2)
        {
            key_counter = key_counter + 1;
        }
        else
        {
            key_counter = 0;
        }
    }
    dec_text[length] = '\0';
    printf("%s\n", dec_text);
}

int main(void)
{
    char word[50];
    char key[50];
    char salt[17];
    char result[100];
    char dec_text[50];
    int salt_char;

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

    salt[strcspn(salt, "\n")] = '\0';
    word[strcspn(word, "\n")] = '\0';

    strcpy(result, salt);
    strcat(result, word);
    strcat(result, salt);

    printf("%s", result);
    printf("\n");

    unsigned char enc_text[strlen(result) + 1];

    encrypt(result, key, enc_text);
    decrypt((char *)enc_text, dec_text, strlen(word), key);

    return 0;
}
