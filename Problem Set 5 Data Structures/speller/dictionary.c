// Implements a dictionary's functionality

#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 131071;
int num_words = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int hash_check = hash(word);
    if (table[hash_check] == NULL)
    {
        return false;
    }
    else
    {
        node *cursor = table[hash_check];
        while (cursor != NULL)
        {
            if (strcasecmp(word, cursor->word) == 0)
            {
                return true;
            }
            else
            {
                cursor = cursor->next;
            }
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned long h = 0; // starts 0
    for (int i = 0; word[i] != '\0'; i++)
    {
        unsigned char c = (unsigned char) tolower(word[i]);
        if (c == '\'')
            continue;
        h = h * 31 + c;
    }
    return (unsigned int) (h % N);
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open the dictionary
    FILE *dict = fopen(dictionary, "r");
    // Check if NULL
    if (dict == NULL)
    {
        return false;
    }
    // Read words from the dictionry

    char word[LENGTH + 1];
    while (fscanf(dict, "%s", word) != EOF)
    {
        // Create a node for each word
        node *n = malloc(sizeof(node));
        // Check if Malloc NULL
        if (n == NULL)
        {
            return false;
        }
        // prepare the node
        strcpy(n->word, word);
        n->next = NULL;

        // Call the Hash function
        unsigned int hash_dict = hash(word);

        // Store the node in the Hash table FIFO
        n->next = table[hash_dict];
        table[hash_dict] = n;
        num_words += 1;
    }
    // Close dictionary
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    if (num_words == 0)
    {
        return 0;
    }
    else
    {
        return num_words;
    }
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    // TODO
    return true;
}
