#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#ifndef fighters_H_
#define fighters_H_

typedef struct info {
    char name[50];
    char weightclass[20];
    int wins;
    int loses;
    int age;
    char country[20];
}INFO;

typedef struct fighter {
    INFO data;
    int size;
    struct fighter *left;
    struct fighter *right;
} FIGHTER;

typedef struct data{
    FIGHTER *bst_root;
    FIGHTER *answer;
    GtkWidget *listbox;
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *scrolled;
    GtkEntry *entry;

    GtkSizeGroup  *sg_name;
    GtkSizeGroup  *sg_weightclass;
    GtkSizeGroup  *sg_wins;
    GtkSizeGroup  *sg_loses;
    GtkSizeGroup  *sg_age;
    GtkSizeGroup  *sg_country;
} DATA;

FIGHTER *bst_node(INFO *data);
int get_size(FIGHTER *n);
void bst_insert(FIGHTER **rootp, INFO *data);
int strcmp_ci(const char *a, const char *b);
FIGHTER *bst_search(FIGHTER *root, const char *name);
void create_bst(FILE *f, FIGHTER **root);
FIGHTER *random_fighter(FIGHTER *root);
void bst_to_liststore(FIGHTER *root, GtkListStore *store);
gboolean match_func(GtkEntryCompletion *completion, const gchar *key, GtkTreeIter *iter, gpointer user_data);
gboolean on_match_selected(GtkEntryCompletion *completion, GtkTreeModel *model, GtkTreeIter *iter, gpointer user_data);

#endif