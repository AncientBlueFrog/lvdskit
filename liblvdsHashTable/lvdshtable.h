#ifndef LVDSHTABLE_H
#define LVDSHTABLE_H

#include <stdbool.h>
#include <stdio.h>

#define HASH_FUNCTION djb2

#define HT_MAGIC_NUMBER 31

typedef struct ht_elem
{
    void *data;
    char *key;
    struct ht_elem *next;
} *lvds_hashtable[HT_MAGIC_NUMBER];

unsigned long HASH_FUNCTION(unsigned char *str);

lvds_hashtable *new_hashtable();
bool hashtable_load(lvds_hashtable *table, void *data, char *key);
bool hashtable_sload(lvds_hashtable *table, void *data, char *key);
bool hashtable_sload_fif(lvds_hashtable *table, void *data, char *key, void (*cleaner_fun)());
void *hashtable_check(lvds_hashtable *table, char *key);
bool hashtable_append(lvds_hashtable *dest, lvds_hashtable *src);
bool hashtable_sappend(lvds_hashtable *dest, lvds_hashtable *src, void (*cleaner_fun)());
bool hashtable_absorb(lvds_hashtable *dest, lvds_hashtable *src);
void hashtable_delete(lvds_hashtable *table);
void hashtable_unload(lvds_hashtable *table, void (*)());
#endif // !LVDSHTABLE
