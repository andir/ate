#include <gtk/gtk.h>
#include <vte/vte.h>

static const gchar *colors[] = {"#282a2e", "#a54242", "#8c9440", "#de935f",
                                "#5f819d", "#85678f", "#5e8d87", "#707880",
                                "#373b41", "#cc6666", "#b5bd68", "#f0c674",
                                "#81a2be", "#b294bb", "#8abeb7", "#c5c8c6"};

const char *background_color = "#1d1f21";
const char *foreground_color = "#c5c8c6";

// static const keybinding* keybindings[] = {};
const gboolean enable_hyperlinks = TRUE;
const gboolean hide_mouse_cursor = TRUE;
const gboolean cursor_blink = FALSE;
const gint scrollback_size = 50000;

size_t get_color_pallete(GdkRGBA **gcolors_p) {
  const size_t colors_count = sizeof(colors) / sizeof(colors[0]);
  *gcolors_p = calloc(sizeof(GdkRGBA), colors_count);

  if (*gcolors_p == NULL) {
    fprintf(stderr, "failed to allocate memory for GdkRGBA");
    return -1;
  }

  for (size_t i = 0; i < colors_count; i++) {
    GdkRGBA rgba;
    fprintf(stdout, "[%li] Parsing: %s\n", i, colors[i]);
    if (!gdk_rgba_parse(&rgba, colors[i])) {
      fprintf(stderr, "failed to parse color: %s\n", colors[i]);
      return -1;
    }
    (*gcolors_p)[i] = rgba;
  }

  return colors_count;
}

void spawn_cb(VteTerminal *terminal, GPid pid, GError *error,
              gpointer user_data) {
  if (error != NULL && pid == -1) {
    g_printerr("failed to launch shell:%s\n", error->message);
    g_error_free(error);
    exit(1);
  } else {
    fprintf(stderr, "Shell launched: %i\n", pid);
  }
}

// whenever the terminal bell is triggered propagate the event to the window
void bell(VteTerminal *terminal, gpointer user_data) {
  GtkWidget *window = user_data;
  g_assert(terminal != NULL);
  g_assert(window != NULL);

  gtk_window_set_urgency_hint(GTK_WINDOW(window), TRUE);
}

// Update the appliations windows title to the new title of the current
// shell/process
void window_title_changed(VteTerminal *terminal, gpointer user_data) {
  GtkWidget *window = user_data;
  g_assert(terminal != NULL);
  g_assert(window != NULL);

  const char *title = vte_terminal_get_window_title(terminal);
  g_assert(title != NULL);

  gtk_window_set_title(GTK_WINDOW(window), title);
}

// Increase the font size on request
void increase_font_size(VteTerminal *terminal) {

  fprintf(stderr, "increase font size\n");

  fprintf(stderr, "terminal is %s\n", G_OBJECT_TYPE_NAME(terminal));

  g_assert(terminal != NULL);

  PangoFontDescription *desc =
      pango_font_description_copy(vte_terminal_get_font(terminal));
  g_assert(desc != NULL);

  gint size = pango_font_description_get_size(desc);

  fprintf(stderr, "font size: %i\n", size / PANGO_SCALE);

  pango_font_description_set_size(desc, size + PANGO_SCALE);
  vte_terminal_set_font(terminal, desc);
  pango_font_description_free(desc);
}

// Decrease the font size on request
void decrease_font_size(VteTerminal *terminal) {

  fprintf(stderr, "decrease font size\n");

  g_assert(terminal != NULL);

  PangoFontDescription *desc =
      pango_font_description_copy(vte_terminal_get_font(terminal));
  g_assert(desc != NULL);

  gint size = pango_font_description_get_size(desc);

  fprintf(stderr, "font size: %i\n", size / PANGO_SCALE);

  pango_font_description_set_size(desc, size - PANGO_SCALE);
  vte_terminal_set_font(terminal, desc);
  pango_font_description_free(desc);
}

// handlers of the resize signals
void signal_increase_font_size(VteTerminal *terminal, gpointer user_data) {
  increase_font_size(terminal);
}

void signal_decrease_font_size(VteTerminal *terminal, gpointer user_data) {
  decrease_font_size(terminal);
}

void accel_increase_font_size(GtkAccelGroup *accel_group,
                              GObject *acceleratable, guint keyval,
                              GdkModifierType modifier) {
  VteTerminal *terminal =
      g_object_get_data(G_OBJECT(acceleratable), "terminal");
  increase_font_size(terminal);
}

void accel_decrease_font_size(GtkAccelGroup *accel_group,
                              GObject *acceleratable, guint keyval,
                              GdkModifierType modifier) {
  VteTerminal *terminal =
      g_object_get_data(G_OBJECT(acceleratable), "terminal");
  decrease_font_size(terminal);
}


void accel_paste(GtkAccelGroup *accel_group,
		GObject* acceleratable, guint keyval,
		GdkModifierType modifier) {
  VteTerminal *terminal =
      g_object_get_data(G_OBJECT(acceleratable), "terminal");
  vte_terminal_paste_clipboard(terminal);
}

int main(int argc, char *argv[]) {
  GtkWidget *window, *terminal;

  GdkRGBA *colors = NULL;
  GdkRGBA color;
  const size_t colors_count = get_color_pallete(&colors);
  if (colors_count <= 0 || colors == NULL) {
    return 1;
  }

  /* Initialise GTK, the window and the terminal */
  gtk_init(&argc, &argv);
  terminal = vte_terminal_new();
  vte_terminal_set_colors(VTE_TERMINAL(terminal), NULL, NULL, colors,
                          colors_count);
  free(colors);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "at");

  gdk_rgba_parse(&color, foreground_color);
  vte_terminal_set_color_foreground(VTE_TERMINAL(terminal), &color);

  gdk_rgba_parse(&color, background_color);
  vte_terminal_set_color_background(VTE_TERMINAL(terminal), &color);

  vte_terminal_set_scrollback_lines(VTE_TERMINAL(terminal), scrollback_size);

  /* Enable Hyperlinks */
  if (enable_hyperlinks) {
    vte_terminal_set_allow_hyperlink(VTE_TERMINAL(terminal), TRUE);
  }

  /* Hide the mouse cursor after inactivity */
  if (hide_mouse_cursor) {
    vte_terminal_set_mouse_autohide(VTE_TERMINAL(terminal), TRUE);
  }

  /* Disable cursor blinking */
  if (!cursor_blink) {
    vte_terminal_set_cursor_blink_mode(VTE_TERMINAL(terminal), VTE_CURSOR_BLINK_OFF);
  }

  /* Connect some signals */
  g_signal_connect(window, "delete-event", gtk_main_quit, NULL);
  // g_signal_connect(window, "key_press_event", G_CALLBACK(on_key_press),
  // VTE_TERMINAL(terminal));
  g_signal_connect(terminal, "child-exited", gtk_main_quit, NULL);
  g_signal_connect(terminal, "window-title-changed",
                   G_CALLBACK(window_title_changed), GTK_WINDOW(window));
  g_signal_connect(terminal, "bell", G_CALLBACK(bell), GTK_WINDOW(window));
  g_signal_connect(terminal, "increase-font-size",
                   G_CALLBACK(signal_increase_font_size), GTK_WINDOW(window));
  g_signal_connect(terminal, "decrease-font-size",
                   G_CALLBACK(signal_decrease_font_size), GTK_WINDOW(window));

  /* hook the custom accelerators into the application */
  GtkAccelGroup *accelg = gtk_accel_group_new();

  /* ctrl+shift+(plus) | increment font size */
  gtk_accel_group_connect(accelg, /* group */
                          gdk_keyval_from_name("plus"),
                          GDK_CONTROL_MASK | GDK_SHIFT_MASK, /* key & mask */
                          GTK_ACCEL_LOCKED,                  /* flags */
                          g_cclosure_new(G_CALLBACK(accel_increase_font_size),
                                         terminal, NULL) /* callback */
  );

  /* ctrl+shift+(minus) | decrement font size */
  gtk_accel_group_connect(accelg, /* group */
                          gdk_keyval_from_name("minus"),
                          GDK_CONTROL_MASK | GDK_SHIFT_MASK, /* key & mask */
                          GTK_ACCEL_LOCKED,                  /* flags */
                          g_cclosure_new(G_CALLBACK(accel_decrease_font_size),
                                         terminal, NULL) /* callback */
  );
  gtk_accel_group_connect(accelg, /* group */
                          gdk_keyval_from_name("underscore"),
                          GDK_CONTROL_MASK | GDK_SHIFT_MASK, /* key & mask */
                          GTK_ACCEL_LOCKED,                  /* flags */
                          g_cclosure_new(G_CALLBACK(accel_decrease_font_size),
                                         terminal, NULL) /* callback */
  );

  /* alt+shift+y | paste */
  gtk_accel_group_connect(accelg, /* group */
		  	  gdk_keyval_from_name("y"),
			  GDK_MOD1_MASK | GDK_SHIFT_MASK, /* key & mask */
			  GTK_ACCEL_LOCKED,		  /* flags */
			  g_cclosure_new(G_CALLBACK(accel_paste),
				  	 terminal, NULL) /* callback */
  );

  gtk_window_add_accel_group(GTK_WINDOW(window), accelg);

  // attach the terminal as private data to the window
  g_object_set_data(G_OBJECT(window), "terminal", terminal);

  /* Start a new shell */
  gchar **envp = g_get_environ();
  gchar **command =
      (gchar *[]){g_strdup(g_environ_getenv(envp, "SHELL")), NULL};

  if (argc > 1) {
    command = &argv[1];
  }

  fprintf(stderr, "launch command: %s\n", command[0]);
  g_strfreev(envp);
  vte_terminal_spawn_async(VTE_TERMINAL(terminal), VTE_PTY_DEFAULT,
                           NULL,            /* working directory  */
                           command,         /* command */
                           NULL,            /* environment */
                           G_SPAWN_DEFAULT, /* spawn flags */
                           NULL, NULL,      /* child setup */
                           NULL,            /* child pid */
                           -1,              /* timeout */
                           NULL,            /* cancellable */
                           spawn_cb,        /* callback */
                           NULL);           /* user_data */

  /* Put widgets together and run the main loop */
  gtk_container_add(GTK_CONTAINER(window), terminal);
  gtk_widget_show_all(window);
  gtk_main();
}
