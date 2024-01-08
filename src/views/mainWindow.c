/*
 * mainWindow.c
 *
 *  Created on: 22/12/2023
 *      Author: Tiago C. Teixeira
 * Description: Implements functions to create the views main window and its widgets.
 */

#include "stdlib.h"
#include "../globals.h"
#include "gtk/gtk.h"
#include "../viewinterface.h"
#include "../models/dpmodel.h"

#include "../controllers/dpcatcontroller.h"
#include "../controllers/dpcodecontroller.h"
#include "../controllers/dppartcontroller.h"
#include "../controllers/dpucasescontroller.h"

#include "messageWindow.h"
#include "../messages.h"
#include "../filepaths.h"
#include "../gettext.h"
//#include <pangocairo.h>

//// Function to get the line height of a PangoLayout
//int __get_line_height (PangoLayout *layout) {
//    PangoFontMetrics *metrics = pango_context_get_metrics (pango_layout_get_context (layout), pango_layout_get_font_description(layout), NULL);
//    int height = pango_font_metrics_get_ascent(metrics) + pango_font_metrics_get_descent (metrics);
//    pango_font_metrics_unref (metrics);
//    return height;
//}
//

//#define APP_LOGO_REL_PATH FILE_PATHS_IMAGES ## PATH_SEPARATOR ## APP_LOGO

/*
 * Event handler for about button clicked
 * Shows an about dialog with author, app name, version, copyright, license, etc.
 */
static void on_aboutButton_clicked (GtkWidget *button, GtkWidget *menu) {
	GtkAboutDialog* aboutDlg = (GtkAboutDialog*)gtk_about_dialog_new();

	char* copyrightNotice = globals_get_app_copyright_new ();
	gtk_about_dialog_set_copyright (aboutDlg, copyrightNotice);
	free (copyrightNotice);

	const gchar** authors = malloc (strlen(APP_AUTHOR) * sizeof(gchar));
	authors[0] = APP_AUTHOR;
	gtk_about_dialog_set_authors (aboutDlg, authors);

	gtk_about_dialog_set_program_name (aboutDlg, APP_TITLE);

	char* comments = globals_get_app_comments_new ();
	gtk_about_dialog_set_comments (aboutDlg, comments);
	free (comments);

	gtk_about_dialog_set_version (aboutDlg, APP_VERSION);
	gtk_about_dialog_set_license_type (aboutDlg, GTK_LICENSE_GPL_3_0);

	// Logo is an svg image format
	// Adjust size based on screen resolution.
	int w = 1; int h = 1;
	vi_get_screen_resolution (&w, &h);
	w = (w < h) ? w : h;
	if (w > 1080) w = 300; else w = 100;

	GError** err = NULL;
	char* logoRelPath = filepaths_get_logo_image_file_path_new (false);
	GdkPixbuf *about_logo_pixbuf =
				gdk_pixbuf_new_from_file_at_size (logoRelPath, w, w,
								err);

	if (!about_logo_pixbuf) {
		printf("Failed to get logo image file in '%s'", logoRelPath);
		if (err)
			printf("%s\n", (*err)->message);
	}
	else {
		gtk_about_dialog_set_logo (aboutDlg, about_logo_pixbuf);
		g_object_unref (about_logo_pixbuf); about_logo_pixbuf= NULL;
	}

	// Open modal window
	gtk_dialog_run (GTK_DIALOG (aboutDlg));
	gtk_widget_destroy (GTK_WIDGET (aboutDlg));	// Must release window after
	free (authors);
}

/*
 * Creates the theme style selector dropdown.
 * The dropdown list let's you select system/drak/ligth theme for the app.
 */
static GtkWidget* main_create_dropdown_theme ()
{
	enum {
		DROP_THEME_NAME_COLUMN,
		DROP_THEME_CHECK_COLUMN,
		DROP_THEME_NUM_COLUMNS
	};
	const char* sysThemeName = MESSAGES_THEME_NAME_SYSTEM;
	const char* darkThemeName = MESSAGES_THEME_NAME_DARK;
	const char* lightThemeName = MESSAGES_THEME_NAME_LIGHT;

	const char * const themeOption[] = { sysThemeName, darkThemeName, lightThemeName, NULL };

	// Create a list store for the combo box
	GtkListStore *store = gtk_list_store_new (DROP_THEME_NUM_COLUMNS, G_TYPE_STRING, G_TYPE_BOOLEAN);

	// Add items to the list store
	GtkTreeIter iter;
	gtk_list_store_append (store, &iter);
	gtk_list_store_set 	  (store, &iter, DROP_THEME_NAME_COLUMN, themeOption[0], DROP_THEME_CHECK_COLUMN, TRUE, -1);
	gtk_list_store_append (store, &iter);
	gtk_list_store_set	  (store, &iter, DROP_THEME_NAME_COLUMN, themeOption[1], DROP_THEME_CHECK_COLUMN, FALSE, -1);
	gtk_list_store_append (store, &iter);
	gtk_list_store_set	  (store, &iter, DROP_THEME_NAME_COLUMN, themeOption[2], DROP_THEME_CHECK_COLUMN, FALSE, -1);

	// Create the combo box and set the model
	GtkWidget *combobox = gtk_combo_box_new_with_model (GTK_TREE_MODEL (store));
	g_object_unref(store);

	// Create a cell renderer for the combo box
	GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (combobox), renderer, TRUE);
	gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (combobox), renderer, "text", 0, NULL);

	// Create a toggle renderer for the combo box
	GtkCellRenderer *toggleRenderer = gtk_cell_renderer_toggle_new();
	gtk_cell_renderer_toggle_set_radio (GTK_CELL_RENDERER_TOGGLE (toggleRenderer), FALSE);
	gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (combobox), toggleRenderer, TRUE);
	gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (combobox), toggleRenderer, "active", 1, NULL);

	return combobox;
}

/*
 * Creates the header bar about button.
 */
static GtkWidget* main_create_about_button ()
{
	// Create the about button
	GtkWidget *button = gtk_button_new_with_label (MESSAGES_ABOUT);
	return button;
}

/*
 * Callback function to handle the combobox item selection
 */
static void on_dropDownTheme_changed (GtkWidget *widget, gpointer data) {
    GtkComboBox *combobox = GTK_COMBO_BOX (widget);
    gint active_item = gtk_combo_box_get_active (combobox);

    GtkTreeModel* model = gtk_combo_box_get_model (combobox);
    GtkTreeIter iter;

    if (gtk_tree_model_get_iter_first (model, &iter)) {
		do {
			gtk_list_store_set(GTK_LIST_STORE(model), &iter, 1, FALSE, -1);
		} while (gtk_tree_model_iter_next(model, &iter));
	}

    // Set check mark for the selected item
    if (active_item >= 0) {
        GtkTreeIter iter;
        if (gtk_combo_box_get_active_iter (combobox, &iter)) {
            gtk_list_store_set (GTK_LIST_STORE( gtk_combo_box_get_model (combobox)), &iter, 1, TRUE, -1);
        }

        // Set app theme for current selected item
        vi_set_theme (data, active_item);
    }
}

/*
 * Creates the main window header bar.
 */
static GtkWidget* main_create_header_bar (struct viewInterface* vi)
{
	// Create window header bar
	GtkWidget* header = gtk_header_bar_new ();
	gtk_header_bar_set_show_close_button ( GTK_HEADER_BAR (header), TRUE);
	gtk_header_bar_set_title (GTK_HEADER_BAR (header), APP_TITLE);

	GtkWidget *aboutButton = main_create_about_button ();
	g_signal_connect (aboutButton, "clicked", G_CALLBACK (on_aboutButton_clicked), NULL);

	// Create theme selector dropdown
	// Dropdown label
	GtkWidget* dropDownThemelabel = gtk_label_new (MESSAGES_THEME_AND_SPACE);
	gtk_widget_set_margin_start (dropDownThemelabel, 8);

	GtkWidget* dropDownTheme = main_create_dropdown_theme ();
	g_signal_connect (dropDownTheme, "changed", G_CALLBACK (on_dropDownTheme_changed), vi);

	// Select first item (System theme)
	gtk_combo_box_set_active (GTK_COMBO_BOX (dropDownTheme), 0);

	// Create vertical box for dropdown
	GtkWidget *themeDropDownBox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 10);
	gtk_box_pack_start (GTK_BOX (themeDropDownBox), dropDownTheme, FALSE, FALSE, 0);

	gtk_header_bar_pack_start (GTK_HEADER_BAR (header), GTK_WIDGET (dropDownThemelabel));
	gtk_header_bar_pack_start (GTK_HEADER_BAR (header), GTK_WIDGET (themeDropDownBox));
	gtk_header_bar_pack_end (GTK_HEADER_BAR (header), GTK_WIDGET (aboutButton));

	return header;
}

/*
 * Creates the categories tree view.
 */
static GtkWidget* main_create_tree_view ()
{
	const int tView1RightMargin = 5;
	// ------- Categories tree view --------------
	GtkWidget* tView1 = gtk_tree_view_new();
	gtk_widget_set_can_focus (tView1, TRUE);
	gtk_widget_set_vexpand (tView1, TRUE);
	gtk_widget_set_hexpand (tView1, TRUE);
	gtk_widget_set_margin_end (GTK_WIDGET (tView1), tView1RightMargin);
	gtk_widget_set_size_request (tView1, 150, 150);
	return tView1;
}

/*
 * Creates the source view widget to display sample code examples.
 */
static GtkWidget* main_create_source_view ()
{
	const int sView1RightMargin = 5;
	// Create a new GtkSourceBuffer
	GtkTextTagTable *tagTable = gtk_text_tag_table_new ();
	GtkSourceBuffer	 *sBuffer = gtk_source_buffer_new ( GTK_TEXT_TAG_TABLE (tagTable));
	gtk_source_buffer_set_highlight_syntax (GTK_SOURCE_BUFFER(sBuffer), TRUE);

	// Create a GtkSourceView
	GtkWidget* sView1 = gtk_source_view_new_with_buffer (sBuffer);

	// set a language manager and set a language
	vi_set_source_lang (GTK_SOURCE_VIEW (sView1), SOURCE_LANGUAGE_JAVA);

	// Set a style scheme (you can replace 'classic' with another style scheme)
	GtkSourceStyleSchemeManager *styleManager = gtk_source_style_scheme_manager_get_default();
	GtkSourceStyleScheme *style_scheme = gtk_source_style_scheme_manager_get_scheme (styleManager, "classic");
	gtk_source_buffer_set_style_scheme (sBuffer, style_scheme);
	gtk_source_view_set_show_line_numbers (GTK_SOURCE_VIEW (sView1), TRUE);
	gtk_widget_set_margin_end (GTK_WIDGET (sView1), sView1RightMargin);

	// Code is not editable .
	gtk_text_view_set_editable (GTK_TEXT_VIEW (sView1), FALSE);
	return sView1;
}

/*
 * Creates the design pattern/category description expander.
 */
static GtkWidget* main_create_dpdesc_expander ()
{
	const int expanderTopMargin = 2;
	const int expanderLeftMargin = 5;
	const int expanderBottomMargin = 0;

	// Create an expander
	GtkWidget *expander = gtk_expander_new (MESSAGES_DESCRIPTION_WORD);
	gtk_widget_set_vexpand (GTK_WIDGET (expander), FALSE);
	gtk_widget_set_hexpand (GTK_WIDGET (expander), TRUE);
	gtk_widget_set_valign (GTK_WIDGET (expander), GTK_ALIGN_FILL);
	gtk_widget_set_halign (GTK_WIDGET (expander), GTK_ALIGN_FILL);
	gtk_widget_set_margin_start ( GTK_WIDGET (expander), expanderLeftMargin);
	gtk_widget_set_margin_top ( GTK_WIDGET (expander), expanderTopMargin);
	gtk_widget_set_margin_bottom ( GTK_WIDGET (expander), expanderBottomMargin);

	gtk_expander_set_expanded (GTK_EXPANDER (expander), TRUE);

	// Set expander bold
	vi_set_expander_title_bold (GTK_EXPANDER(expander));
	return expander;
}

/*
 * Creates the design pattern/category description text view.
 * Note: This text view will be nested inside the dpdesc expander.
 */
static GtkWidget* main_create_dpdesc_text_view ()
{
	const int descTextViewRightMargin = 5;
	const int descTextViewLeftMargin = 8;
	const int descTextViewTopMargin = 8;

	// Create a text buffer and view
	GtkTextBuffer *descBuffer = gtk_text_buffer_new (NULL);
	GtkWidget *descTextView = gtk_text_view_new_with_buffer (descBuffer);
	gtk_widget_set_halign (GTK_WIDGET (descTextView), GTK_ALIGN_FILL);
	gtk_widget_set_valign (GTK_WIDGET (descTextView), GTK_ALIGN_START);
	gtk_widget_set_vexpand (GTK_WIDGET (descTextView), FALSE);
	gtk_widget_set_hexpand (GTK_WIDGET (descTextView), TRUE);
	gtk_widget_set_margin_end (GTK_WIDGET (descTextView), descTextViewRightMargin);
	gtk_widget_set_margin_start ( GTK_WIDGET (descTextView), descTextViewLeftMargin);
	gtk_widget_set_margin_top ( GTK_WIDGET (descTextView), descTextViewTopMargin);
	gtk_text_view_set_editable (GTK_TEXT_VIEW (descTextView), FALSE);
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (descTextView), GTK_WRAP_WORD);

	return descTextView;
}

/*
 * Creates the design pattern usage bar.
 * Note: This bar measures the frequency of usage of a given design pattern,
 * in a scale from 1 to 5.
 */
static GtkWidget* main_create_usage_bar ()
{
	GtkWidget* usageBar = gtk_level_bar_new();
	gtk_level_bar_set_min_value (GTK_LEVEL_BAR (usageBar), 0);
	gtk_level_bar_set_max_value (GTK_LEVEL_BAR (usageBar), 5);
	gtk_level_bar_set_mode (GTK_LEVEL_BAR (usageBar), GTK_LEVEL_BAR_MODE_DISCRETE);
	return usageBar;
}

/*
 * Creates the design pattern participants text view.
 */
static GtkWidget* main_create_participants_text_view ()
{
	// Create text view for participants content
	GtkTextBuffer* partBuffer = gtk_text_buffer_new (NULL);
	GtkWidget* partTextView = gtk_text_view_new_with_buffer (GTK_TEXT_BUFFER (partBuffer));
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (partTextView), GTK_WRAP_WORD);
	gtk_text_view_set_editable (GTK_TEXT_VIEW (partTextView), FALSE);
	gtk_text_view_set_left_margin(GTK_TEXT_VIEW (partTextView), 5);
	gtk_text_view_set_right_margin(GTK_TEXT_VIEW (partTextView), 5);
	gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW (partTextView), 5);
	return partTextView;
}

/*
 * Creates the usage cases text view.
 */
static GtkWidget* main_create_usage_cases_text_view ()
{
	const int uCasesTextViewLeftMargin = 5;
	const int uCasesTextViewRightMargin = 5;
	const int uCasesTextViewBottomMargin = 5;

	// Create text view for usage cases content
	GtkTextBuffer* uCasesBuffer = gtk_text_buffer_new (NULL);
	GtkWidget* uCasesTextView = gtk_text_view_new_with_buffer (GTK_TEXT_BUFFER (uCasesBuffer));
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (uCasesTextView), GTK_WRAP_WORD);
	gtk_text_view_set_editable (GTK_TEXT_VIEW (uCasesTextView), FALSE);
	gtk_text_view_set_left_margin(GTK_TEXT_VIEW (uCasesTextView), uCasesTextViewLeftMargin);
	gtk_text_view_set_right_margin(GTK_TEXT_VIEW (uCasesTextView), uCasesTextViewRightMargin);
	gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW (uCasesTextView), uCasesTextViewBottomMargin);
	return uCasesTextView;
}

/*
 * Updates the widgets based on the tree view current selected row.
 */
static void mainWindow_process_tree_view_selected_item (GtkTreeModel *model,
											   GtkTreePath *path,
											   GtkTreeIter iter,
											   struct viewInterface* vi)
{
	int depth = gtk_tree_path_get_depth (path);
	int dpId = -1;

	// Reset level bar to zero
	gtk_level_bar_set_value (vi->dpUsageLevelBar, 0);
	gtk_label_set_text (vi->dpUsageLevelLabel, "");

	if (depth > 0) {
		GValue value = G_VALUE_INIT;
		gtk_tree_model_get_value (model, &iter, DESC_COLUMN, &value);
		const char* desc = g_value_get_string (&value);
		// Update description
		vi_set_text_view_content(vi->dpDescView, desc, FALSE);
		g_value_unset(&value);
	}

	if (depth > 1) {
		GValue value = G_VALUE_INIT;
		gtk_tree_model_get_value (model, &iter, ID_COLUMN, &value);
		dpId = g_value_get_int (&value);
		g_value_unset(&value);

		// Set freq.usage level
		GValue value2 = G_VALUE_INIT;
		gtk_tree_model_get_value (model, &iter, FREQ_USAGE_COLUMN, &value2);
		int dpFreqLevel = g_value_get_int (&value2);
		if ((dpFreqLevel >= DP_FREQ_LEVEL_MIN) && (dpFreqLevel <= DP_FREQ_LEVEL_MAX)) {
			gtk_level_bar_set_value (vi->dpUsageLevelBar, dpFreqLevel);

			// Update freq level description label
			char* desc = dpmodel_get_frequency_desc (dpFreqLevel);
			if (desc)
				gtk_label_set_text (vi->dpUsageLevelLabel, desc);

			free (desc);
		}

		g_value_unset(&value2);
	}

	// We need a full path for sqlite api's
	char* dbFullPath = filepaths_get_db_file_path_new (false);

	if (!filepaths_file_exists (dbFullPath)) {
		int len = strlen (dbFullPath) + strlen (MESSAGES_ERROR_TITLE) -1;
		char msg[len]; msg[0] = 0;
		sprintf (msg, MESSAGES_DATABASE_FILE_NOT_FOUND, dbFullPath);
		fprintf (stderr, msg);
		messageWindow_show_error (NULL, MESSAGES_ERROR_TITLE, msg);
		return;
	}
			// globals_get_db_full_path_new ();
	char** retErrMsg = NULL;

	// Refresh source view
	dpcatcontroller_refreshSourceView (dbFullPath, dpId, vi, retErrMsg);

	if (globals_hasError (retErrMsg)) {
		messageWindow_show_error (vi->mainWwindow, MESSAGES_ERROR_TITLE, *retErrMsg);
		free (*retErrMsg);
	}

	// Refresh participants
	dppartcontroller_refresh_Participants (dbFullPath, dpId, vi, retErrMsg);

	if (globals_hasError (retErrMsg)) {
		messageWindow_show_error (vi->mainWwindow, MESSAGES_ERROR_TITLE, *retErrMsg);
		free (*retErrMsg);
	}

	// Refresh participants
	dpucasescontroller_refresh_cases (dbFullPath, dpId, vi, retErrMsg);

	if (globals_hasError (retErrMsg)) {
		messageWindow_show_error (vi->mainWwindow, MESSAGES_ERROR_TITLE, *retErrMsg);
		free (*retErrMsg);
	}

//	gtk_widget_set_size_request(GTK_WIDGET (wi->expander), 10, 10);

	free (dbFullPath);
}

/*
 * Event handler fot GtkTreeView 'row-activate' event.
 *
 * When a row is selected, the source view(s) and other widgets should be updated with code of selected
 * design pattern.
 */
static void on_row_activated (GtkTreeView *tree_view, GtkTreePath *path, GtkTreeViewColumn *column,
							  gpointer user_data)
{
    GtkTreeModel *model;
    GtkTreeIter iter;
    struct viewInterface* vi = (struct viewInterface*)user_data;

    model = gtk_tree_view_get_model (tree_view);

    // get iter for selected row
    if (gtk_tree_model_get_iter (model, &iter, path)) {
    	mainWindow_process_tree_view_selected_item (model, path, iter, vi);
    }
}

/*
 * Tree view button press event handler.
 */
static void on_button_press (GtkWidget *treeview,
					  GdkEventButton *event,
					  gpointer user_data) {
    if (event->type == GDK_BUTTON_PRESS && event->button == 1) {  // Check for left button click
        GtkTreePath *path;
        GtkTreeModel *model;
        GtkTreeIter iter;
        struct viewInterface* wi = (struct viewInterface*)user_data;

        if (gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(treeview), event->x, event->y, &path, NULL, NULL, NULL)) {
            model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));

            if (gtk_tree_model_get_iter (model, &iter, path)) {
            	mainWindow_process_tree_view_selected_item (model, path, iter, wi);
            }

            gtk_tree_path_free(path);
        }
    }
}

/*
 * Sets the text color of a given label with a given CSS color string.
 * Note: private function.
 */
static void _mainWindow_set_label_color (GtkLabel *label, const char *color) {
    GtkStyleContext *context = gtk_widget_get_style_context (GTK_WIDGET(label));
    const char *css_format = "label { color: %s; }";
    char *css = g_strdup_printf (css_format, color);
    GtkCssProvider *provider = gtk_css_provider_new ();
    gtk_css_provider_load_from_data (provider, css, -1, NULL);
    gtk_style_context_add_provider (context, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_free (css);
}

/*
 * Sets the color text of a given label.
 */
static void mainWindow_set_label_color (GtkLabel *label, const GdkRGBA labelColor)
{
	if (label) {
		// Convert GdkRGBA to a CSS color string
		gchar *cssColor = gdk_rgba_to_string (&labelColor);
		_mainWindow_set_label_color (label, cssColor);
		free (cssColor);
	}
}

/*
 * Gets the application style color for label text.
 * Note: You should later release result with free ().
 */
static GdkRGBA* mainWindow_get_app_color_label_text (GtkWidget* widget)
{
	// Obtain the style context of the widget
	GtkStyleContext *context = gtk_widget_get_style_context (widget);

	// Get the color for the "INSENSITIVE" state
	GdkRGBA* labelColor = (GdkRGBA*)malloc (sizeof (GdkRGBA));
	gtk_style_context_get_color (context, GTK_STATE_FLAG_INSENSITIVE, labelColor);
	return labelColor;
}

/*
 * Conistent format of label widgets.
 */
static void mainWindow_format_label (GtkLabel* label) {
	if (label) {
		const int labelLeftMargin = 8;
		const int labelRightMargin = 4;
		const int labelTopMargin = 3;
		const int labelBottomMargin = 7;

		// Get the color for the "INSENSITIVE" state
		GdkRGBA* labelColor = mainWindow_get_app_color_label_text (GTK_WIDGET (label));

		// Set text color
		mainWindow_set_label_color (label, *labelColor);

		// Bold text
		vi_set_label_bold (GTK_LABEL (label));

		// Set margins and alignment
		gtk_widget_set_halign (GTK_WIDGET (label), GTK_ALIGN_START);
		gtk_widget_set_margin_start (GTK_WIDGET (label), labelLeftMargin);
		gtk_widget_set_margin_end (GTK_WIDGET (label), labelRightMargin);
		gtk_widget_set_margin_top (GTK_WIDGET (label), labelTopMargin);
		gtk_widget_set_margin_bottom (GTK_WIDGET (label), labelBottomMargin);

		free (labelColor);
	}
}

/*
 * Creates the applicatiion main window.
 * Note: Tipicaly can be invoked in the GtkApplication 'activate' signal
 * 		 event handler.
 */
void mainWindow_create_main_window (GtkApplication* app, struct viewInterface* vi,
									char* dbFullPath)
{
	GtkWidget *window;
	GtkWidget *hpaned1;
	GtkWidget *header;
	const char* C_FREQ_USAGE_BAR_TITLE = MESSAGES_FREQUENCY_USE_AND_SPACE;
	const int boxpadding = 10;
	const int hBoxFreqUsageTopBottomMargin = 12;
	const int hBoxFreqUsageLeftMargin = 5;
	const int hBoxFreqUsageRightMargin = 8;
	const int hPaned1LeftMargin = 5;
	const int hPaned1RightMargin = 5;
	const int hPaned1BottomMargin = 5;

	// Create app main window
	window = gtk_application_window_new (app);
	vi->mainWwindow = GTK_WINDOW (window);

	// Get current app theme name
	vi->sysThemeName = vi_alloc_current_theme_name ();

	// Create top level and header boxes
	GtkWidget * toplevel_vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, boxpadding);
	GtkWidget * header_hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, boxpadding);

	// Create window header bar
	header = main_create_header_bar (vi);
	gtk_window_set_titlebar (GTK_WINDOW (window), header);

	// add title box to top level box
	gtk_box_pack_start (GTK_BOX (toplevel_vbox), header_hbox, FALSE, FALSE, 0);

	hpaned1 = gtk_paned_new (GTK_ORIENTATION_HORIZONTAL);
	vi->dpCatPaned = GTK_PANED (hpaned1);

	gtk_paned_set_wide_handle (GTK_PANED (hpaned1), TRUE);
	gtk_widget_set_can_focus (hpaned1, TRUE);

	// ------- Categories tree view --------------
	vi->treeView1 = GTK_TREE_VIEW (main_create_tree_view ());

	// Add the GtkScrolledWindow to tree view
	GtkWidget* scrolledWindow_treeView = gtk_scrolled_window_new (NULL, NULL);
	gtk_container_add (GTK_CONTAINER (scrolledWindow_treeView), GTK_WIDGET (vi->treeView1));

	// ------- Source code --------------
	// Create label for source code panel
	GtkWidget* labelCode = gtk_label_new (MESSAGES_CODE_EXAMPLE);
	mainWindow_format_label (GTK_LABEL (labelCode));

	// Create label for code language name
	GtkWidget* labelCodeLangName = gtk_label_new ("");
	vi->dpCodeLangNameLabel = GTK_LABEL (labelCodeLangName);
	mainWindow_format_label (GTK_LABEL (labelCodeLangName));

	// Create horizontal box to pack example code and language name labels
	GtkWidget* hBoxLabelExCode = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start (GTK_BOX (hBoxLabelExCode), labelCode,
						FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (hBoxLabelExCode), GTK_WIDGET (vi->dpCodeLangNameLabel),
						TRUE, TRUE, 0);

	// Create a GtkSourceView
	vi->sourceView1 = GTK_SOURCE_VIEW (main_create_source_view ());

	// Add the GtkSourceView to a GtkScrolledWindow
	GtkWidget* scrolledWindow_sourceView = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_set_margin_start ( GTK_WIDGET (scrolledWindow_sourceView), 5);
	gtk_container_add (GTK_CONTAINER (scrolledWindow_sourceView), GTK_WIDGET (vi->sourceView1));

	// Create description expander
	// Create an expander
	vi->expander = GTK_EXPANDER (main_create_dpdesc_expander ());
//	g_signal_connect( vi->expander, "notify::expanded", G_CALLBACK (on_expander_description_expanded), NULL);

	GtkWidget* expLabel = gtk_expander_get_label_widget (vi->expander);
	if (expLabel) {
		GdkRGBA* textColor = mainWindow_get_app_color_label_text (expLabel);
		mainWindow_set_label_color (GTK_LABEL (expLabel), *textColor);
		free (textColor);
	}

	vi->dpDescView = GTK_TEXT_VIEW (main_create_dpdesc_text_view ());

	// Pack the TextView into the expander
	gtk_container_add (GTK_CONTAINER (vi->expander), GTK_WIDGET (vi->dpDescView));

	// Create VBOX for hpaned child 2
	GtkWidget* vBoxCode = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_set_homogeneous (GTK_BOX (vBoxCode), FALSE);

	// Create GtkLevelBar
	vi->dpUsageLevelBar = GTK_LEVEL_BAR (main_create_usage_bar ());

	// Create usage title label
	GtkWidget* usageBarTitle = gtk_label_new (C_FREQ_USAGE_BAR_TITLE);

	// Create usage level description label
	GtkWidget* usageLevelDesc = gtk_label_new ("");
	vi->dpUsageLevelLabel = GTK_LABEL (usageLevelDesc);

	// Pack it in a horizontal box
	GtkWidget* hBoxFreqUsage = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start (GTK_BOX (hBoxFreqUsage), usageBarTitle, FALSE, TRUE, 3);
	gtk_box_pack_start (GTK_BOX (hBoxFreqUsage), GTK_WIDGET (vi->dpUsageLevelBar), FALSE, TRUE, 3);
	gtk_box_pack_start ( GTK_BOX (hBoxFreqUsage), usageLevelDesc, FALSE, TRUE, 3);

	gtk_widget_set_margin_start ( GTK_WIDGET (hBoxFreqUsage), hBoxFreqUsageLeftMargin);
	gtk_widget_set_margin_bottom ( GTK_WIDGET (hBoxFreqUsage), hBoxFreqUsageTopBottomMargin);
	gtk_widget_set_margin_top ( GTK_WIDGET (hBoxFreqUsage), hBoxFreqUsageTopBottomMargin);
	gtk_widget_set_margin_end ( GTK_WIDGET (hBoxFreqUsage), hBoxFreqUsageRightMargin);

	// Add expander
	gtk_box_pack_start (GTK_BOX (vBoxCode), GTK_WIDGET (vi->expander), FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vBoxCode), hBoxFreqUsage, FALSE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (vBoxCode), hBoxLabelExCode, FALSE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (vBoxCode), scrolledWindow_sourceView, TRUE, TRUE, 0);

	// Create a new horizontal paned widget for participants and usage cases textviews.
	GtkWidget* codePartPaned = gtk_paned_new (GTK_ORIENTATION_HORIZONTAL);
	vi->dpCodePaned = GTK_PANED (codePartPaned);

	gtk_paned_set_wide_handle (GTK_PANED (codePartPaned), TRUE);
//	gtk_paned_set_position (GTK_PANED (codePartPaned), 200);
	gtk_widget_set_can_focus (codePartPaned, TRUE);

	vi->dpPartsView = GTK_TEXT_VIEW (main_create_participants_text_view ());

	// Create label for participants panel
	GtkWidget* labelPart = gtk_label_new (MESSAGES_PARTICIPANTS_WORD);
	mainWindow_format_label (GTK_LABEL (labelPart));

	// Add the GtkTextView to a GtkScrolledWindow
	GtkWidget* scrolledWindow_partView = gtk_scrolled_window_new (NULL, NULL);
	gtk_container_add (GTK_CONTAINER (scrolledWindow_partView), GTK_WIDGET (vi->dpPartsView));

	gtk_widget_set_margin_start (GTK_WIDGET (hpaned1), hPaned1LeftMargin);
	gtk_widget_set_margin_bottom (GTK_WIDGET (hpaned1), hPaned1BottomMargin);
	gtk_widget_set_margin_end (GTK_WIDGET (hpaned1), hPaned1RightMargin);

	// Create vbox for participants panel and add label and participants scrolled window
	GtkWidget* partVbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_pack_start (GTK_BOX (partVbox), GTK_WIDGET (labelPart), FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (partVbox), GTK_WIDGET (scrolledWindow_partView),
											TRUE, TRUE, 0);

	// Create participants/usage cases paned container
	GtkWidget* partCasesPaned = gtk_paned_new (GTK_ORIENTATION_VERTICAL);
	vi->dpCatPaned = GTK_PANED (partCasesPaned);

	gtk_paned_set_wide_handle (GTK_PANED (partCasesPaned), TRUE);
//	gtk_paned_set_position (GTK_PANED (partCasesPaned), 150);
	gtk_widget_set_can_focus (partCasesPaned, TRUE);

	// ------- Usage Cases --------------
	// Create label for usage cases panel
	GtkWidget* labelCases = gtk_label_new (MESSAGES_USAGE_CASES_WORD);
	mainWindow_format_label (GTK_LABEL (labelCases));

	vi->dpUCasesView = GTK_TEXT_VIEW (main_create_usage_cases_text_view ());

	// Add the GtkTextView of usage cases to a GtkScrolledWindow
	GtkWidget* scrolledWindowCasesView = gtk_scrolled_window_new (NULL, NULL);
	gtk_container_add (GTK_CONTAINER (scrolledWindowCasesView), GTK_WIDGET (vi->dpUCasesView));

	// Create vbox for usage cases panel and add label and usage cases scrolled window
	GtkWidget* casesVbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_pack_start (GTK_BOX (casesVbox), GTK_WIDGET (labelCases), FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (casesVbox), GTK_WIDGET (scrolledWindowCasesView),
												TRUE, TRUE, 0);

	// Add participants and usage cases vertical boxes to host paned
	gtk_paned_pack1 (GTK_PANED (partCasesPaned), GTK_WIDGET (partVbox), FALSE, TRUE);
	gtk_paned_pack2 (GTK_PANED (partCasesPaned), GTK_WIDGET (casesVbox), FALSE, FALSE);

	// Add code box and part/cases paned to host paned
//	gtk_paned_add1 (GTK_PANED (codePartPaned), GTK_WIDGET (vBoxCode));
//	gtk_paned_add2 (GTK_PANED (codePartPaned), GTK_WIDGET (partCasesPaned));

	gtk_paned_pack1 (GTK_PANED (codePartPaned), GTK_WIDGET (vBoxCode), TRUE, TRUE);
	gtk_paned_pack2 (GTK_PANED (codePartPaned), GTK_WIDGET (partCasesPaned), TRUE, TRUE);

	// Add tree view and right paned container to outer paned panel
//	gtk_paned_add1 (GTK_PANED (hpaned1), scrolledWindow_treeView);
	gtk_paned_pack1 (GTK_PANED (hpaned1), scrolledWindow_treeView, FALSE, TRUE);

//	gtk_paned_add2 (GTK_PANED (hpaned1), GTK_WIDGET (codePartPaned));
	gtk_paned_pack2 (GTK_PANED (hpaned1), GTK_WIDGET (codePartPaned), TRUE, TRUE);


	// add paned panel to top level vbox
	gtk_box_pack_end (GTK_BOX (toplevel_vbox), hpaned1, TRUE, TRUE, 0);

	gtk_container_add(GTK_CONTAINER(window), toplevel_vbox);

	// Show all widgets
	gtk_widget_show_all (window);

	// Resizes window to 80% screen width and 80% screen height
	// and sets default size with same dimensions
	vi_set_window_size_perc (GTK_WINDOW (window), 80, 80);
	gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER_ALWAYS);

	int w = 0; int h = 0;
	gtk_window_get_default_size (GTK_WINDOW (window), &w, &h);
//	w = 1024; h = 614;

	// Set position of panel splitters based on current default size
	gtk_paned_set_position (GTK_PANED (hpaned1), (gint)(w/4.8));
	gtk_paned_set_position (GTK_PANED (codePartPaned), (gint)(w/3.0));
//	printf ("Code/partpane: %i\n", gtk_paned_get_position (GTK_PANED (codePartPaned)));
	gtk_paned_set_position (GTK_PANED (partCasesPaned), (gint)(h/2.5));
//	printf ("Part/cases: %i\n", gtk_paned_get_position (GTK_PANED (partCasesPaned)));

//	// Execute tests
//	mainO_test_all();

	char** retErrMsg = malloc (sizeof (char**));
	// Load TreeView
	dpcatcontroller_refreshTreeView (dbFullPath, vi, retErrMsg);

	if (globals_hasError (retErrMsg)) {
		messageWindow_show_error (GTK_WINDOW (window),
								  "Error", *retErrMsg);
	}

	free (retErrMsg);

	// Connect the row-activated signal
	g_signal_connect (vi->treeView1, "row-activated",
					  G_CALLBACK (on_row_activated), vi);
	// Connect the row-activated signal
	g_signal_connect (vi->treeView1, "button-press-event",
					  G_CALLBACK (on_button_press), vi);
}

