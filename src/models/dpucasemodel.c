/*
 * dpucasesmodel.c
 *
 *  Created on: 01/12/2023
 *      Author: Tiago C. Teixeira
 * Description: Implementation for design pattern usage cases model.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sql.h"
#include <sqlite3.h>
#include "records.h"
#include "base.h"
#include "dpucasemodel.h"
#include "../gettext.h"
#include "../messages.h"

/*
 * Releases the model resources.
 */
void dpucasemodel_destroy_model (struct dpucasemodel* model) {
	if (model) {
		free (model->desig);
		free (model->desc);
		free (model);
	}
}

/*
 * Releases data from memory.
 */
void dpucasemodel_freeData_callback (void* data)
{
	dpucasemodel_destroy_model ((struct dpucasemodel*)data);
}

/*
 * Creates a model instance.
 */
struct dpucasemodel* dpucasemodel_create_data () {
	struct dpucasemodel* res = (struct dpucasemodel*)malloc (sizeof(*res));
	if (!res) {
		fprintf ( stderr, MESSAGES_MEMORY_ALLOC_ERROR, "dpucasemodel struct");
		abort ();
	}

	res->id = -1;
	res->desig = NULL;
	res->desc = NULL;
	res->dpId = -1;
	return res;
}

/*
 * Creates the model structure filled with database record fields data.
 * Returns the model struct filled with database record fields data.
 * Note: Sqlite statment must point to data record.
 */
void* dpucasemodel_fillData_callback (sqlite3_stmt* stmt)
{
	struct dpucasemodel* model = dpucasemodel_create_data ();
	if (model) {
		// Part Id
		model->id = sqlite3_column_int (stmt, UCASES_FIELD_INDEX_DP_UCASE_ID);

		// Designation
		model->desig = strdup ((char*)sqlite3_column_text (stmt, UCASES_FIELD_INDEX_DP_UCASE_DESIG));

		// Description
		model->desc = strdup ((char*)sqlite3_column_text (stmt, UCASES_FIELD_INDEX_DP_UCASE_DESC));

		// Design patterns Id
		model->dpId = sqlite3_column_int (stmt, UCASES_FIELD_INDEX_DP_ID);
	}

	return model;
}

/*
 * Loads data for a given design pattern from database.
 * Returns list of records loaded from database.
 */
struct tableRowList* dpucasemodel_load_cases (const char* filePath, int dpId, char** retErrMsg)
{
	char* sql = SQLITE_USAGE_CASES_QUERY;
	struct sqlParameter* params[1];

	params[0] = (struct sqlParameter*)malloc (sizeof(struct sqlParameter));
	params[0]->dataType = SQLITE3_DATA_TYPE_INTEGER;
	params[0]->value = &dpId;

	struct tableRowList* result =
			base_execute_select_params (sql, filePath, params, 1,
										dpucasemodel_fillData_callback,
										dpucasemodel_freeData_callback,
										retErrMsg);
	free (params[0]);
	return result;
}

/*
 * Function to test model.
 */
void dpucasemodel_test (const char* databasePath) {
	char* retErrMsg = 0;
	int dpId = 1;	// Singleton
	printf (MESSAGES_TESTING_USAGE_CASES_DATA_MODEL, "dpucasemodel");
	struct tableRowList* rows = dpucasemodel_load_cases (databasePath, dpId, &retErrMsg);

	if (!retErrMsg || (strlen(retErrMsg) == 0)) {
		char msg[50];
		sprintf (msg, MESSAGES_ROWS_LOADED, rows->size);

		char desc[255];
		struct tableRow* row = rows->first;
		for (int i = 0; i < rows->size; ++i) {
			struct dpucasemodel* ucase = (struct dpucasemodel*)(row->data);
			printf (MESSAGES_UCASE_ID, ucase->id);
			printf (MESSAGES_DP_ID, ucase->dpId);
			printf (MESSAGES_DESIGNATION, ucase->desig);

			int len = strlen (ucase->desc);
			if (len > 250) {
				len = 250;
				memcpy (desc, ucase->desc, len);
				desc[len] = ' '; desc[len+1] = '.'; desc[len+2] = '.'; desc[len+3] = '.';
				desc[len+4] = '\0';
			}
			else {
				memcpy (desc, ucase->desc, len);
				desc[len] = '\0';
			}

			printf (MESSAGES_DESCRIPTION, desc);
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
