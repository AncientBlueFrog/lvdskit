#ifndef LVDSHTABLE_H
#define LVDSHTABLE_H

#include <stdbool.h>

#define HASH_FUNCTION djb2

#define HT_MAGIC_NUMBER 31

typedef struct ht_elem
{
    void *data;
    char *key;
    struct ht_elem *next;
} * hashtable[HT_MAGIC_NUMBER];

unsigned long HASH_FUNCTION(unsigned char *str);

hashtable *new_hashtable();
bool hash_add(hashtable table, void *data);
bool hashtable_load(hashtable *table, void *data, char *str);
bool hashtable_sload(hashtable *table, void *data, char *str);
bool hashtable_sload_fif(hashtable *table, void *data, char *key, void (*cleaner_fun)());
void *hashtable_check(hashtable *table, char *str);
bool hashtable_append(hashtable *dest, hashtable *src);
bool hashtable_sappend(hashtable *dest, hashtable *src, void (*cleaner_fun)());
bool hashtable_absorb(hashtable *dest, hashtable *src);
bool hashtable_delete(hashtable *table);
bool hashtable_unload(hashtable *table, void (*)());
#endif // !LVDSHTABLE
