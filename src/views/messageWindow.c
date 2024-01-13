/*
 * messageWindow.c
 *
 *  Created on: 20/11/2023
 *      Author: Tiago C. Teixeira
 * Description: Utility functions to simplify displaying simple message windows
 * 				like error messages.
 */

#include <stdlib.h>
#include <gtk-3.0/gtk/gtk.h>



/*
 * Displays an error message dialog.
 */
void messageWindow_show_error (GtkWindow* parentWindow, const char* title, const char* message)
{
	GtkWidget *dialog;

	// create dialog window
	dialog = gtk_message_dialog_new (GTK_WINDOW (parentWindow),
	                                 GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
	                                 GTK_MESSAGE_ERROR,
	                                 GTK_BUTTONS_OK,
	                                 title);

	if (message)
		// setup meaage
		gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG (dialog),
	                                          	  message, NULL);

	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);
}

/*
 * Displays an info message dialog.
 */
void messageWindow_show_info (GtkWindow* parentWindow, const char* title, const char* message)
{
	GtkWidget *dialog;

	// create dialog window
	dialog = gtk_message_dialog_new (GTK_WINDOW (parentWindow),
	                                 GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
	                                 GTK_MESSAGE_INFO,
	                                 GTK_BUTTONS_OK,
	                                 title);

	if (message)
		// setup meaage
		gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG (dialog),
	                                          	  message, NULL);

	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy(dialog);
}


