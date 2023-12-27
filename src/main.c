/*
 ============================================================================
 Name        : DPViewergtk.c
 Author      : Tiago C. Teixeira
 Version     :
 Copyright   : Copyright © 2023, Tiago Teixeira
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include "gettext.h"
#include <stdio.h>
#include <stdlib.h>
#include <gtk-3.0/gtk/gtk.h>
#include <gdk-pixbuf-2.0/gdk-pixbuf/gdk-pixbuf-core.h>

//#include <gtksourceview/gtksourceview.h>
//#include <gtksourceview/gtksourcebuffer.h>
//#include <gtksourceview/gtksourcelanguage.h>
//#include <gtksourceview/gtksourcelanguagemanager.h>


#include <gtksourceview/gtksource.h>
////#include <gtksourceview/gtksourcebuffer.h>
#include <gtksourceview/gtksourcelanguage.h>
#include <gtksourceview/gtksourcelanguagemanager.h>

#include "controllers/dpcatcontroller.h"
#include "globals.h"
#include "models/base.h"
#include "views/messageWindow.h"
//#include "globals.h"
#include "models/dpcatmodel.h"
#include "models/dpmodel.h"
#include "models/dpcodemodel.h"
#include "models/dppartmodel.h"
#include "models/dpucasemodel.h"


#include "controllers/dpcodecontroller.h"
#include "controllers/dppartcontroller.h"
#include "controllers/dpucasescontroller.h"


#include "viewinterface.h"
#include "views/mainWindow.h"

//// translation libs
//#include <libintl.h>
//#include <locale.h>

//// Macro for standard mark of text to be translated _()
//// This is a common way to mark localized strings.
//#define _(String) gettext(String)



//#include <gtk-3.0/gtk/gtkpopovermenu.h>
//#include <gdk/gdk.h>

//char* _global_full_db_path = NULL;

//#define SOURCE_LANGUAGE_C "c"
//#define SOURCE_LANGUAGE_JAVA "java"

// Declare windows interface as global.
struct viewInterface* wi;
char* _global_full_db_path;
//GtkWidget *popover;

/*
 * Initializes globals vars..
 */
void main_init_globals (void)
{
	_global_full_db_path = globals_get_db_full_path_new ();

	// Instanciates the window interface has a global variable
	wi = (struct viewInterface*)malloc(sizeof(*wi));
}

/*
 * Releases globals resources..
 */
void main_destroy_globals (void)
{
	if (_global_full_db_path)
		free (_global_full_db_path);
}

/*
 * Executes all tests
 */
void main_test_all() {
	// display sqlite3 engine version
	char vstr[15];
	base_sqlite3_version(vstr);
	printf (_("Sqlite3 version: %s\n\n"), vstr);

	dpcatmodel_test (_global_full_db_path);		// Test model 'dpcatmodel'
	dpmodel_test (_global_full_db_path);		// Test model 'dpmodel'
	dpcodemodel_test (_global_full_db_path);	// Test model 'dpcodemodel'
	dppartmodel_test (_global_full_db_path);	// Test model 'dppartmodel
	dpucasemodel_test (_global_full_db_path);	// Test model 'dpucasemodel
}

/*
 * Event handler for application 'activate' signal.
 */
static void on_app_activate (GtkApplication* app, gpointer user_data)
{
	mainWindow_create_main_window (app, wi, _global_full_db_path);
}

/*
 * App startup main function.
 */
int main (int argc, char **argv)
{
	// Specify directory which contains the messages catalogs
	bindtextdomain("base", "./locales/");
	textdomain ("base");

	// Initialize global vars
	main_init_globals();

	// Gtk application start sequence --------------
	// When using GtkApplication, it is not necessary to call gtk_init() manually.
	// It is called as soon as the application gets registered as the primary
	// instance.
	GtkApplication *app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect (app, "activate", G_CALLBACK (on_app_activate), NULL);
	int status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);
	//--------------------

	// Releases global vars
	main_destroy_globals();
	return status;
}

//
//
//void print_hello() {
//	puts("Hello world!");
//}
//
//static void quit_cb (GtkWindow *window)
//{
//  gtk_window_close (window);
//}
//
//static void activate (GtkApplication *app, gpointer user_data)
//{
//  /* Construct a GtkBuilder instance and load our UI description */
//  GtkBuilder *builder = gtk_builder_new ();
//  gtk_builder_add_from_file (builder, "builder.ui", NULL);
//
//  /* Connect signal handlers to the constructed widgets. */
//  GObject *window = gtk_builder_get_object (builder, "window");
//  gtk_window_set_application (GTK_WINDOW (window), app);
//
//  GObject *button = gtk_builder_get_object (builder, "button1");
//  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
//
//  button = gtk_builder_get_object (builder, "button2");
//  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
//
//  button = gtk_builder_get_object (builder, "quit");
//  g_signal_connect_swapped (button, "clicked", G_CALLBACK (quit_cb), window);
//
//  gtk_widget_set_visible (GTK_WIDGET (window), TRUE);
//
//  /* We do not need the builder any more */
//  g_object_unref (builder);
//}
//
//int main (int argc, char *argv[])
//{
//#ifdef GTK_SRCDIR
//  g_chdir (GTK_SRCDIR);
//#endif
//
//  GtkApplication *app = gtk_application_new ("Design Patterns", G_APPLICATION_DEFAULT_FLAGS);
//  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
//
//  int status = g_application_run (G_APPLICATION (app), argc, argv);
//  g_object_unref (app);
//
//  return status;
//}
//
//
