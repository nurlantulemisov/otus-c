#include <getopt.h>
#include <glib/gstdio.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

enum { TITLE_COLUMN, N_COLUMNS };

static void render_tree_view(GtkTreeStore *store, const char *path,
                             GtkTreeIter *root, int **error) {

  GDir *dir = g_dir_open(path, 0, NULL);
  if (dir != NULL) {
    perror("failed open dir");
    int status = EXIT_FAILURE;
    *error = &status;

    return;
  }

  const gchar *dir_name;
  while ((dir_name = g_dir_read_name(dir)) != NULL) {
    GtkTreeIter iter;
    gtk_tree_store_append(store, &iter, root);
    gtk_tree_store_set(store, &iter, TITLE_COLUMN, dir_name, -1);

    struct stat st;

    char buf[100];
    snprintf(buf, sizeof(buf), "%s/%s", path, dir_name);

    if (g_stat(buf, &st) == -1)
      return;

    if (S_ISDIR(st.st_mode)) {
      render_tree_view(store, buf, &iter, error);
    }
  }
  g_dir_close(dir);
}

static void activate(GtkApplication *app, gpointer user_data) {
  const char *path = (char *)user_data;

  GtkTreeStore *store =
      gtk_tree_store_new(N_COLUMNS, G_TYPE_STRING, G_TYPE_STRING);

  int *error = NULL;
  render_tree_view(store, path, NULL, &error);
  if (error != NULL) {
    printf("error render tree");
    return;
  }

  GtkWidget *tree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));

  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("Dir list", renderer,
                                                    "text", TITLE_COLUMN, NULL);

  gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

  GtkWidget *window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Window");
  gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);

  gtk_window_set_child(GTK_WINDOW(window), tree);

  gtk_window_present(GTK_WINDOW(window));
}

void print_help(void) {
  printf("Usage: ./myprog -p <path>\n");
  printf("Options:\n");
  printf("  -p <path>  path\n");
  printf("  -h         Print this help message\n");
}

int main(int argc, char **argv) {
  char *path = NULL;

  int c;
  while ((c = getopt(argc, argv, "p:h")) != -1) {
    switch (c) {
    case 'p':
      path = optarg;
      break;
    case 'h':
      print_help();
      return 0;
    default:
      printf("Invalid option: %c\n", c);
      return 1;
    }
  }

  if (path == NULL) {
    perror("Missing required options: -p\n");
    return EXIT_FAILURE;
  }

  GtkApplication *app =
      gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), path);
  int status = g_application_run(G_APPLICATION(app), 0, NULL);
  g_object_unref(app);

  return status;
}
