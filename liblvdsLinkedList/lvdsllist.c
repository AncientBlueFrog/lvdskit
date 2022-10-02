#include "lvdsllist.h"
#include <stdlib.h>
#include <string.h>

struct List *new_list()
{
    struct List *l;

    l = calloc(1, sizeof(struct List));

    return l;
}

struct List *ll_new_link(struct List *src_list)
{
    struct List *l;

    l = calloc(1, sizeof(struct List));

    l->start = src_list->start;
    l->link = true;

    return l;
}

bool list_add(struct List *l, void *data)
{
    if (l == NULL)
        return false;

    if (l->link)
        return false;

    struct LinkedList *newnode = malloc(sizeof(struct LinkedList));

    if (newnode == NULL)
        return false;

    newnode->data = data;
    newnode->next = NULL;

    if (l->start == NULL)
    {
        l->start = newnode;
        l->end = newnode;
        l->cursor = NULL;
    }
    else
    {
        l->end->next = newnode;
        l->end = newnode;
    }

    return true;
}

bool list_sadd(struct List *l, char *data)
{
    if (l == NULL)
        return false;

    if (l->link)
        return false;

    list_rewind(l);
    for (char *str; (str = list_get(l));)
    {
        if (!strcmp(str, data))
            return true;
    }

    struct LinkedList *newnode = malloc(sizeof(struct LinkedList));

    if (newnode == NULL)
        return false;

    newnode->data = strdup(data);

    if (newnode->data == NULL)
        return false;

    newnode->next = NULL;

    if (l->start == NULL)
    {
        l->start = newnode;
        l->end = newnode;
        l->cursor = NULL;
    }
    else
    {
        l->end->next = newnode;
        l->end = newnode;
    }

    return true;
}

void list_delete_node(struct List *l, int index)
{
    if (l->link)
        return;

    struct LinkedList *node = l->start, *previous_node = NULL;

    for (int i = 0; i < index; i++)
    {
        if (node == NULL)
            break;

        previous_node = node;
        node = node->next;
    }

    if (previous_node == NULL)
    {
        l->start = node->next;
        free(node);
    }
    else
    {
        previous_node->next = node->next;
        free(node);
    }
}

void list_unload_node(struct List *l, int index)
{
    if (l->link)
        return;

    if (l == NULL)
        return;

    struct LinkedList *node = l->start, *previous_node = NULL;

    for (int i = 0; i < index; i++)
    {
        if (node == NULL)
            break;

        previous_node = node;
        node = node->next;
    }

    if (previous_node == NULL)
    {
        free(node->data);

        l->start = node->next;
        free(node);
    }
    else
    {
        free(node->data);

        previous_node->next = node->next;
        free(node);
    }
}

void list_delete(struct List *l)
{
    if (l->link == false)
    {
        struct LinkedList *node = l->start, *previous_node;

        while (node != NULL)
        {
            previous_node = node;

            node = node->next;

            free(previous_node);
        }
    }

    free(l);
}

void list_unload(struct List *l)
{
    if (!l->link)
    {
        if (l == NULL)
            return;

        struct LinkedList *node = l->start, *previous_node;

        while (node != NULL)
        {
            previous_node = node;

            node = node->next;

            free(previous_node->data);

            free(previous_node);
        }
    }

    free(l);
}

void list_rewind(struct List *l)
{
    l->cursor = l->start;
}

void *list_get(struct List *l)
{
    if (l == NULL)
        return NULL;

    if (l->cursor == NULL)
        return NULL;

    void *data = l->cursor->data;

    l->cursor = l->cursor->next;

    return data;
}

bool list_search_string(struct List *l, char *s1)
{
    list_rewind(l);

    for (void *s2; (s2 = list_get(l));)
    {
        if (!strcmp(s1, s2))
            return true;
    }

    list_rewind(l);
    return false;
}

bool list_append(struct List *dest, struct List *src)
{
    if (dest->link)
        return true;

    list_rewind(src);

    for (void *data; (data = list_get(src));)
    {
        if (!list_add(dest, data))
            return false;
    }

    list_rewind(src);

    return true;
}

bool list_sappend(struct List *dest, struct List *src)
{
    if (dest->link)
        return true;

    list_rewind(src);
    for (char *src_data; (src_data = list_get(src));)
    {
        if (!list_sadd(dest, src_data))
            return false;
    }

    list_rewind(src);

    return true;
}

bool list_absorb(struct List *dest, struct List *src)
{
    if ((dest->link) || (src->link))
        return true;

    list_rewind(src);

    for (void *data; (data = list_get(src));)
    {
        if (!list_add(dest, data))
            return false;
    }

    list_rewind(src);
    list_delete(src);

    return true;
}
