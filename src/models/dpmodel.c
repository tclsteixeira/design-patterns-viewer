/*
 * domodel.c
 *
 *  Created on: 21/11/2023
 *      Author: Tiago C. Teixeira
 * Description: Implements the Design Patterns data model.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "base.h"
#include "records.h"
#include "dpmodel.h"
#include "sql.h"
#include <string.h>
#include "../gettext.h"
#include "../messages.h"

/*
 * Gets the frequency usage description for a given frequency level.
 * Returns frequency level description if succeeded, empty string otherwise.
 * Note: the returned string must be released later with free ().
 */
char* dpmodel_get_frequency_desc (const int freqLevel) {
	if ((freqLevel < DP_FREQ_LEVEL_MIN) || (freqLevel > DP_FREQ_LEVEL_MAX)) {
		fprintf (stderr, "Invalid frequency level '%d'. Must be between '%d' and '%d'.\n", freqLevel, DP_FREQ_LEVEL_MIN, DP_FREQ_LEVEL_MAX);
		return "";
	}

	char* descs[6] = { "", DP_FREQ_DESC_LOW, DP_FREQ_DESC_MEDIUM_LOW, DP_FREQ_DESC_MEDIUM, DP_FREQ_DESC_MEDIUM_HIGH, DP_FREQ_DESC_HIGH };
	return strdup ( descs[freqLevel] );
}

/*
 * Creates a design pattern model instance.
 */
struct dpmodel* dpmodel_create_data () {
	struct dpmodel* res = (struct dpmodel*)malloc(sizeof(*res));
	if (!res) {
		fprintf (stderr, MESSAGES_MEMORY_ALLOC_ERROR, "dpmodel struct");
		abort ();
	}

	res->id = -1;
	res->name = NULL;
	res->desc = NULL;
	res->catId = -1;
	res->freqUsage = 0;
	res->freqUsageDesc = NULL;
	return res;
}

/*
 * Creates the model structure filled with database record fields data.
 * Returns the model struct filled with database record fields data.
 */
void* dpmodel_fillData_callback (sqlite3_stmt* stmt)
{
	struct dpmodel* model = dpmodel_create_data ();
	if (model) {
		// Id
		model->id = sqlite3_column_int (stmt, DP_FIELD_INDEX_DP_ID);
		// Name
		model->name = strdup ((char*)sqlite3_column_text (stmt, DP_FIELD_INDEX_DP_NAME));
		// Description
		model->desc = strdup ((char*)sqlite3_column_text (stmt, DP_FIELD_INDEX_DP_DESC));
		// CatId
		model->catId = sqlite3_column_int (stmt, DP_FIELD_INDEX_DP_CAT_ID);
		// Freq usage
		model->freqUsage = sqlite3_column_int (stmt, DP_FIELD_INDEX_DP_FREQ_USAGE);
		// Freq usage desc
		model->freqUsageDesc = strdup ((char*)sqlite3_column_text (stmt, DP_FIELD_INDEX_DP_FREQ_USAGE_DESC));
	}

	return model;
}

/*
 * Releases data from memory.
 */
void dpmodel_destroy_model (struct dpmodel* model)
{
	if (model) {
		free (model->name);
		free (model->desc);
		free (model->freqUsageDesc);
		free (model);
	}
}

/*
 * Releases data from memory.
 */
void dpmodel_freeData_callback (void* data)
{
	dpmodel_destroy_model ((struct dpmodel*)data);
}

/*
 * Loads patterns associated with a given category Id from the database.
 * Returns list of records loaded from database.
 */
struct tableRowList* dpmodel_load_patterns (const char* filePath, int catId, char** retErrMsg)
{
	char* sql = SQLITE_DP_QUERY;
	struct sqlParameter* params[1];

	params[0] = (struct sqlParameter*)malloc (sizeof(struct sqlParameter));
	params[0]->dataType = SQLITE3_DATA_TYPE_INTEGER;
	params[0]->value = &catId;

	struct tableRowList* result =
			base_execute_select_params (sql, filePath, params, 1,
										dpmodel_fillData_callback,
										dpmodel_freeData_callback,
										retErrMsg);
	free (params[0]);
	return result;
}

/*
 * Function to test model.
 */
void dpmodel_test(const char* databasePath) {
	char* retErrMsg = 0;
	int catId = 2;

	printf (MESSAGES_TESTING_DESIGN_PATTERNS_DATA_MODEL, "dpmodel");
	struct tableRowList* rows = dpmodel_load_patterns (databasePath, catId, &retErrMsg);

	if (!retErrMsg || (strlen(retErrMsg) == 0)) {
		char msg[50];
		sprintf (msg, MESSAGES_ROWS_LOADED, rows->size);
		struct tableRow* row = rows->first;
		for (int i = 0; i < rows->size; ++i) {
			struct dpmodel* dp = (struct dpmodel*)(row->data);
			printf (MESSAGES_CATEGORY_ID, dp->catId);
			printf (MESSAGES_DP_ID, dp->id);
			printf (MESSAGES_PATTERN_NAME, dp->name);
			printf (MESSAGES_FREQ_USAGE, dp->freqUsage); printf ("%s\n", dp->freqUsageDesc);
			row = row->next;
		}

		printf (MESSAGES_SUCCEEDED, msg);
		records_list_destroy (rows);
	}
	else {
		printf (MESSAGES_ERROR, retErrMsg);
		free (retErrMsg);
	}
}
