#include <gtk/gtk.h>
#include <stdio.h>
#include "fighters.h"

char txt[100] = "Fighters.txt";

void colour_str(char *answer, char *fighter, GtkWidget *box)
{
    GtkStyleContext *ctx = gtk_widget_get_style_context(box);

    gtk_style_context_remove_class(ctx, "correct");
    gtk_style_context_remove_class(ctx, "incorrect");

    if (strcmp_ci(answer, fighter) == 0)
        gtk_style_context_add_class(ctx, "correct");
    else
        gtk_style_context_add_class(ctx, "incorrect");
}

int colour_int(int answer, int fighter, GtkWidget *box)
{
    GtkStyleContext *ctx = gtk_widget_get_style_context(box);

    gtk_style_context_remove_class(ctx, "correct");
    gtk_style_context_remove_class(ctx, "incorrect");

    if (fighter == answer)
    {
        gtk_style_context_add_class(ctx, "correct");
        return 0; // if the fighter guessed is equal to answer
    }

    else
    {
        gtk_style_context_add_class(ctx, "incorrect");
        if (answer > fighter)
        {
            return 1; // if the fighter guessed is lower than answer
        }
        else
        {
            return -1; // if the fighter guessed is higher than answer
        }
    }
}

// submit button action
static void on_button_clicked(GtkWidget *button, DATA *ad)
{
    const char *name = gtk_entry_get_text(GTK_ENTRY(ad->entry));
    FIGHTER *fighter = bst_search(ad->bst_root, name);
    GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_widget_set_hexpand(row, TRUE);

    if (!fighter)
    {
        return;
    }

    // name
    GtkWidget *printName = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5); // creates printName box
    gtk_size_group_add_widget(ad->sg_name, printName);
    gtk_widget_set_name(printName, "guess");                         // set name for css selector
    gtk_widget_set_size_request(printName, 120, 75);                 // size of box
    gtk_box_pack_start(GTK_BOX(row), printName, FALSE, FALSE, 0);    // location of box
    GtkWidget *lname = gtk_label_new(fighter->data.name);            // create label of fighter name
    GtkStyleContext *ctxName = gtk_widget_get_style_context(printName);
    gtk_style_context_add_class(ctxName, "guess");
    gtk_box_pack_start(GTK_BOX(printName), lname, TRUE, TRUE, 0); // set data of printName
    colour_str(ad->answer->data.name, fighter->data.name, printName);
    gtk_widget_show_all(printName);

    // weightclass
    GtkWidget *printWeightclass = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5); // creates printWieghtclass box
    gtk_size_group_add_widget(ad->sg_weightclass, printWeightclass);
    gtk_widget_set_name(printWeightclass, "guess");                         // set name for css selector
    gtk_widget_set_size_request(printWeightclass, 120, 75);                 // size of box
    gtk_box_pack_start(GTK_BOX(row), printWeightclass, FALSE, FALSE, 0);    // location of box
    GtkWidget *lweightclass = gtk_label_new(fighter->data.weightclass);     // create label of fighter weightclass
    GtkStyleContext *ctxWeightclass = gtk_widget_get_style_context(printWeightclass);
    gtk_style_context_add_class(ctxWeightclass, "guess");
    gtk_box_pack_start(GTK_BOX(printWeightclass), lweightclass, TRUE, TRUE, 0); // set data of printWeightclass
    colour_str(ad->answer->data.weightclass, fighter->data.weightclass, printWeightclass);
    gtk_widget_show_all(printWeightclass);

    // wins
    GtkWidget *printWins = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);                // creates printWins box
    gtk_size_group_add_widget(ad->sg_wins, printWins);
    gtk_widget_set_name(printWins, "guess");                                        // set name for css selector
    gtk_widget_set_size_request(printWins, 120, 75);                                // size of box
    gtk_box_pack_start(GTK_BOX(row), printWins, FALSE, FALSE, 0);                   // location of box
    int winsHoL = colour_int(ad->answer->data.wins, fighter->data.wins, printWins); // set color and stuff
    char *wins_str;
    // create label of fighter wins
    if (winsHoL == 1)
    {
        wins_str = g_strdup_printf("%d - HIGHER", fighter->data.wins);
    }
    else if (winsHoL == -1)
    {
        wins_str = g_strdup_printf("%d - LOWER", fighter->data.wins);
    }
    else
    {
        wins_str = g_strdup_printf("%d", fighter->data.wins);
    }
    GtkWidget *lwins = gtk_label_new(wins_str);
    g_free(wins_str);
    GtkStyleContext *ctxWins = gtk_widget_get_style_context(printWins);
    gtk_style_context_add_class(ctxWins, "guess");
    gtk_box_pack_start(GTK_BOX(printWins), lwins, TRUE, TRUE, 0); // set data of printWins
    gtk_widget_show_all(printWins);

    // loses
    GtkWidget *printLoses = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5); // creates printLoses box
    gtk_size_group_add_widget(ad->sg_loses, printLoses);
    gtk_widget_set_name(printLoses, "guess");                         // set name for css selector
    gtk_widget_set_size_request(printLoses, 120, 75);                 // size of box
    gtk_box_pack_start(GTK_BOX(row), printLoses, FALSE, FALSE, 0);    // location of box
    int losesHoL = colour_int(ad->answer->data.loses, fighter->data.loses, printLoses);
    char *loses_str;
    // create label of fighter loses
    if (losesHoL == 1)
    {
        loses_str = g_strdup_printf("%d - HIGHER", fighter->data.loses);
    }
    else if (losesHoL == -1)
    {
        loses_str = g_strdup_printf("%d - LOWER", fighter->data.loses);
    }
    else
    {
        loses_str = g_strdup_printf("%d", fighter->data.loses);
    }
    GtkWidget *lloses = gtk_label_new(loses_str);
    g_free(loses_str);
    GtkStyleContext *ctxLoses = gtk_widget_get_style_context(printLoses);
    gtk_style_context_add_class(ctxLoses, "guess");
    gtk_box_pack_start(GTK_BOX(printLoses), lloses, TRUE, TRUE, 0); // set data of printLoses
    gtk_widget_show_all(printLoses);

    // age
    GtkWidget *printAge = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5); // creates printAge box
    gtk_size_group_add_widget(ad->sg_age, printAge);
    gtk_widget_set_name(printAge, "guess");                         // set name for css selector
    gtk_widget_set_size_request(printAge, 120, 75);                 // size of box
    gtk_box_pack_start(GTK_BOX(row), printAge, FALSE, FALSE, 0);    // location of box
    int ageHoL = colour_int(ad->answer->data.age, fighter->data.age, printAge);
    char *age_str;
    // create label of fighter age
    if (ageHoL == 1)
    {
        age_str = g_strdup_printf("%d - HIGHER", fighter->data.age);
    }
    else if (ageHoL == -1)
    {
        age_str = g_strdup_printf("%d - LOWER", fighter->data.age);
    }
    else
    {
        age_str = g_strdup_printf("%d", fighter->data.age);
    }
    GtkWidget *lage = gtk_label_new(age_str);
    g_free(age_str);
    GtkStyleContext *ctxAge = gtk_widget_get_style_context(printAge);
    gtk_style_context_add_class(ctxAge, "guess");
    gtk_box_pack_start(GTK_BOX(printAge), lage, TRUE, TRUE, 0); // set data of printAge
    gtk_widget_show_all(printAge);

    // country
    GtkWidget *printCountry = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5); // creates printCountry box
    gtk_size_group_add_widget(ad->sg_country, printCountry);
    gtk_widget_set_name(printCountry, "guess");                         // set name for css selector
    gtk_widget_set_size_request(printCountry, 120, 75);                 // size of box
    gtk_box_pack_start(GTK_BOX(row), printCountry, FALSE, FALSE, 0);    // location of box
    GtkWidget *lcountry = gtk_label_new(fighter->data.country);         // create label of fighter country
    GtkStyleContext *ctxCountry = gtk_widget_get_style_context(printCountry);
    gtk_style_context_add_class(ctxCountry, "guess");
    gtk_box_pack_start(GTK_BOX(printCountry), lcountry, TRUE, TRUE, 0); // set data of printCountry
    colour_str(ad->answer->data.country, fighter->data.country, printCountry);
    gtk_widget_show_all(printCountry);

    gtk_list_box_insert(GTK_LIST_BOX(ad->listbox), row, 0);
    gtk_widget_show_all(row);
}

void guess(DATA *ad, GtkWidget *button)
{
    FILE *f = fopen(txt, "r");
    if (!f)
    {
        perror("fopen failed");
        return;
    }
    create_bst(f, &ad->bst_root);
    ad->answer = random_fighter(ad->bst_root);
    g_print(ad->answer->data.name);

    ad->entry = GTK_ENTRY(gtk_entry_new());
    gtk_widget_set_size_request(GTK_WIDGET(ad->entry), 250, 30);            // size of entry box
    gtk_grid_attach(GTK_GRID(ad->grid), GTK_WIDGET(ad->entry), 2, 1, 2, 1); // location of text field
    gtk_widget_set_halign(GTK_WIDGET(ad->entry), GTK_ALIGN_END);
    gtk_widget_set_hexpand(GTK_WIDGET(ad->entry), TRUE);

    GtkListStore *store = gtk_list_store_new(1, G_TYPE_STRING);
    bst_to_liststore(ad->bst_root, store);
    GtkEntryCompletion *completion = gtk_entry_completion_new();
    gtk_entry_completion_set_model(completion, GTK_TREE_MODEL(store));
    gtk_entry_completion_set_text_column(completion, 0);
    gtk_entry_set_completion(GTK_ENTRY(ad->entry), completion);
    gtk_entry_completion_set_match_func(completion, match_func, NULL, NULL);
    g_signal_connect(completion, "match-selected", G_CALLBACK(on_match_selected), ad->bst_root);
    gtk_editable_select_region(GTK_EDITABLE(ad->entry), 0, -1);
    gtk_editable_set_position(GTK_EDITABLE(ad->entry), -1);

    g_signal_connect(ad->entry, "activate", G_CALLBACK(on_button_clicked), ad); // enter key calls submit button
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), ad);     // sumbit button action

    fclose(f); // close file
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    gtk_init(&argc, &argv);
    DATA *ad = malloc(sizeof(DATA));
    ad->bst_root = NULL;
    ad->sg_name = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);
    ad->sg_weightclass = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);
    ad->sg_wins = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);
    ad->sg_loses = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);
    ad->sg_age = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);
    ad->sg_country = gtk_size_group_new(GTK_SIZE_GROUP_HORIZONTAL);

    ad->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(ad->window), "UFCdle");
    gtk_window_set_default_size(GTK_WINDOW(ad->window), 1100, 600); // size of screen
    g_signal_connect(ad->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkCssProvider *provider = gtk_css_provider_new(); // creates border and font colour for output boxs
    gtk_css_provider_load_from_data(provider,
                                    ".guess {"
                                    "  border: 2px solid black;"
                                    "  padding: 5px;"
                                    "}"
                                    ".guess.correct {"
                                    "  background-color: green;"
                                    "}"
                                    ".guess.incorrect {"
                                    "  background-color: red;"
                                    "}"
                                    ".guess label {"
                                    "  font-size: 15px;"
                                    "  font-weight: bold;"
                                    "}",
                                    -1, NULL);

    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER);

    g_object_unref(provider);

    // create grid
    ad->grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(ad->grid), 20);
    gtk_grid_set_column_spacing(GTK_GRID(ad->grid), 20);
    // center the grid in the window
    gtk_widget_set_halign(ad->grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(ad->grid, GTK_ALIGN_START);
    gtk_widget_set_hexpand(ad->grid, TRUE);
    gtk_widget_set_vexpand(ad->grid, TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(ad->grid), TRUE);

    // creates scrolled feature
    ad->scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(
        GTK_SCROLLED_WINDOW(ad->scrolled),
        GTK_POLICY_NEVER,
        GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(ad->scrolled), ad->grid);
    gtk_container_add(GTK_CONTAINER(ad->window), ad->scrolled);

    ad->listbox = gtk_list_box_new();
    gtk_widget_set_vexpand(ad->listbox, TRUE);
    gtk_widget_set_hexpand(ad->listbox, TRUE);
    gtk_grid_attach(GTK_GRID(ad->grid), ad->listbox, 0, 4, 6, 1);

    // shows border for grid
    /**
        gtk_widget_set_name(ad->grid, "debug-grid");
        GtkCssProvider *pro = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider,
                                        "#debug-grid > * {"
                                        "  border: 1px solid red;"
                                        "}",
                                        -1, NULL);

        gtk_style_context_add_provider_for_screen(
            gdk_screen_get_default(),
            GTK_STYLE_PROVIDER(pro),
            GTK_STYLE_PROVIDER_PRIORITY_USER);
    */

    // title label
    GtkWidget *title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title), "<span font='35'>UFCdle</span>");
    gtk_grid_attach(GTK_GRID(ad->grid), title, 0, 0, 6, 1); // location of label
    gtk_widget_set_halign(title, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(title, TRUE);

    // submit button
    GtkWidget *button = gtk_button_new_with_label("Submit");
    gtk_grid_attach(GTK_GRID(ad->grid), button, 4, 1, 1, 1); // location of button
    gtk_widget_set_halign(button, GTK_ALIGN_START);
    gtk_widget_set_hexpand(button, TRUE);

    // labels above output boxs
    GtkWidget *name = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(name), "<span font='15'>NAME</span>");
    gtk_grid_attach(GTK_GRID(ad->grid), name, 0, 3, 1, 1); // location of name label
    gtk_widget_set_hexpand(name, TRUE);
    gtk_widget_set_halign(name, GTK_ALIGN_END);

    GtkWidget *weightclass = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(weightclass), "<span font='15'>WIGHTCLASS</span>");
    gtk_grid_attach(GTK_GRID(ad->grid), weightclass, 1, 3, 1, 1); // location of weightclass label
    gtk_widget_set_hexpand(weightclass, TRUE);
    gtk_widget_set_halign(weightclass, GTK_ALIGN_END);

    GtkWidget *wins = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(wins), "<span font='15'>WINS</span>");
    gtk_grid_attach(GTK_GRID(ad->grid), wins, 2, 3, 1, 1); // location of wins label
    gtk_widget_set_hexpand(wins, TRUE);
    gtk_widget_set_halign(wins, GTK_ALIGN_END);

    GtkWidget *loses = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(loses), "<span font='15'>LOSES</span>");
    gtk_grid_attach(GTK_GRID(ad->grid), loses, 3, 3, 1, 1); // location of loses label
    gtk_widget_set_hexpand(loses, TRUE);
    gtk_widget_set_halign(loses, GTK_ALIGN_END);

    GtkWidget *age = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(age), "<span font='15'>AGE</span>");
    gtk_grid_attach(GTK_GRID(ad->grid), age, 4, 3, 1, 1); // location of age label
    gtk_widget_set_hexpand(age, TRUE);
    gtk_widget_set_halign(age, GTK_ALIGN_END);

    GtkWidget *country = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(country), "<span font='15'>COUNTRY</span>");
    gtk_grid_attach(GTK_GRID(ad->grid), country, 5, 3, 1, 1); // location of country label
    gtk_widget_set_hexpand(country, TRUE);
    gtk_widget_set_halign(country, GTK_ALIGN_END);

    // guess method
    guess(ad, button);

    gtk_widget_show_all(ad->window);
    gtk_main();
    return 0;
}

//  cd "C:\CODING\UFC_GuessWho"
//  gcc main.c fighters.c -o run.exe `pkg-config --cflags --libs gtk+-3.0`
//  ./run.exe