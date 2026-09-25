#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void)
//vigenere cipher encryption
{
    char word[50];
    char key[50];
    char enc_text[] = "";
    int key_counter = 0; // counter for the current character of key
    int ascii_char, sum;
    char enc_char, dec_char;

    printf("Enter a string to be encrypted (char only): ");
    fgets(word, sizeof(word), stdin);
    printf("Enter a key for encryption (char only): ");
    fgets(key, sizeof(key), stdin);

    for (int i = 0; i < strlen(word) - 1; i++)
    {
        ascii_char = toupper((int)word[i]);
        if (ascii_char != ' ')
        {
            if (key_counter < strlen(key) - 2) //-2 because strlen returns 4 (4 because an extra character of \n stored and strlen starts from 1 not 0)
            {
                sum = (((int)key[key_counter] - 97) + (ascii_char - 65));
                enc_char = (sum % 26) + 65;
                key_counter = key_counter + 1;
            }
            else
            {
                sum = (((int)key[key_counter] - 97) + (ascii_char - 65));
                enc_char = (sum % 26) + 65;
                key_counter = key_counter + 1;
                key_counter = 0;
            }
            printf("%c", enc_char);
        }
        else
        {
            printf(" ");
        }
    }
}
