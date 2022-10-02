#ifndef LVDSTRIE_H
#define LVDSTRIE_H

#include <stdbool.h>

#define TRIE_MAGIC_NUMBER 38
#define NUMBER_OF_LETTERS 26
#define NUMBER_OF_NUMBERS 10
#define STRANGE_VALUE_OFFSET 37

// trie struct
struct Trie
{
    bool end;
    struct Trie *node[TRIE_MAGIC_NUMBER];
};

struct Data_Trie
{
    void *data;
    struct Data_Trie *node[TRIE_MAGIC_NUMBER];
};
// Make a new node.
struct Trie *trie_new_node();

/*
 * Returns true if word is in dictionary else false.
 */

bool trie_check(struct Trie *, const char word[]);

/*
 * Loads dict into memory.  Returns true if successful else false.
 */

bool trie_load(struct Trie *, const char[]);
bool trie_load_strings(struct Trie *, int, char *(*)[]);
bool trie_load_file(struct Trie *, const char[]);

/*
 * Unloads dictionary from memory.  Returns true if successful else false.
 */

void trie_unload(struct Trie *);

// Data Trie
struct Data_Trie *dtrie_new_node();
void *dtrie_check(struct Data_Trie *, const char word[]);
void dtrie_unload(struct Data_Trie *);
bool dtrie_load(struct Data_Trie *, const char[], void *);

#endif //
