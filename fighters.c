#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <gtk/gtk.h>
#include "fighters.h"

FIGHTER *bst_node(INFO *data)
{
    FIGHTER *np = (FIGHTER *)malloc(sizeof(FIGHTER));
    if (np)
    {
        np->data = *data;
        np->size = 1;
        np->left = NULL;
        np->right = NULL;
    }
    return np;
}

int get_size(FIGHTER *n)
{
    return n ? n->size : 0;
}

void bst_insert(FIGHTER **rootp, INFO *data)
{
    if (*rootp == NULL)
    {
        *rootp = bst_node(data);
        return;
    }

    int cmp = strcmp_ci(data->name, (*rootp)->data.name);

    if (cmp < 0)
        bst_insert(&(*rootp)->left, data);
    else if (cmp > 0)
        bst_insert(&(*rootp)->right, data);

    (*rootp)->size = 1 + get_size((*rootp)->left) + get_size((*rootp)->right);
}

int strcmp_ci(const char *a, const char *b)
{
    while (*a && *b)
    {
        int ca = tolower((unsigned char)*a);
        int cb = tolower((unsigned char)*b);

        if (ca != cb)
            return ca - cb;

        a++;
        b++;
    }
    return tolower((unsigned char)*a) - tolower((unsigned char)*b);
}

FIGHTER *bst_search(FIGHTER *root, const char *name)
{
    if (root == NULL)
        return NULL;

    int cmp = strcmp_ci(name, root->data.name);

    if (cmp == 0)
        return root;
    else if (cmp < 0)
        return bst_search(root->left, name);
    else
        return bst_search(root->right, name);
}

void create_bst(FILE *f, FIGHTER **rootp)
{
    char line[100];

    while (fgets(line, sizeof(line), f))
    {
        INFO *data = malloc(sizeof(INFO));

        int num = 0;
        char temp[100];
        strcpy(temp, line);

        char *token = strtok(temp, ",");
        while (token)
        {
            if (num == 0) // name
            {
                strcpy(data->name, token);
                num++;
            }
            else if (num == 1) // weightclass
            {
                strcpy(data->weightclass, token);
                num++;
            }
            else if (num == 2) // wins
            {
                data->wins = atoi(token);
                num++;
            }
            else if (num == 3) // loses
            {
                data->loses = atoi(token);
                num++;
            }
            else if (num == 4) // age
            {
                data->age = atoi(token);
                num++;
            }
            else if (num == 5) // country
            {
                strcpy(data->country, token);
                num++;
            }
            token = strtok(NULL, ",");
        }

        bst_insert(rootp, data);
    }
}

FIGHTER *random_fighter(FIGHTER *root)
{
    if (!root)
        return NULL;

    int left_size = get_size(root->left);
    int r = rand() % root->size;

    if (r < left_size)
        return random_fighter(root->left);
    else if (r == left_size)
        return root;
    else
        return random_fighter(root->right);
}

void bst_to_liststore(FIGHTER *root, GtkListStore *store)
{
    if (!root)
        return;

    bst_to_liststore(root->left, store);

    GtkTreeIter iter;
    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter, 0, root->data.name, -1);
    bst_to_liststore(root->right, store);
}

gboolean match_func(GtkEntryCompletion *completion, const gchar *key, GtkTreeIter *iter, gpointer user_data)
{
    GtkTreeModel *model = gtk_entry_completion_get_model(completion);

    gchar *text;
    gtk_tree_model_get(model, iter, 0, &text, -1);

    gboolean match = g_strrstr(g_ascii_strdown(text, -1), g_ascii_strdown(key, -1)) != NULL;

    g_free(text);
    return match;
}

gboolean on_match_selected(GtkEntryCompletion *completion, GtkTreeModel *model, GtkTreeIter *iter, gpointer root)
{
    gchar *name;
    gtk_tree_model_get(model, iter, 0, &name, -1);

    GtkWidget *entry = gtk_entry_completion_get_entry(completion);
    gtk_entry_set_text(GTK_ENTRY(entry), name);
    FIGHTER *found = bst_search(root, name);

    g_free(name);
    return TRUE; // stop default handling
}