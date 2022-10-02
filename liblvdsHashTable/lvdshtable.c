#include "lvdshtable.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

unsigned long djb2(unsigned char *key)
{
    if (key == NULL)
        return 0;

    unsigned long hash = 5381;
    int c;

    while ((c = *key++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

hashtable *new_hashtable()
{
    hashtable *temp_htable = calloc(1, sizeof(hashtable));

    if (temp_htable == NULL)
        return NULL;

    return temp_htable;
}

bool hashtable_delete(hashtable *table)
{
    for (int i = 0; i < HT_MAGIC_NUMBER; i++)
    {
        for (struct ht_elem *list = (*table)[i], *previous_list; (previous_list = list);)
        {
            list = list->next;
            free(previous_list->key);
            free(previous_list);
        }
    }

    free(table);

    return true;
}

bool hashtable_unload(hashtable *table, void (*data_unload_function)())
{
    for (int i = 0; i < HT_MAGIC_NUMBER; i++)
    {
        for (struct ht_elem *list = (*table)[i], *previous_list; (previous_list = list);)
        {
            list = list->next;
            free(previous_list->key);

            if (previous_list->data != NULL)
            {
                if (data_unload_function == NULL)
                    free(previous_list->data);
                else
                    data_unload_function(previous_list->data);
            }
            free(previous_list);
        }
    }

    free(table);

    return true;
}

bool hashtable_load(hashtable *table, void *data, char *key)
{
    if (key == NULL)
        return false;

    unsigned long hash = HASH_FUNCTION((unsigned char *)key);
    int index = hash % HT_MAGIC_NUMBER;

    char *temp_str = strdup(key);

    if (temp_str == NULL)
        return false;

    struct ht_elem *previous_list = NULL;
    for (struct ht_elem *list = (*table)[index]; list != NULL; list = list->next)
        previous_list = list;

    struct ht_elem *next_list = malloc(sizeof(struct ht_elem));

    if (next_list == NULL)
        return false;

    next_list->data = data;
    next_list->key = temp_str;
    next_list->next = NULL;

    if (previous_list == NULL)
        (*table)[index] = next_list;
    else
        previous_list->next = next_list;

    return true;
}

bool hashtable_sload(hashtable *table, void *data, char *key)
{
    if (key == NULL)
        return false;

    unsigned long hash = HASH_FUNCTION((unsigned char *)key);
    int index = hash % HT_MAGIC_NUMBER;

    struct ht_elem *previous_list = NULL;
    for (struct ht_elem *list = (*table)[index]; list != NULL; list = list->next)
    {
        previous_list = list;

        if (!strcmp(key, previous_list->key))
        {
            return true;
        }
    }

    char *temp_str = strdup(key);

    if (temp_str == NULL)
        return false;

    struct ht_elem *next_list = malloc(sizeof(struct ht_elem));

    if (next_list == NULL)
        return false;

    next_list->data = data;
    next_list->key = temp_str;
    next_list->next = NULL;

    if (previous_list == NULL)
        (*table)[index] = next_list;
    else
        previous_list->next = next_list;

    return true;
}

bool hashtable_sload_fif(hashtable *table, void *data, char *key, void (*cleaner_fun)())
{
    if (key == NULL)
        return false;

    unsigned long hash = HASH_FUNCTION((unsigned char *)key);
    int index = hash % HT_MAGIC_NUMBER;

    struct ht_elem *previous_list = NULL;
    for (struct ht_elem *list = (*table)[index]; list != NULL; list = list->next)
    {
        previous_list = list;

        if (!strcmp(key, previous_list->key))
        {
            if (data == NULL)
                return true;

            if (cleaner_fun == NULL)
                free(data);
            else
                cleaner_fun(data);

            return true;
        }
    }

    char *temp_str = strdup(key);

    if (temp_str == NULL)
        return false;

    struct ht_elem *next_list = malloc(sizeof(struct ht_elem));

    if (next_list == NULL)
        return false;

    next_list->data = data;
    next_list->key = temp_str;
    next_list->next = NULL;

    if (previous_list == NULL)
        (*table)[index] = next_list;
    else
        previous_list->next = next_list;

    return true;
}

void *hashtable_check(hashtable *table, char *key)
{
    unsigned long hash = HASH_FUNCTION((unsigned char *)key);
    int index = hash % HT_MAGIC_NUMBER;

    for (struct ht_elem *list = (*table)[index]; list != NULL; list = list->next)
    {
        if (strcmp(list->key, key) == 0)
        {
            if (list->data == NULL)
                return list->key;

            return list->data;
        }
    }

    return false;
}

bool hashtable_append(hashtable *dest, hashtable *src)
{
    if ((src == NULL) || (dest == NULL))
        return true;

    for (int i = 0; i < HT_MAGIC_NUMBER; i++)
    {
        for (struct ht_elem *src_list = (*src)[i]; src_list != NULL; src_list = src_list->next)
            hashtable_load(dest, src_list->data, src_list->key);
    }

    return true;
}

bool hashtable_sappend(hashtable *dest, hashtable *src, void (*cleaner_fun)())
{
    if ((src == NULL) || (dest == NULL))
        return true;

    for (int i = 0; i < HT_MAGIC_NUMBER; i++)
    {
        for (struct ht_elem *src_list = (*src)[i]; src_list != NULL; src_list = src_list->next)
            hashtable_sload_fif(dest, src_list->data, src_list->key, cleaner_fun);
    }

    return true;
}
bool hashtable_absorb(hashtable *dest, hashtable *src)
{
    for (int i = 0; i < HT_MAGIC_NUMBER; i++)
    {
        for (struct ht_elem *src_list = (*src)[i]; src_list != NULL; src_list = src_list->next)
            hashtable_load(dest, src_list->data, src_list->key);
    }

    hashtable_delete(src);
    return true;
}
