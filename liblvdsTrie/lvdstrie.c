#include "lvdstrie.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct lvds_trie trie;

// Make a new node.
trie *trie_new_node()
{
    trie *ret = malloc(sizeof(trie));

    ret->end = 0;

    for (int i = 0; i < TRIE_MAGIC_NUMBER; i++)
    {
        ret->node[i] = NULL;
    }

    return ret;
}

// Destroying trie
void trie_unload(trie *t)
{
    if (t == NULL)
        return;

    for (int i = 0; i < TRIE_MAGIC_NUMBER; i++)
    {
        if (t->node[i] != NULL)
        {
            trie_unload(t->node[i]);
        }
    }

    free(t);
}

/*
 * Returns true if word is in dictionary else false.
 */

bool trie_check(trie *node, const char *word)
{
    int adr = 0;
    char c;

    trie *next_node;

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
            return false;
        }
        else
        {
            node = next_node;
        }
    }

    if (node->end == true)
        return true;

    return false;
}
/*
bool trie_append(trie *dest_node, trie *src_node)
{
    char c;

    trie *next_node;

    for (int adr = 0; adr < TRIE_MAGIC_NUMBER; adr++)
    {
        if (src_node->end == true)
            {
                trie_load(dest_node ,)
            }

        next_node = src_node->node[adr];

        if (next_node == NULL)
        {
            continue;
        }
        else
        {
            trie_append(dest_node, src_node);
        }
    }

    return false;
}
*/
/*
 * Loads dict into memory.  Returns true if successful else false.
 */

bool trie_load_strings(trie *node, int nmem, char *(*words)[])
{
    for (int i = 0; i < nmem; i++)
    {
        if (!trie_load(node, (*words)[i]))
            return false;
    }

    return true;
}

bool trie_load(trie *node, const char word[])
{
    if (node == NULL)
    {
        printf("Attempt to pass a NULL key to a hashtable\n");
        return false;
    }

    trie *next_node;

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
            next_node = trie_new_node();
            node->node[adr] = next_node;

            if (!next_node)
            {
                return false;
            }
        }
        node = next_node;
    }

    node->end = true;

    return true;
}

bool trie_load_file(trie *node, const char dict_name[])
{
    trie *next_node;

    FILE *dict_file = fopen(dict_name, "r");

    if (!dict_file)
    {
        printf("Não foi possivel abrir arquivo\n");
        return false;
    }

    int c;
    int adr;
    while (c != EOF)
    {
        for (int i = 0; (c = fgetc(dict_file)) != '\n'; i++)
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
                next_node = trie_new_node();
                node->node[adr] = next_node;

                if (!next_node)
                {
                    fclose(dict_file);
                    return false;
                }
            }
            node = next_node;
        }
        node->end = true;
    }

    fclose(dict_file);
    return true;
}
