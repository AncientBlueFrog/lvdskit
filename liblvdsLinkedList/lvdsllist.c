#include "lvdsllist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DISPLAY_MALLOC_ERROR printf("Failed to allocate memory\n")

struct lvds_list *new_list()
{
    struct lvds_list *l;

    l = calloc(1, sizeof(struct lvds_list));

    return l;
}

struct lvds_list *ll_new_link(struct lvds_list *src_list)
{
    struct lvds_list *l;

    l = calloc(1, sizeof(struct lvds_list));

    l->start = src_list->start;
    l->link = true;

    return l;
}

bool list_add(struct lvds_list *l, void *data)
{
    if (l == NULL)
    {
        printf("Attempt to pass a NULL list\n");
        return false;
    }

    if (l->link)
    {
        printf("Attempt to add in a link\n");
        return false;
    }

    struct ll_node *newnode = malloc(sizeof(struct ll_node));

    if (newnode == NULL)
    {
        DISPLAY_MALLOC_ERROR;
        return false;
    }

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

bool list_sadd(struct lvds_list *l, char *data)
{
    if (l == NULL)
    {
        printf("Attempt to pass a NULL list\n");
        return false;
    }

    if (l->link)
    {
        printf("Attempt to add in a link\n");
        return false;
    }

    list_rewind(l);
    for (char *temp_str; (temp_str = list_get(l));)
    {
        if (!strcmp(temp_str, data))
            return true;
    }

    char *str = strdup(data);
    if (!list_add(l, str))
    {
        DISPLAY_MALLOC_ERROR;
        free(str);
        return false;
    }

    return true;
}

void list_delete_node(struct lvds_list *l, int index)
{
    if (l == NULL)
    {
        printf("Attempt to pass a NULL list\n");
        return;
    }

    if (l->link)
    {
        printf("Attempt to add in a link\n");
        return;
    }

    struct ll_node *node = l->start, *previous_node = NULL;

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

void list_unload_node(struct lvds_list *l, int index)
{
    if (l->link)
    {
        printf("Attempt to unload a link\n");
        return;
    }

    if (l == NULL)
    {
        printf("Attempt to pass a NULL list\n");
        return;
    }

    struct ll_node *node = l->start, *previous_node = NULL;

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

void list_delete(struct lvds_list *l)
{
    if (l->link == false)
    {
        struct ll_node *node = l->start, *previous_node;

        while (node != NULL)
        {
            previous_node = node;

            node = node->next;

            free(previous_node);
        }
    }

    free(l);
}

void list_unload(struct lvds_list *l, void (*cleaner_fun)(void *))
{
    if (!l->link)
    {
        if (l == NULL)
        {
            printf("Attempt to pass a NULL list\n");
            return;
        }

        struct ll_node *node = l->start, *previous_node;

        while (node != NULL)
        {
            previous_node = node;

            node = node->next;

            if (cleaner_fun == NULL)
                free(previous_node->data);
            else
                cleaner_fun(previous_node->data);

            free(previous_node);
        }
    }

    free(l);
}

void list_sunload(struct lvds_list *l)
{
    if (!l->link)
    {
        if (l == NULL)
        {
            printf("Attempt to pass a NULL list\n");
            return;
        }

        struct ll_node *node = l->start, *previous_node;

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

void list_rewind(struct lvds_list *l)
{
    l->cursor = l->start;
}

void *list_get(struct lvds_list *l)
{
    if (l == NULL)
    {
        printf("Attempt to pass a NULL list\n");
        return NULL;
    }

    if (l->cursor == NULL)
        return NULL;

    void *data = l->cursor->data;

    l->cursor = l->cursor->next;

    return data;
}

bool list_search_string(struct lvds_list *l, char *s1)
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

bool list_append(struct lvds_list *dest, struct lvds_list *src)
{
    if (dest->link)
    {
        printf("Attempt to append in a link\n");
        return true;
    }

    list_rewind(src);

    for (void *data; (data = list_get(src));)
    {
        if (!list_add(dest, data))
            return false;
    }

    list_rewind(src);

    return true;
}

bool list_sappend(struct lvds_list *dest, struct lvds_list *src)
{
    if (dest->link)
    {
        printf("Attempt to append in a link\n");
        return true;
    }

    list_rewind(src);
    for (char *src_data; (src_data = list_get(src));)
    {
        if (!list_sadd(dest, src_data))
            return false;
    }

    list_rewind(src);

    return true;
}

bool list_absorb(struct lvds_list *dest, struct lvds_list *src)
{
    if ((dest->link) || (src->link))
    {
        printf("Attempt to absorb a link\n");
        return true;
    }

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
