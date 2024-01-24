/*
 * vi.c
 *
 *  Created on: 25/11/2023
 *      Author: Tiago C. Teixeira
 * Description: Implements the window user interface.
 */

#include "gettext.h"
#include <gtk-3.0/gtk/gtk.h>
#include <gtksourceview-3.0/gtksourceview/gtksource.h>
#include <gtksourceview-3.0/gtksourceview/gtksourceview.h>
#include <gtksourceview-3.0/gtksourceview/gtksourcebuffer.h>
#include <gtksourceview-3.0/gtksourceview/gtksourcelanguagemanager.h>
#include <pango-1.0/pango/pango-attributes.h>
#include <pango-1.0/pango/pango.h>
#include <pango-1.0/pango/pango-enum-types.h>
#include <gdk/gdk.h>
#include "viewinterface.h"

#define WI_PANGO_BOLD 700
#define WI_PANGO_ITALIC "italic"

#define WI_TEXT_BUFFER_TAG_ITALIC "italic"
#define WI_TEXT_BUFFER_TAG_BOLD "bold"


typedef enum {
	TAGS_ENUM_ITALIC = 0,
	TAGS_ENUM_BOLD = 1
} enumTextViewTags;


//typedef enum {
//  PANGO_WEIGHT_THIN = 100,
//  PANGO_WEIGHT_ULTRALIGHT = 200,
//  PANGO_WEIGHT_LIGHT = 300,
//  PANGO_WEIGHT_BOOK = 380,
//  PANGO_WEIGHT_NORMAL = 400,
//  PANGO_WEIGHT_MEDIUM = 500,
//  PANGO_WEIGHT_SEMIBOLD = 600,
//  PANGO_WEIGHT_BOLD = 700,
//  PANGO_WEIGHT_ULTRABOLD = 800,
//  PANGO_WEIGHT_HEAVY = 900,
//} PangoWeightext;

/*
 * Gets the name of the current theme.
 * result should be later free when no longer needed.
 */
char* vi_alloc_current_theme_name () {
	char* result = NULL;
	GtkSettings *settings = gtk_settings_get_default();
	// Get the current GTK theme name
	const gchar *currentTheme = NULL;
	g_object_get (settings, VI_THEME_SETTING_NAME, &currentTheme, NULL);
	if (currentTheme)
		result = strdup (currentTheme);

	return result;
}

/*
 * Sets the ui theme (system, dark, light).
 */
void vi_set_theme (struct viewInterface* wi, enum viThemeOptions themeOp)
{
	GtkSettings *settings = gtk_settings_get_default();

	switch (themeOp) {
		case VI_SYSTEM_THEME:	// System theme
			if (wi->sysThemeName)
				g_object_set(settings, VI_THEME_SETTING_NAME, wi->sysThemeName, NULL);
			else {
				const char* msg = "System theme name is undefined (NULL).\n";
				fprintf(stderr, msg);
				printf (msg);
			}
			break;
		case VI_DARK_THEME:		// Dark mode
			g_object_set(settings, VI_THEME_SETTING_NAME, VI_DARK_THEME_NAME, NULL);
			break;
		case VI_LIGHT_THEME:	// Light mode
			g_object_set(settings, VI_THEME_SETTING_NAME, VI_LIGHT_THEME_NAME, NULL);
			break;
		default:
			fprintf(stderr, "Invalid function argument 'themeOp'.\n");
			break;
	}
}

/*
 * Sets the source language for the GtkSourceView.
 */
void vi_set_source_lang (GtkSourceView* sView, char* langName) {
	// Get the GtkSourceBuffer from the GtkSourceView
	GtkTextBuffer *text_buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (sView));
	GtkSourceBuffer *buff = GTK_SOURCE_BUFFER (text_buffer);
	GtkSourceLanguageManager* lm = gtk_source_language_manager_get_default ();
	GtkSourceLanguage* lang = gtk_source_language_manager_get_language (lm, langName);
	gtk_source_buffer_set_language (GTK_SOURCE_BUFFER (buff), lang);
}

/*
 * Sets cursor at start position of a given text buffer.
 */
void vi_set_text_buffer_cursor_at_start (GtkTextBuffer* buff) {
	GtkTextIter iter;
	gtk_text_buffer_get_start_iter (GTK_TEXT_BUFFER (buff), &iter);
	gtk_text_buffer_place_cursor (GTK_TEXT_BUFFER (buff), &iter);
}

/*
 * Sets the content of a given source view.
 */
void vi_set_source_view_content (GtkSourceView* sv, char* content)
{
	GtkSourceBuffer* sBuff = (GtkSourceBuffer*)gtk_text_view_get_buffer (GTK_TEXT_VIEW (sv));
	gtk_text_buffer_set_text ( GTK_TEXT_BUFFER(sBuff), content, -1);
	vi_set_text_buffer_cursor_at_start (GTK_TEXT_BUFFER (sBuff));
}

/*
 * Sets the content of a given text view.
 */
void vi_set_text_view_content (GtkTextView* tv, const char* content, gboolean isMarkup)
{
	GtkTextBuffer* tvBuff = gtk_text_view_get_buffer (GTK_TEXT_VIEW (tv));
	if (isMarkup) {
		gtk_text_buffer_set_text (tvBuff, "", -1);	// Clean buffer
		GtkTextIter iter;
		gtk_text_buffer_get_start_iter (tvBuff, &iter);
		gtk_text_buffer_insert_markup (tvBuff, &iter, content, -1);
	}
	else
		gtk_text_buffer_set_text (tvBuff, content, -1);

	// Put cursor at start
	vi_set_text_buffer_cursor_at_start (tvBuff);
}

/*
 * Appends text or markup to the given text view.
 */
void vi_text_view_append_text (GtkTextView *textView, const char *text, gboolean isMarkup) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer (textView);

    GtkTextIter iter;
    gtk_text_buffer_get_end_iter (buffer, &iter);

    if (isMarkup)
    	gtk_text_buffer_insert_markup (buffer, &iter, text, -1);
    else
    	gtk_text_buffer_insert (buffer, &iter, text, -1);
}

/*
 * Sets the label text with bold attribute.
 */
void vi_set_label_bold (GtkLabel* label) {
    PangoAttrList *attr_list = pango_attr_list_new();
    PangoAttribute *attr = pango_attr_weight_new ( WI_PANGO_BOLD );

    // Add the attribute to the attribute list
    pango_attr_list_insert(attr_list, attr);

    // Apply the attribute list to the label
    gtk_label_set_attributes (GTK_LABEL (label), attr_list);

    // Free the attribute list
    pango_attr_list_unref(attr_list);
}

/*
 * Sets the expander title text bold.
 */
void vi_set_expander_title_bold (GtkExpander* expander) {
	vi_set_label_bold (GTK_LABEL (gtk_expander_get_label_widget (expander)));
}

/*
 * Sets the window size based on screen percentages.
 */
void vi_set_window_size_perc (GtkWindow* window, uint widthPerc, uint heightPerc) {

	GdkRectangle workarea = {0};
	gdk_monitor_get_workarea(
	gdk_display_get_primary_monitor (gdk_display_get_default()),
									 &workarea);

	int scaleFactor = 1;

	// Calculate the desired window size based on 50 percentage
	gint desired_width = scaleFactor * ((50 * workarea.width) / 100);
	gint desired_height = scaleFactor * ((50 * workarea.height) / 100);

	// Set the window size
	gtk_widget_set_size_request (GTK_WIDGET (window), desired_width, desired_height);

	// Recalculate the desired window size based on the percentage
	desired_width = scaleFactor * ((widthPerc * workarea.width) / 100);
	desired_height = scaleFactor * ((heightPerc * workarea.height) / 100);
	gtk_window_set_default_size (window, desired_width, desired_height);
	// Set the window size
//	gtk_widget_set_size_request (GTK_WIDGET (window), desired_width, desired_height);

	gtk_window_resize (window, desired_width, desired_height);
}

/*
 * Retrieves the screen resolution size for the default display.
 */
void vi_get_screen_resolution (int* width, int* height) {
	GdkRectangle workarea = {0};
	gdk_monitor_get_workarea (gdk_display_get_primary_monitor (gdk_display_get_default()),
								&workarea);
	*width = workarea.width;
	*height = workarea.height;
}
