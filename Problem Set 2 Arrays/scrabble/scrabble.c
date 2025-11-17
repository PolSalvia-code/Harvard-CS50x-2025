#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// functions
int points(string player_word);

// global variables
string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; // here we make an array of the alphabet

int punctuation[26] = {
    1, 3, 3, 2,  1, 4, 2, 4, 1, 8, 5, 1, 3,
    1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10}; // here we make an array of the punctuation

int main(void)
{
    string player1 = get_string("Player 1: ");
    string player2 = get_string("Player 2: ");
    int points_player1 = points(player1);
    int points_player2 = points(player2);

    if (points_player1 == points_player2)
    {
        printf("Tie!\n");
    }
    else if (points_player1 < points_player2)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Player 1 wins!\n");
    }
}

int points(string player_word) // Here is declared the function to calculate the points of the
                               // player
{
    int alphabet_len = strlen(alphabet);
    int player_score = 0;
    int len_player = strlen(player_word);
    for (int i = 0; i < len_player; i++)
    {
        int found = 0; // This variable -> 0 if not found 1 if found the position of the leter in
                       // the alphabet

        for (int j = 0; found == 0 && j < alphabet_len; j++)
        {

            if (toupper(player_word[i]) == alphabet[j])
            {
                player_score += punctuation[j];
                found = 1;
            }
        }
    }
    return player_score;
};
