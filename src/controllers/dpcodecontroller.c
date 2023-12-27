/*
 * dpcodecontroller.c
 *
 *  Created on: 27/11/2023
 *      Author: Tiago C. Teixeira
 * Description: Implements the design patterns code model.
 */

#include <stdlib.h>
#include "../models/dpcodemodel.h"
#include <gtksourceview-3.0/gtksourceview/gtksource.h>

//#include <gtksourceview-3.0/gtksourceview/gtksourcebuffer.h>
#include "../globals.h"
#include "../models/records.h"
#include "../viewinterface.h"

/*
 * Updates the description content.
 */
void dpcodecontroller_update_description (struct viewInterface* wi, const char* desc)
{
	vi_set_text_view_content (wi->dpDescView, desc, FALSE);
}

/*
 * Update text content in the SourceView.
 */
void dpcodecontroller_update_sourceView (struct viewInterface* wi, struct tableRowList* codes)
{
	struct tableRow* row = NULL;
	struct dpcodemodel* model = NULL;

	// Clear current content
	vi_set_source_view_content (wi->sourceView1, "");
	gtk_label_set_label (wi->dpCodeLangNameLabel, "");

	if (codes) {
		row = codes->first;

		for (int i = 0; i < codes->size; ++i) {
			if (row->data) {
				model = (struct dpcodemodel*)(row->data);
				vi_set_source_view_content (wi->sourceView1, model->content);

				// Set lang name
				char* langName = g_strdup_printf ("(%s)", model->langName);
				gtk_label_set_label (wi->dpCodeLangNameLabel, langName);
				free (langName);
			}

			row = row->next;
		}
	}

	row = NULL;
	model = NULL;
}

/*
 * Updates the source view content with design paterns code examples from database.
 */
void dpcatcontroller_refreshSourceView (const char* databasePath, int dpId, struct viewInterface* wi, char** retErrMsg)
{
	if (dpId < 0)
		dpcodecontroller_update_sourceView (wi, NULL);
	else {
		struct tableRowList* codes = dpcodemodel_load_code (databasePath, dpId, retErrMsg);
		if (!(globals_hasError(retErrMsg) || (codes == NULL))) {
			// Update content in the SourceView.
			dpcodecontroller_update_sourceView (wi, codes);
		}

		if (codes)
			records_list_destroy (codes);
	}
}
