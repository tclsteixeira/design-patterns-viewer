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
#include <gtk-3.0/gtk/gtk.h>
#include <gdk-pixbuf-2.0/gdk-pixbuf/gdk-pixbuf-core.h>
#include <gtksourceview/gtksource.h>
#include <gtksourceview/gtksourcelanguage.h>
#include <gtksourceview/gtksourcelanguagemanager.h>
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
char* _global_full_db_path;

/*
 * Initializes globals vars..
 */
void main_init_globals (void)
{
	_global_full_db_path = filepaths_get_db_file_path_new (false);
	// Instanciates the window interface has a global variable
	vi = (struct viewInterface*)malloc(sizeof(*vi));
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
	mainWindow_create_main_window (app, vi, _global_full_db_path);
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
#ifdef DEBUG_MODE
	char dirName[strlen ("..") + strlen (FILE_PATHS_LOCALE) + 1];
	dirName[0] = 0;
	strcat (dirName, "..");
	strcat (dirName, FILE_PATHS_LOCALE);
	dirName[strlen (dirName)] = '\0';

	// Specify directory which contains the messages catalogs
	bindtextdomain (APP_NAME, dirName);	// "./locale/"
	textdomain (APP_NAME);
#else
	// Specify directory which contains the messages catalogs
	bindtextdomain (APP_NAME, FILE_PATHS_DISTR_LOCALE_BASE);	// "/usr/share/locale"
	textdomain (APP_NAME);
#endif

	// Set the locale. The empty string "" means to use the user's default locale.
	setlocale (LC_ALL, "");

	// Handle argument options (1º arg is command name 'DPViewergtk')
	if ((argc > 1) && (argc < 3)) {
		if ((strcmp (argv[1], "-h") == 0) || (strcmp (argv[1],"--help") == 0)) {
			printf ("%s: ./%s [%s]\n\n", MESSAGES_USAGE_WORD_CAP, APP_NAME, MESSAGES_OPTIONS_WORD_LOWER);
			main_print_options ();
			return 0;
		}
		else if ((strcmp (argv[1], "-v") == 0) || (strcmp (argv[1], "--version") == 0)) {
			printf("%s\n", APP_VERSION);
			return 0;
		}
		else {
			main_print_options( );
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
