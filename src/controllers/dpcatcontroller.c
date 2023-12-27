/*
 * dpcatcontroler.c
 *
 *  Created on: 25/11/2023
 *      Author: Tiago C. Teixeira
 * Description: Implements the design patern categories controler.
 */

#include "../controllers/dpcatcontroller.h"

#include <stdbool.h>
#include "../viewinterface.h"
#include "../models/dpcatmodel.h"
#include "../models/dpmodel.h"
#include "../models/records.h"
#include "../globals.h"
#include "../messages.h"

static void set_cell_data(GtkTreeViewColumn *column, GtkCellRenderer *renderer,
                           GtkTreeModel *model, GtkTreeIter *iter, gpointer user_data)
{
	gchar *text;
	gint depth;

	// Get the text and depth from the model
	gtk_tree_model_get (model, iter, NAME_COLUMN, &text, DEPTH, &depth, -1);
//	gtk_tree_model_get (model, iter, DEPTH, &depth, -1);

	// Apply Pango markup based on the depth
	if (depth == 1) {
		g_object_set (renderer, "markup", g_strdup_printf("<b><i>%s</i></b>", text), NULL);
	} else {
		g_object_set (renderer, "markup", text, NULL);
	}

	g_free(text);
}

/*
 * Adds columns to the tree view.
 */
void dpcatcontroller_add_tree_view_columns (GtkTreeView* tv) {
	// Add a hidden column for ID
    GtkTreeViewColumn *column_id = gtk_tree_view_column_new ();
    gtk_tree_view_append_column (GTK_TREE_VIEW (tv), column_id);
    gtk_tree_view_column_set_visible (column_id, FALSE);

    GtkCellRenderer *renderer_id = gtk_cell_renderer_text_new ();
    gtk_tree_view_column_pack_start (column_id, renderer_id, TRUE);
    gtk_tree_view_column_add_attribute (column_id, renderer_id, "text", ID_COLUMN);

    // Add a visible column for Name
    GtkTreeViewColumn *column_name = gtk_tree_view_column_new ();
//    gtk_tree_view_column_set_title (column_name, "Name");

    GtkCellRenderer *renderer_name = gtk_cell_renderer_text_new ();
    gtk_tree_view_column_pack_start (column_name, renderer_name, TRUE);
    gtk_tree_view_column_add_attribute (column_name, renderer_name, "text", NAME_COLUMN);

    // Use a function to set the cell data functionally
	gtk_tree_view_column_set_cell_data_func (column_name, renderer_name, (GtkTreeCellDataFunc)set_cell_data, NULL, NULL);

	gtk_tree_view_append_column (GTK_TREE_VIEW (tv), column_name);
	gtk_tree_view_column_set_title (column_name, MESSAGES_PATTERNS_WORD);

    // Add a invisible column for Desc
	GtkTreeViewColumn *column_desc = gtk_tree_view_column_new ();
	gtk_tree_view_column_set_title (column_desc, "Desc");
	gtk_tree_view_append_column (GTK_TREE_VIEW (tv), column_desc);
	gtk_tree_view_column_set_visible (column_desc, FALSE);

	GtkCellRenderer *renderer_desc = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (column_desc, renderer_desc, TRUE);
	gtk_tree_view_column_add_attribute (column_desc, renderer_desc, "text", DESC_COLUMN);

	// Add a invisible column for freq usage level
	GtkTreeViewColumn *column_freq_usage_level = gtk_tree_view_column_new ();
	gtk_tree_view_column_set_title (column_freq_usage_level, "Freq Usage");
	gtk_tree_view_append_column (GTK_TREE_VIEW (tv), column_freq_usage_level);
	gtk_tree_view_column_set_visible (column_freq_usage_level, FALSE);

	GtkCellRenderer *renderer_freq_usage = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (column_freq_usage_level, renderer_freq_usage, TRUE);
	gtk_tree_view_column_add_attribute (column_freq_usage_level, renderer_freq_usage, "text", FREQ_USAGE_COLUMN);

	// Add a invisible column for freq usage description
	GtkTreeViewColumn *column_freq_usage_desc = gtk_tree_view_column_new ();
	gtk_tree_view_column_set_title (column_freq_usage_desc, "Freq Usage");
	gtk_tree_view_append_column (GTK_TREE_VIEW (tv), column_freq_usage_desc);
	gtk_tree_view_column_set_visible (column_freq_usage_desc, FALSE);

	GtkCellRenderer *renderer_freq_usage_desc = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (column_freq_usage_desc, renderer_freq_usage_desc, TRUE);
	gtk_tree_view_column_add_attribute (column_freq_usage_desc, renderer_freq_usage_desc, "text", FREQ_USAGE_DESC_COLUMN);
}

/*
 * Creates the tree model and fills the given tree view.
 */
void dpcatcontroller_fill_tree_view (GtkTreeView* tv, struct treeItem** items, int count)
{
	GtkTreeIter iter;

	/* create tree store */
	GtkTreeStore* model = gtk_tree_store_new (NUM_COLUMNS,
							G_TYPE_INT,		// Id column
							G_TYPE_STRING,	// Name column
							G_TYPE_STRING, 	// Desc column
							G_TYPE_INT, 	// Freq usage
							G_TYPE_STRING,	// Freq. usage desc
							G_TYPE_INT		// Depth
						   );

	struct dpcatmodel* cat = NULL;
	struct dpmodel* dp = NULL;

	for (int i = 0; i < count; ++i) {
		cat = items[i]->cat;
		gtk_tree_store_append (model, &iter, NULL);
		gtk_tree_store_set (model, &iter,
							ID_COLUMN, cat->id,
						    NAME_COLUMN, cat->name,
							DESC_COLUMN, cat->desc,
							FREQ_USAGE_COLUMN, NULL,
							FREQ_USAGE_DESC_COLUMN, NULL,
							DEPTH, 1,
						    -1);

		struct tableRowList* patterns = items[i]->patterns;

		if (patterns) {
			// start from first design pattern row
			struct tableRow* dpRow = (struct tableRow*)(patterns->first);
			for (int pat = 0; pat < items[i]->patterns->size; ++pat) {
				GtkTreeIter childIter;
				dp = (struct dpmodel*)(dpRow->data);

				// Append design pattern to Tree Store as child of current category.
				gtk_tree_store_append (model, &childIter, &iter);
				gtk_tree_store_set (model, &childIter,
									ID_COLUMN, dp->id,
									NAME_COLUMN, dp->name,
									DESC_COLUMN, dp->desc,
									FREQ_USAGE_COLUMN, dp->freqUsage,
									FREQ_USAGE_DESC_COLUMN, dp->freqUsageDesc,
									DEPTH, 2,
									-1);

				dpRow = dpRow->next;
			}	// for (int pat = 0...
		}	// if (patterns)...
	}	// for (int i = 0; i...

	gtk_tree_view_set_model (tv, GTK_TREE_MODEL (model));
	dpcatcontroller_add_tree_view_columns (tv);
	gtk_widget_set_vexpand (GTK_WIDGET (tv), TRUE);
	g_object_unref (model);
}

/*
 * Releases resources allocated by TreeItem array.
 */
void dpcatcontroller_destroy_treeItemArr (struct treeItem** tItemArr, int size)
{
	if (tItemArr) {
		for (int i = 0; i < size; ++i) {
			// It's just a pointer to another struct that
			// will be released later.
			tItemArr[i]->patterns = NULL;
			dpcatmodel_destroy_model (tItemArr[i]->cat);
			free (tItemArr[i]);
		}

		free (tItemArr);
	}
}

/*
 * Updates the tree view with categories and design paterns loaded from database.
 */
void dpcatcontroller_refreshTreeView (const char* databasePath, struct viewInterface* wi, char** retErrMsg)
{
	struct tableRowList* cats = dpcatmodel_load_categories (databasePath, retErrMsg);
	struct tableRowList* patts = NULL;	// Store patterns retrieved from database.
	struct treeItem** tItemArr = NULL;	// Struct that will populate tree view.
	int arrSize = 0;

	if (globals_hasError(retErrMsg) || (cats == NULL)) {
		goto TERMINATE;
	}

	if (cats->size > 0) {
		// Allocate TreeItem array
		tItemArr = malloc(cats->size * sizeof(*tItemArr));
		struct tableRow* row = cats->first;
		int catId = -1;
		for (int i = 0; i < cats->size; ++i) {
			// Allocate TreeItem struct
			tItemArr[i] = malloc(sizeof(struct treeItem));
			arrSize++;
			// Allocate category model struct
			tItemArr[i]->cat = malloc(sizeof(struct dpcatmodel));

			// Copy data to category model
			tItemArr[i]->cat->id = ((struct dpcatmodel*)(row->data))->id;
			tItemArr[i]->cat->name = strdup (((struct dpcatmodel*)(row->data))->name);
			tItemArr[i]->cat->desc = strdup (((struct dpcatmodel*)(row->data))->desc);


//			memcpy (tItemArr[i]->cat->name,
//					((struct dpcatmodel*)(row->data))->name,
//					DP_CAT_FIELD_DP_CAT_NAME_NUMCHARS);
//			memcpy (tItemArr[i]->cat->desc,
//								((struct dpcatmodel*)(row->data))->desc,
//								DP_CAT_FIELD_DP_CAT_DESC_NUMCHARS);

			// Load patterns from database for current category
			catId = tItemArr[i]->cat->id;
			patts = dpmodel_load_patterns (databasePath, catId, retErrMsg);

			if (globals_hasError(retErrMsg) || (patts == NULL)) {
				goto TERMINATE;
			}

			// Save patterns list reference in TreeItem struct
			tItemArr[i]->patterns = patts;
			row = row->next;
		}

		// Fill the TreeView with TreeItem data.
		dpcatcontroller_fill_tree_view (wi->treeView1, tItemArr, cats->size);
	}

TERMINATE:
	if (tItemArr)
		dpcatcontroller_destroy_treeItemArr (tItemArr, arrSize);
	if (cats)
		records_list_destroy (cats);
	if (patts)
		records_list_destroy (patts);
}


