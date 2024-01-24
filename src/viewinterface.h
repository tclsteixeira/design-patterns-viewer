/*
 * vi.h
 *
 *  Created on: 25/11/2023
 *      Author: Tiago C. Teixeira
 * Description: Header declarations for the view interface structure.
 */

#ifndef VIEWINTERFACE_H_
	#define VIEWINTERFACE_H_

	#include <gtk-3.0/gtk/gtk.h>
	#include <gtksourceview-3.0/gtksourceview/gtksource.h>

	#define SOURCE_LANGUAGE_C "c"
	#define SOURCE_LANGUAGE_JAVA "java"
	#define SOURCE_LANGUAGE_PYTHON "python"
	#define SOURCE_LANGUAGE_VALA "vala"
	#define SOURCE_LANGUAGE_BASH "sh"
	#define SOURCE_LANGUAGE_CPLUSPLUS "cpp"
	#define SOURCE_LANGUAGE_D "d"
	#define SOURCE_LANGUAGE_R "r"

	// Themes name constants
	#define VI_THEME_SETTING_NAME "gtk-theme-name"
	#define VI_DARK_THEME_NAME "Adwaita-dark"
	#define VI_LIGHT_THEME_NAME "Adwaita"

	enum viThemeOptions {
		VI_SYSTEM_THEME = 0,
		VI_DARK_THEME,
		VI_LIGHT_THEME
	};

	/*
	 * Stores the views layer widgets to provide future access to the controllers.
	 */
	struct viewInterface {
		GtkWindow* 		mainWwindow;
		GtkHeaderBar* 	headerBar;
		GtkComboBox* 	dropDownTheme;
		GtkSourceView* 	sourceView1;
		GtkExpander* 	expander;
		GtkTreeView* 	treeView1;
		GtkTextView* 	dpDescView;
		GtkTextView* 	dpUCasesView;
		GtkTextView* 	dpPartsView;
		GtkLevelBar* 	dpUsageLevelBar;
		GtkLabel* 		dpUsageLevelLabel;
		GtkLabel* 		dpCodeLangNameLabel;
		GtkPaned* 		dpCatPaned;
		GtkPaned* 		dpPartPaned;
		GtkPaned* 		dpCodePaned;

		char* 			sysThemeName;
	};

	/*
	 * Gets the name of the current theme.
	 * result should be later free when no longer needed.
	 */
	char* vi_alloc_current_theme_name ();

	/*
	 * Sets the ui theme.
	 */
	void vi_set_theme (struct viewInterface* wi, enum viThemeOptions themeOp);

	/*
	 * Sets the label text with bold attribute.
	 */
	void vi_set_label_bold (GtkLabel* label);

	/*
	 * Sets the expander title text bold.
	 */
	void vi_set_expander_title_bold (GtkExpander* expander);

	/*
	 * Sets the source language for the GtkSourceView.
	 */
	void vi_set_source_lang (GtkSourceView* sView, char* langName);

	/*
	 * Sets cursor at start position of a given text buffer.
	 */
	void vi_set_text_buffer_cursor_at_start (GtkTextBuffer* buff);

	/*
	 * Sets the content of a given source view.
	 */
	void vi_set_source_view_content (GtkSourceView* sv, char* content);

	/*
	 * Sets the content of a given text view.
	 */
	void vi_set_text_view_content (GtkTextView* tv, const char* content, gboolean isMarkup);

	/*
	 * Appends text to a given text view.
	 */
	void vi_text_view_append_text (GtkTextView *textView, const char *text, gboolean isMarkup);

	/*
	 * Sets the window size based on screen percentages.
	 */
	void vi_set_window_size_perc (GtkWindow* window, const uint widthPerc, const uint heightPerc);

	/*
	 * Retrieves the screen resolution size for the default display.
	 */
	void vi_get_screen_resolution (int* width, int* height);

#endif /* VIEWINTERFACE_H_ */
