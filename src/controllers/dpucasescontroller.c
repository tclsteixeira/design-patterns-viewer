/*
 * dpucasescontroller.c
 *
 *  Created on: 03/12/2023
 *      Author: Tiago C. Teixeira
 * Description: Implements the design pattern usage cases controller.
 */

#include <stdlib.h>
#include "../models/dpucasemodel.h"
#include "../models/records.h"
#include <string.h>
#include "dpucasescontroller.h"
#include "../globals.h"
#include "../viewinterface.h"

/*
 * Update text content in the text view.
 */
void dpucasescontroller_update_text_view (struct viewInterface* wi, struct tableRowList* uCases)
{
	struct tableRow* row = NULL;
	struct dpucasemodel* model = NULL;

	// Clear current content
	vi_set_text_view_content (wi->dpUCasesView, "", FALSE);
	if (uCases) {
		row = uCases->first;

		for (int i = 0; i < uCases->size; ++i) {
			if (row->data) {
				model = (struct dpucasemodel*)(row->data);

				// Add name
				int len = strlen(model->desig) + strlen("<b></b>") + 1;
				char markup[len];
				sprintf (markup, "<b>%s</b>", model->desig);
				vi_text_view_append_text (wi->dpUCasesView, markup, TRUE);
				vi_text_view_append_text (wi->dpUCasesView, "\n", FALSE);

				// Add description
				vi_text_view_append_text (wi->dpUCasesView, model->desc, TRUE);
				vi_text_view_append_text (wi->dpUCasesView, "\n\n", FALSE);
			}

			row = row->next;
		}
	}

	row = NULL;
	model = NULL;
}

/*
 * Updates the usage cases text view with design paterns usage cases from database.
 */
void dpucasescontroller_refresh_cases (const char* databasePath, int dpId, struct viewInterface* wi, char** retErrMsg)
{
	if (dpId < 0)
		dpucasescontroller_update_text_view (wi, NULL);
	else {
		struct tableRowList* UCases = dpucasemodel_load_cases (databasePath, dpId, retErrMsg);
		if (!(globals_hasError(retErrMsg) || (UCases == NULL))) {
			// Update content in the SourceView.
			dpucasescontroller_update_text_view (wi, UCases);
		}

		if (UCases)
			records_list_destroy (UCases);
	}
}
