/*
 ============================================================================
 Name        : DPViewergtk.c
 Author      : Tiago C. Teixeira
 Version     : 0.01
 Copyright   : Copyright © 2023, Tiago Teixeira
 Description : DPViewergtk in C, Ansi-style
 ============================================================================
 */

#include "gettext.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <gtk-3.0/gtk/gtk.h>
#include <gdk-pixbuf-2.0/gdk-pixbuf/gdk-pixbuf-core.h>
#include "controllers/dpcatcontroller.h"
#include "globals.h"
#include "models/base.h"
#include "views/messageWindow.h"
#include "messages.h"
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
#include "filepaths.h"

// Declare windows interface as global.
struct viewInterface* vi;
//char* _global_full_db_path;

/*
 * Initializes globals vars..
 */
void main_init_globals (void)
{
//	_global_full_db_path = filepaths_get_db_file_path_new (false);
	// Instanciates the window interface has a global variable
	vi = (struct viewInterface*)malloc(sizeof(*vi));
}

/*
 * Releases globals resources..
 */
void main_destroy_globals (void)
{
//	if (_global_full_db_path)
//		free (_global_full_db_path);
	free (vi);
}

/*
 * Executes all tests
 */
void main_test_all() {
	char* dbPath = filepaths_get_db_file_path_new (false);

	// display sqlite3 engine version
	char vstr[15];
	base_sqlite3_version (vstr);
	printf (_("Sqlite3 version: %s\n\n"), vstr);

	dpcatmodel_test (dbPath);		// Test model 'dpcatmodel'
	dpmodel_test (dbPath);		// Test model 'dpmodel'
	dpcodemodel_test (dbPath);	// Test model 'dpcodemodel'
	dppartmodel_test (dbPath);	// Test model 'dppartmodel
	dpucasemodel_test (dbPath);	// Test model 'dpucasemodel
}

/*
 * Event handler for application 'activate' signal.
 */
static void on_app_activate (GtkApplication* app, gpointer user_data)
{
	mainWindow_create_main_window (app, vi, filepaths_get_db_file_path_new (false));
}

/*
 * Prints command line options.
 */
static void main_print_options (void) {
	printf ("%s:\n    -h, --help\n", MESSAGES_OPTIONS_WORD_LOWER);
	printf (MESSAGES_DISPLAY_HELP_AND_EXIT);
	printf ("    -v, --version\n");
	printf (MESSAGES_DISPLAY_VERSION_AND_EXIT);
}

/*
 * App startup main function.
 */
int main (int argc, char **argv)
{
	char* dirName = filepaths_get_locale_dir_path_new (FALSE);
	bindtextdomain (APP_NAME, dirName);
	free (dirName);

	textdomain (APP_NAME);

	// Set the locale. The empty string "" means to use the user's default locale.
	setlocale (LC_ALL, "");

	/*
	 * Handle argument options (1º arg is command name path),
	 * second argument can be following options.
	 * Options:
	 * 		-v, --version
	 * 				Displays the version and exit.
	 * 		-h, --help
	 * 				Displays help and exit.
	 */
	if ((argc > 1) && (argc < 3)) {
		if ((strcmp (argv[1], "-h") == 0) || (strcmp (argv[1],"--help") == 0)) {
			// Display help info.
			printf ("%s: %s [%s]\n\n", MESSAGES_USAGE_WORD_CAP, APP_NAME, MESSAGES_OPTIONS_WORD_LOWER);
			main_print_options ();
			return 0;
		}
		else if ((strcmp (argv[1], "-v") == 0) || (strcmp (argv[1], "--version") == 0)) {
			// Display version.
			printf("%s\n", APP_VERSION);
			return 0;
		}
		else {
			// Unknown argument.
			printf (MESSAGES_UNKNOWN_ARG_ERROR, argv[1]);
			printf ("\n");
			main_print_options ( );
			return 1;
		}
	}
	else if (argc > 2) {
		printf ("%s\n", MESSAGES_INVALID_NUMBER_ARGS_ERROR);
		main_print_options ();
		return 2;
	}

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
