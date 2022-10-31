#include "lvdshtable.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DISPLAY_MALLOC_ERROR printf("Failed to allocate memory\n")

typedef lvds_hashtable hashtable;

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

void hashtable_delete(hashtable *table)
{
    if (table == NULL)
        return;

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

    return;
}

void hashtable_unload(hashtable *table, void (*cleaner_fun)(void *))
{
    if (table == NULL)
        return;

    for (int i = 0; i < HT_MAGIC_NUMBER; i++)
    {
        for (struct ht_elem *list = (*table)[i], *previous_list; (previous_list = list);)
        {
            list = list->next;
            free(previous_list->key);

            if (previous_list->data != NULL)
            {
                if (cleaner_fun == NULL)
                    free(previous_list->data);
                else
                    cleaner_fun(previous_list->data);
            }
            free(previous_list);
        }
    }

    free(table);

    return;
}

bool hashtable_load(hashtable *table, void *data, char *key)
{
    if (key == NULL)
    {
        printf("Attempt to pass a NULL key to a hashtable\n");
        return false;
    }

    unsigned long hash = HASH_FUNCTION((unsigned char *)key);
    int index = hash % HT_MAGIC_NUMBER;

    char *temp_str = strdup(key);

    if (temp_str == NULL)
    {
        DISPLAY_MALLOC_ERROR;
        return false;
    }
    struct ht_elem *previous_list = NULL;
    for (struct ht_elem *list = (*table)[index]; list != NULL; list = list->next)
        previous_list = list;

    struct ht_elem *next_list = malloc(sizeof(struct ht_elem));

    if (next_list == NULL)
    {
        DISPLAY_MALLOC_ERROR;
        return false;
    }

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
    {
        printf("Attempt to pass a NULL key to a hashtable\n");
        return false;
    }

    unsigned long hash = HASH_FUNCTION((unsigned char *)key);
    int index = hash % HT_MAGIC_NUMBER;
    char *temp_str = NULL;

    struct ht_elem *previous_list = NULL;
    for (struct ht_elem *list = (*table)[index]; list != NULL; list = list->next)
    {
        previous_list = list;

        // Verify if the key is already in the hashtable.
        if (!strcmp(key, previous_list->key))
        {
            // If the table returns data(not NULL) from the key, end function.
            if (hashtable_check(table, key) == NULL)
                hashtable_load(table, data, key);

            return true;
        }
    }

    temp_str = strdup(key);

    if (temp_str == NULL)
    {
        DISPLAY_MALLOC_ERROR;
        return false;
    }

    struct ht_elem *next_list = malloc(sizeof(struct ht_elem));

    if (next_list == NULL)
    {
        DISPLAY_MALLOC_ERROR;
        return false;
    }

    next_list->data = data;
    next_list->key = temp_str;
    next_list->next = NULL;

    if (previous_list == NULL)
        (*table)[index] = next_list;
    else
        previous_list->next = next_list;

    return true;
}

bool hashtable_sload_fif(hashtable *table, void *data, char *key, void (*cleaner_fun)(void *))
{
    if (key == NULL)
    {
        printf("Attempt to pass a NULL key to a hashtable\n");
        return false;
    }

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
    {
        DISPLAY_MALLOC_ERROR;
        return false;
    }

    struct ht_elem *next_list = malloc(sizeof(struct ht_elem));

    if (next_list == NULL)
    {
        DISPLAY_MALLOC_ERROR;
        return false;
    }

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
    if ((table == NULL) || (key == NULL))
    {
        printf("Attempt to pass NULL values to function\n");
        return NULL;
    }

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

    return NULL;
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
