#ifndef LVDSLLIST_H
#define LVDSLLIST_H

#include <stdbool.h>
#include <stdio.h>

struct ll_node
{
    void *data;
    struct ll_node *next;
};

struct lvds_list
{
    bool link;
    struct ll_node *start, *end, *cursor;
};

struct lvds_list *new_list();
struct lvds_list *ll_new_link(struct lvds_list *src_list);
bool list_add(struct lvds_list *l, void *value);
bool list_sadd(struct lvds_list *l, char *value);
void list_delete_node(struct lvds_list *l, int index);
void list_unload_node(struct lvds_list *l, int index);
void list_delete(struct lvds_list *l);
void list_unload(struct lvds_list *l, void (*cleaner)(void *));
void list_sunload(struct lvds_list *l);
void list_rewind(struct lvds_list *l);
void *list_get(struct lvds_list *l);
bool list_search_string(struct lvds_list *l, char *str);
bool list_append(struct lvds_list *dest, struct lvds_list *src);
bool list_sappend(struct lvds_list *dest, struct lvds_list *src);
bool list_absorb(struct lvds_list *dest, struct lvds_list *src);

#endif // lvdsllist
