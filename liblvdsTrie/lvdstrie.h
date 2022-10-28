#ifndef LVDSTRIE_H
#define LVDSTRIE_H

#include <stdbool.h>

#define TRIE_MAGIC_NUMBER 38
#define NUMBER_OF_LETTERS 26
#define NUMBER_OF_NUMBERS 10
#define STRANGE_VALUE_OFFSET 37

// trie struct
struct lvds_trie
{
    bool end;
    struct lvds_trie *node[TRIE_MAGIC_NUMBER];
};

struct lvds_dtrie
{
    void *data;
    struct lvds_dtrie *node[TRIE_MAGIC_NUMBER];
};
// Make a new node.
struct lvds_trie *trie_new_node();

/*
 * Returns true if word is in dictionary else false.
 */

bool trie_check(struct lvds_trie *, const char word[]);

/*
 * Loads dict into memory.  Returns true if successful else false.
 */

bool trie_load(struct lvds_trie *, const char[]);
bool trie_load_strings(struct lvds_trie *, int, char *(*)[]);
bool trie_load_file(struct lvds_trie *, const char[]);

/*
 * Unloads dictionary from memory.  Returns true if successful else false.
 */

void trie_unload(struct lvds_trie *);

// Data lvds_trie
struct lvds_dtrie *dtrie_new_node();
void *dtrie_check(struct lvds_dtrie *, const char word[]);
void dtrie_unload(struct lvds_dtrie *);
bool dtrie_load(struct lvds_dtrie *, const char[], void *);

#endif //
