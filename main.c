#include <gtk/gtk.h>
#include <stdbool.h>

#define SIZE 3

typedef enum { EMPTY, CROSS, CIRCLE } CellContent;

typedef struct {
    GtkWidget *buttons[SIZE][SIZE];
    CellContent board[SIZE][SIZE];
    GtkWidget *label;
    bool is_cross_turn;
} GameData;

void apply_css(GtkWidget *widget, const char *css_file) {
    GtkCssProvider *provider = gtk_css_provider_new();
    GdkDisplay *display = gdk_display_get_default();
    GdkScreen *screen = gdk_display_get_default_screen(display);

    gtk_style_context_add_provider_for_screen(
        screen,
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER);

    gtk_css_provider_load_from_path(provider, css_file, NULL);
    g_object_unref(provider);
}

void reset_board(GameData *data) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            data->board[i][j] = EMPTY;
            gtk_button_set_label(GTK_BUTTON(data->buttons[i][j]), "");
            gtk_widget_set_sensitive(data->buttons[i][j], TRUE);
        }
    }
    data->is_cross_turn = true;
    gtk_label_set_text(GTK_LABEL(data->label), "Tour : Croix");
}

void show_popup(GtkWidget *parent, const char *message, GameData *data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Résultat du jeu",
        GTK_WINDOW(parent),
        GTK_DIALOG_MODAL,
        "Recommencer",
        GTK_RESPONSE_ACCEPT,
        "Quitter",
        GTK_RESPONSE_REJECT,
        NULL);

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *label = gtk_label_new(message);
    gtk_container_add(GTK_CONTAINER(content_area), label);
    gtk_widget_show_all(dialog);

    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_ACCEPT) {
        reset_board(data);
    } else {
        gtk_widget_destroy(parent);
    }
    gtk_widget_destroy(dialog);
}

bool is_winner(CellContent board[SIZE][SIZE], CellContent player) {
    for (int i = 0; i < SIZE; i++) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player) return true;
        if (board[0][i] == player && board[1][i] == player && board[2][i] == player) return true;
    }
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player) return true;
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player) return true;
    return false;
}

void check_game_status(GameData *data, GtkWidget *parent) {
    if (is_winner(data->board, CROSS)) {
        show_popup(parent, "Victoire : Croix !", data);
        return;
    }
    if (is_winner(data->board, CIRCLE)) {
        show_popup(parent, "Victoire : Cercle !", data);
        return;
    }

    bool is_draw = true;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (data->board[i][j] == EMPTY) {
                is_draw = false;
                break;
            }
        }
    }

    if (is_draw) {
        show_popup(parent, "Match nul !", data);
        return;
    }
}

void button_clicked(GtkWidget *button, gpointer user_data) {
    GameData *data = (GameData *)user_data;
    GtkWidget *parent = gtk_widget_get_toplevel(button);
    int row, col;

    for (row = 0; row < SIZE; row++) {
        for (col = 0; col < SIZE; col++) {
            if (data->buttons[row][col] == button) break;
        }
        if (col < SIZE) break;
    }

    if (data->board[row][col] != EMPTY) return;

    data->board[row][col] = data->is_cross_turn ? CROSS : CIRCLE;
    gtk_button_set_label(GTK_BUTTON(button), data->is_cross_turn ? "X" : "O");
    gtk_widget_set_sensitive(button, FALSE);

    check_game_status(data, parent);
    data->is_cross_turn = !data->is_cross_turn;

    if (gtk_label_get_text(GTK_LABEL(data->label))[0] != 'V' &&
        gtk_label_get_text(GTK_LABEL(data->label))[0] != 'M') {
        gtk_label_set_text(GTK_LABEL(data->label),
                           data->is_cross_turn ? "Tour : Croix" : "Tour : Cercle");
    }
}

void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Jeu de Morpion");
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    GameData *data = g_malloc(sizeof(GameData));

    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            data->buttons[i][j] = gtk_button_new();
            gtk_widget_set_size_request(data->buttons[i][j], 100, 100);

            // Ajouter une classe CSS uniquement pour les boutons du jeu
            GtkStyleContext *context = gtk_widget_get_style_context(data->buttons[i][j]);
            gtk_style_context_add_class(context, "game-button");

            g_signal_connect(data->buttons[i][j], "clicked", G_CALLBACK(button_clicked), data);
            gtk_grid_attach(GTK_GRID(grid), data->buttons[i][j], j, i, 1, 1);
        }
    }

    data->label = gtk_label_new("Tour : Croix");
    gtk_grid_attach(GTK_GRID(grid), data->label, 0, SIZE, SIZE, 1);

    reset_board(data);

    // Appliquer le CSS
    apply_css(GTK_WIDGET(window), "style.css");

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("com.example.morpion", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}