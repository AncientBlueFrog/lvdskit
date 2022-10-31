#include "lvdstrie.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct lvds_dtrie dtrie;

// Make a new node.
dtrie *dtrie_new_node()
{
    dtrie *ret = malloc(sizeof(dtrie));

    ret->data = NULL;

    for (int i = 0; i < TRIE_MAGIC_NUMBER; i++)
    {
        ret->node[i] = NULL;
    }

    return ret;
}

// Destroying dtrie
void dtrie_unload(dtrie *t)
{
    if (t == NULL)
        return;
    for (int i = 0; i < 27; i++)
    {
        if (t->node[i] != NULL)
        {
            dtrie_unload(t->node[i]);
        }
    }

    free(t);
}

/*
 * Returns true if word is in dictionary else false.
 */

void *dtrie_check(dtrie *node, const char *word)
{
    int adr = 26;
    char c;

    dtrie *next_node;

    for (int i = 0; word[i] != 0; i++)
    {
        c = toupper(word[i]);

        if (c == '_')
        {
            adr = NUMBER_OF_LETTERS + NUMBER_OF_NUMBERS;
        }
        else if (isdigit(c))
        {
            adr = NUMBER_OF_LETTERS + (c - '0');
        }
        else if ((c >= 'A') && (c <= 'Z'))
        {
            adr = c - 'A';
        }
        else
        {
            adr = STRANGE_VALUE_OFFSET;
        }

        next_node = node->node[adr];

        if (next_node == NULL)
        {
            return NULL;
        }
        else
        {
            node = next_node;
        }
    }

    return node->data;
}

/*
 * Loads dict into memory.  Returns true if successful else false.
 */

bool dtrie_load(dtrie *node, const char word[], void *data)
{
    if (!node)
    {
        printf("Attempt to pass a NULL key to a hashtable\n");
        return false;
    }

    dtrie *next_node;

    int c;
    int adr;

    for (int i = 0; (c = word[i]); i++)
    {
        if (c == EOF)
        {
            break;
        }

        c = toupper(c);

        if (c == '_')
        {
            adr = NUMBER_OF_LETTERS + NUMBER_OF_NUMBERS;
        }
        else if (isdigit(c))
        {
            adr = NUMBER_OF_LETTERS + (c - '0');
        }
        else if ((c >= 'A') && (c <= 'Z'))
        {
            adr = c - 'A';
        }
        else
        {
            adr = STRANGE_VALUE_OFFSET;
        }

        next_node = node->node[adr];

        if (next_node == NULL)
        {
            next_node = dtrie_new_node();
            node->node[adr] = next_node;

            if (!next_node)
            {
                return false;
            }
        }
        node = next_node;
    }

    node->data = data;

    return true;
}
