/*
 * dppartcontroller.c
 *
 *  Created on: 29/11/2023
 *      Author: Tiago C. Teixeira
 * Description: Implements the design pattern participants controller.
 */

#include <stdlib.h>
#include "../models/dppartmodel.h"
#include "../models/records.h"
#include <string.h>
#include "dppartcontroller.h"
#include "../globals.h"
#include "../viewinterface.h"

/*
 * Update text content in the SourceView.
 */
void dppartcontroller_update_text_view (struct viewInterface* wi, struct tableRowList* parts)
{
	struct tableRow* row = NULL;
	struct dppartmodel* model = NULL;

	// Clear current content
	vi_set_text_view_content (wi->dpPartsView, "", FALSE);
	if (parts) {
		row = parts->first;

		for (int i = 0; i < parts->size; ++i) {
			if (row->data) {
				model = (struct dppartmodel*)(row->data);

				// Add name
				int len = strlen(model->name) + strlen("<b></b>") + 1;
				char markup[len];
				sprintf (markup, "<b>%s</b>", model->name);
				vi_text_view_append_text (wi->dpPartsView, markup, TRUE);
				vi_text_view_append_text (wi->dpPartsView, "\n", FALSE);

				// Add description
				vi_text_view_append_text (wi->dpPartsView, model->desc, TRUE);
				vi_text_view_append_text (wi->dpPartsView, "\n\n", FALSE);
			}

			row = row->next;
		}
	}

	row = NULL;
	model = NULL;
}

/*
 * Updates the participants text view with design paterns participants from database.
 */
void dppartcontroller_refresh_Participants (const char* databasePath, int dpId, struct viewInterface* wi, char** retErrMsg)
{
	if (dpId < 0)
		dppartcontroller_update_text_view (wi, NULL);
	else {
		struct tableRowList* parts = dppartmodel_load_parts (databasePath, dpId, retErrMsg);
		if (!(globals_hasError(retErrMsg) || (parts == NULL))) {
			// Update content in the SourceView.
			dppartcontroller_update_text_view (wi, parts);
		}

		if (parts)
			records_list_destroy (parts);
	}
}

