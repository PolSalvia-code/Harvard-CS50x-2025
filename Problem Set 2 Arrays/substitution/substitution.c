#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(int argc, string argv[])
{
    // Check if the Key is correct

    if (argc == 1 || argc > 2)
    {
        printf("Usage: ./substitution KEY\n");
        return 1;
    }
    else if (strlen(argv[1]) < 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    int len_argv = strlen(argv[1]);
    for (int i = 0; i < len_argv; i++)
    {
        if (!((argv[1][i] >= 'A' && argv[1][i] <= 'Z') || (argv[1][i] >= 'a' && argv[1][i] <= 'z')))
        {
            printf("Key must only contain alphabetic characters.\n");
            return 1;
        }
        else
        {
            for (int j = i + 1; j < len_argv; j++)
            {
                if (tolower(argv[1][i]) == tolower(argv[1][j]))
                {
                    printf("Key must no contain repeated characters.\n");
                    return 1;
                }
            }
        }
    }

    // We apply the encrypt
    string plain_text = get_string("plaintext: ");
    int len_plain_text = strlen(plain_text);

    string ciphertext = plain_text; // inicialize the ciphertext to the argv string
    for (int i = 0; i < len_plain_text; i++)
    {
        if (plain_text[i] >= 'A' && plain_text[i] <= 'Z')
        {
            int diference = plain_text[i] - 'A';
            ciphertext[i] = toupper(argv[1][diference]);
        }
        else if (plain_text[i] >= 'a' && plain_text[i] <= 'z')
        {
            int diference = plain_text[i] - 'a';
            ciphertext[i] = tolower(argv[1][diference]);
        }
    }
    // print the result
    printf("ciphertext: %s\n", ciphertext);
}
