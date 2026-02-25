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

// htp button
static void on_htp_clicked(GtkWidget *button, DATA *ad)
{
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "How to Play",
        GTK_WINDOW(ad->window),
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "Close", GTK_RESPONSE_CLOSE,
        NULL);

    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 100);

    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    GtkWidget *label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label),
                         "<span font='13' weight='bold'>How to Play UFCdle</span>\n\n"
                         "<span font='11'>"
                         "1. A random UFC fighter has been selected.\n\n"
                         "2. Type a fighter's name in the search box and press Submit.\n\n"
                         "3. Each guess reveals stats compared to the mystery fighter:\n"
                         "   - <b>GREEN</b> = correct\n"
                         "   - <b>RED</b> = incorrect\n"
                         "   - <b>HIGHER/LOWER</b> = the answer is higher or lower\n\n"
                         "4. Keep guessing until you find the fighter!"
                         "</span>");

    gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
    gtk_widget_set_margin_start(label, 20);
    gtk_widget_set_margin_end(label, 20);
    gtk_widget_set_margin_top(label, 20);
    gtk_widget_set_margin_bottom(label, 20);

    gtk_container_add(GTK_CONTAINER(content), label);
    gtk_widget_show_all(dialog);

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// helper method for reset button
static void remove_if_not_header(GtkWidget *widget, gpointer data)
{
    GtkListBox *listbox = GTK_LIST_BOX(data);
    GtkListBoxRow *row = GTK_LIST_BOX_ROW(widget);
    if (gtk_list_box_row_get_index(row) != 0)
    {
        gtk_widget_destroy(widget);
    }
}
// reset button action
static void on_reset_clicked(GtkWidget *reset, DATA *ad)
{
    ad->answer = random_fighter(ad->bst_root);
    gtk_container_foreach(GTK_CONTAINER(ad->listbox), remove_if_not_header, ad->listbox);
}

static void on_hint_reveal(GtkWidget *button, HINT_DATA *hd)
{
    char msg[100];

    switch (hd->stat)
    {
    case HINT_NAME:
        snprintf(msg, sizeof(msg), "Name: %s", hd->ad->answer->data.name);
        break;
    case HINT_WEIGHTCLASS:
        snprintf(msg, sizeof(msg), "Weightclass: %s", hd->ad->answer->data.weightclass);
        break;
    case HINT_WINS:
        snprintf(msg, sizeof(msg), "Wins: %d", hd->ad->answer->data.wins);
        break;
    case HINT_LOSES:
        snprintf(msg, sizeof(msg), "Loses: %d", hd->ad->answer->data.loses);
        break;
    case HINT_AGE:
        snprintf(msg, sizeof(msg), "Age: %d", hd->ad->answer->data.age);
        break;
    case HINT_COUNTRY:
        snprintf(msg, sizeof(msg), "Country: %s", hd->ad->answer->data.country);
        break;
    }

    gtk_button_set_label(GTK_BUTTON(button), msg);
    gtk_widget_set_sensitive(button, FALSE); // disable after reveal
}

// hint button action
static void on_hint_clicked(GtkWidget *button, DATA *ad)
{
    GtkWidget *hint_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(hint_window), "Hints");
    gtk_window_set_default_size(GTK_WINDOW(hint_window), 400, 400);
    gtk_window_set_transient_for(GTK_WINDOW(hint_window), GTK_WINDOW(ad->window));

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_start(vbox, 20);
    gtk_widget_set_margin_end(vbox, 20);
    gtk_widget_set_margin_top(vbox, 20);
    gtk_widget_set_margin_bottom(vbox, 20);
    gtk_container_add(GTK_CONTAINER(hint_window), vbox);

    GtkWidget *title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title), "<span font='15' weight='bold'>Reveal a Hint</span>");
    gtk_box_pack_start(GTK_BOX(vbox), title, FALSE, FALSE, 0);

    GtkWidget *btn_name = gtk_button_new_with_label("Reveal Name");
    GtkWidget *btn_weightclass = gtk_button_new_with_label("Reveal Weightclass");
    GtkWidget *btn_wins = gtk_button_new_with_label("Reveal Wins");
    GtkWidget *btn_loses = gtk_button_new_with_label("Reveal Loses");
    GtkWidget *btn_age = gtk_button_new_with_label("Reveal Age");
    GtkWidget *btn_country = gtk_button_new_with_label("Reveal Country");

    gtk_box_pack_start(GTK_BOX(vbox), btn_name, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), btn_weightclass, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), btn_wins, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), btn_loses, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), btn_age, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), btn_country, TRUE, TRUE, 0);

    HINT_DATA *hd_name = g_new(HINT_DATA, 1);
    hd_name->ad = ad;
    hd_name->stat = HINT_NAME;
    HINT_DATA *hd_weightclass = g_new(HINT_DATA, 1);
    hd_weightclass->ad = ad;
    hd_weightclass->stat = HINT_WEIGHTCLASS;
    HINT_DATA *hd_wins = g_new(HINT_DATA, 1);
    hd_wins->ad = ad;
    hd_wins->stat = HINT_WINS;
    HINT_DATA *hd_loses = g_new(HINT_DATA, 1);
    hd_loses->ad = ad;
    hd_loses->stat = HINT_LOSES;
    HINT_DATA *hd_age = g_new(HINT_DATA, 1);
    hd_age->ad = ad;
    hd_age->stat = HINT_AGE;
    HINT_DATA *hd_country = g_new(HINT_DATA, 1);
    hd_country->ad = ad;
    hd_country->stat = HINT_COUNTRY;

    g_signal_connect(btn_name, "clicked", G_CALLBACK(on_hint_reveal), hd_name);
    g_signal_connect(btn_weightclass, "clicked", G_CALLBACK(on_hint_reveal), hd_weightclass);
    g_signal_connect(btn_wins, "clicked", G_CALLBACK(on_hint_reveal), hd_wins);
    g_signal_connect(btn_loses, "clicked", G_CALLBACK(on_hint_reveal), hd_loses);
    g_signal_connect(btn_age, "clicked", G_CALLBACK(on_hint_reveal), hd_age);
    g_signal_connect(btn_country, "clicked", G_CALLBACK(on_hint_reveal), hd_country);

    gtk_widget_show_all(hint_window);
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
    gtk_widget_set_name(printName, "guess");                    // set name for css selector
    gtk_widget_set_size_request(printName, 120, 75);            // size of box
    gtk_box_pack_start(GTK_BOX(row), printName, TRUE, TRUE, 0); // location of box
    GtkWidget *lname = gtk_label_new(fighter->data.name);       // create label of fighter name
    GtkStyleContext *ctxName = gtk_widget_get_style_context(printName);
    gtk_style_context_add_class(ctxName, "guess");
    gtk_box_pack_start(GTK_BOX(printName), lname, TRUE, TRUE, 0); // set data of printName
    colour_str(ad->answer->data.name, fighter->data.name, printName);
    gtk_widget_show_all(printName);

    // weightclass
    GtkWidget *printWeightclass = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5); // creates printWieghtclass box
    gtk_size_group_add_widget(ad->sg_weightclass, printWeightclass);
    gtk_widget_set_name(printWeightclass, "guess");                     // set name for css selector
    gtk_widget_set_size_request(printWeightclass, 120, 75);             // size of box
    gtk_box_pack_start(GTK_BOX(row), printWeightclass, TRUE, TRUE, 0);  // location of box
    GtkWidget *lweightclass = gtk_label_new(fighter->data.weightclass); // create label of fighter weightclass
    GtkStyleContext *ctxWeightclass = gtk_widget_get_style_context(printWeightclass);
    gtk_style_context_add_class(ctxWeightclass, "guess");
    gtk_box_pack_start(GTK_BOX(printWeightclass), lweightclass, TRUE, TRUE, 0); // set data of printWeightclass
    colour_str(ad->answer->data.weightclass, fighter->data.weightclass, printWeightclass);
    gtk_widget_show_all(printWeightclass);

    // wins
    GtkWidget *printWins = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5); // creates printWins box
    gtk_size_group_add_widget(ad->sg_wins, printWins);
    gtk_widget_set_name(printWins, "guess");                                        // set name for css selector
    gtk_widget_set_size_request(printWins, 120, 75);                                // size of box
    gtk_box_pack_start(GTK_BOX(row), printWins, TRUE, TRUE, 0);                     // location of box
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
    gtk_widget_set_name(printLoses, "guess");                    // set name for css selector
    gtk_widget_set_size_request(printLoses, 120, 75);            // size of box
    gtk_box_pack_start(GTK_BOX(row), printLoses, TRUE, TRUE, 0); // location of box
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
    gtk_widget_set_name(printAge, "guess");                    // set name for css selector
    gtk_widget_set_size_request(printAge, 120, 75);            // size of box
    gtk_box_pack_start(GTK_BOX(row), printAge, TRUE, TRUE, 0); // location of box
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
    gtk_widget_set_name(printCountry, "guess");                    // set name for css selector
    gtk_widget_set_size_request(printCountry, 120, 75);            // size of box
    gtk_box_pack_start(GTK_BOX(row), printCountry, TRUE, TRUE, 0); // location of box
    GtkWidget *lcountry = gtk_label_new(fighter->data.country);    // create label of fighter country
    GtkStyleContext *ctxCountry = gtk_widget_get_style_context(printCountry);
    gtk_style_context_add_class(ctxCountry, "guess");
    gtk_box_pack_start(GTK_BOX(printCountry), lcountry, TRUE, TRUE, 0); // set data of printCountry
    colour_str(ad->answer->data.country, fighter->data.country, printCountry);
    gtk_widget_show_all(printCountry);

    gtk_list_box_insert(GTK_LIST_BOX(ad->listbox), row, 1);
    GtkListBoxRow *lbrow = gtk_list_box_get_row_at_index(GTK_LIST_BOX(ad->listbox), 0);
    gtk_widget_set_hexpand(GTK_WIDGET(lbrow), TRUE);
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
    // g_print(ad->answer->data.name);

    // text feild
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
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(ad->listbox), GTK_SELECTION_NONE);
    gtk_container_set_border_width(GTK_CONTAINER(ad->listbox), 0);

    // create overlay for top title row
    GtkWidget *overlay = gtk_overlay_new();
    gtk_widget_set_hexpand(overlay, TRUE);

    GtkWidget *info = gtk_label_new(NULL); // info label
    gtk_label_set_markup(GTK_LABEL(info), "<span font='10'>Fighter stats as of UFC 324 / Jan 24, 2026</span>");
    gtk_widget_set_halign(info, GTK_ALIGN_START);
    gtk_widget_set_valign(info, GTK_ALIGN_CENTER);
    GtkWidget *title = gtk_label_new(NULL); // title label
    gtk_label_set_markup(GTK_LABEL(title), "<span font='35'>UFCdle</span>");
    gtk_widget_set_halign(title, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(title, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(title, TRUE);
    GtkWidget *htp_button = gtk_button_new_with_label("?"); // htp button
    gtk_widget_set_halign(htp_button, GTK_ALIGN_END);
    gtk_widget_set_valign(htp_button, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_end(htp_button, 250); // make margin at end bigger (pushs button closer to center)

    gtk_container_add(GTK_CONTAINER(overlay), title);          // base layer, full width
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), info);       // add info
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), htp_button); // add htp button
    g_signal_connect(htp_button, "clicked", G_CALLBACK(on_htp_clicked), ad);

    gtk_grid_attach(GTK_GRID(ad->grid), overlay, 0, 0, 6, 1); // attach overlay to grid

    // hint button
    GtkWidget *hint = gtk_button_new_with_label("Hint");
    gtk_grid_attach(GTK_GRID(ad->grid), hint, 1, 1, 1, 1); // location of button
    gtk_widget_set_halign(hint, GTK_ALIGN_START);
    gtk_widget_set_hexpand(hint, TRUE);
    g_signal_connect(hint, "clicked", G_CALLBACK(on_hint_clicked), ad); // reset button action

    // reset button
    GtkWidget *reset = gtk_button_new_with_label("New Game");
    gtk_grid_attach(GTK_GRID(ad->grid), reset, 5, 1, 1, 1); // location of button
    gtk_widget_set_halign(reset, GTK_ALIGN_START);
    gtk_widget_set_hexpand(reset, TRUE);
    g_signal_connect(reset, "clicked", G_CALLBACK(on_reset_clicked), ad); // reset button action

    // submit button
    GtkWidget *button = gtk_button_new_with_label("Submit");
    gtk_grid_attach(GTK_GRID(ad->grid), button, 4, 1, 1, 1); // location of button
    gtk_widget_set_halign(button, GTK_ALIGN_START);
    gtk_widget_set_hexpand(button, TRUE);

    // create header row
    GtkWidget *header_row = gtk_list_box_row_new();
    GtkWidget *header_grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(header_grid), TRUE);
    gtk_grid_set_column_spacing(GTK_GRID(header_grid), 10);

    // add each label to the header grid
    GtkWidget *name = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(name), "<span font='15' weight='bold'>NAME</span>");
    GtkWidget *weightclass = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(weightclass), "<span font='15' weight='bold'>WEIGHTCLASS</span>");
    GtkWidget *wins = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(wins), "<span font='15' weight='bold'>WINS</span>");
    GtkWidget *loses = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(loses), "<span font='15' weight='bold'>LOSES</span>");
    GtkWidget *age = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(age), "<span font='15' weight='bold'>AGE</span>");
    GtkWidget *country = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(country), "<span font='15' weight='bold'>COUNTRY</span>");

    gtk_grid_attach(GTK_GRID(header_grid), name, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(header_grid), weightclass, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(header_grid), wins, 2, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(header_grid), loses, 3, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(header_grid), age, 4, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(header_grid), country, 5, 0, 1, 1);

    gtk_container_add(GTK_CONTAINER(header_row), header_grid);

    // add header row to listbox
    gtk_list_box_insert(GTK_LIST_BOX(ad->listbox), header_row, 0);

    // guess method
    guess(ad, button);

    gtk_widget_show_all(ad->window);
    gtk_main();
    return 0;
}

//  cd "C:\CODING\UFC_GuessWho"
//  gcc main.c fighters.c -o run.exe `pkg-config --cflags --libs gtk+-3.0`
//  ./run.exe