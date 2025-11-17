#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
int main(void)
{
    int num_words = 1;
    int num_sentences = 0;
    int total_letters = 0;
    string Text = get_string("Text : ");
    int len_Text = strlen(Text);
    for (int i = 0; i < len_Text; i++)
    {
        if (Text[i] == ' ')
        {
            num_words++;
        }
        else if (Text[i] == '.' || Text[i] == '!' || Text[i] == '?')
        {
            num_sentences++;
        }
        else if ((Text[i] >= 'A' && Text[i] <= 'Z') || (Text[i] >= 'a' && Text[i] <= 'z'))
        {
            total_letters++;
        }
    }

    double L = ((double) total_letters / num_words) * 100;
    double S = ((double) num_sentences / num_words) * 100;

    int index = (int) round(0.0588 * L - 0.296 * S - 15.8);
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %d\n", index);
    }
}
