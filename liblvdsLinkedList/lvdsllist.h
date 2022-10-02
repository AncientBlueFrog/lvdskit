#ifndef LVDSLLIST_H
#define LVDSLLIST_H

#include <stdbool.h>

struct LinkedList
{
    void *data;
    struct LinkedList *next;
};

struct List
{
    bool link;
    struct LinkedList *start, *end, *cursor;
};

struct List *new_list();
struct List *ll_new_link(struct List *src_list);
bool list_add(struct List *l, void *value);
bool list_sadd(struct List *l, char *value);
void list_delete_node(struct List *l, int index);
void list_unload_node(struct List *l, int index);
void list_delete(struct List *l);
void list_unload(struct List *l);
void list_rewind(struct List *l);
void *list_get(struct List *l);
bool list_search_string(struct List *l, char *str);
bool list_append(struct List *dest, struct List *src);
bool list_sappend(struct List *dest, struct List *src);
bool list_absorb(struct List *dest, struct List *src);

#endif // lvdsllist
