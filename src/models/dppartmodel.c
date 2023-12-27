/*
 * dppartmodel.c
 *
 *  Created on: 29/11/2023
 *      Author: Tiago C. Teixeira
 * Description: Implements the design patterns participants data model.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sql.h"
#include <sqlite3.h>
#include "records.h"
#include "base.h"
#include "dppartmodel.h"
#include "../gettext.h"
#include "../messages.h"


/*
 * Releases the model resources.
 */
void dppartmodel_destroy_model (struct dppartmodel* model) {
	if (model) {
		free (model->name);
		free (model->desc);
		free (model);
	}
}

/*
 * Releases data from memory.
 */
void dppartmodel_freeData_callback (void* data)
{
	dppartmodel_destroy_model ((struct dppartmodel*)data);
}

/*
 * Creates a participants model instance.
 */
struct dppartmodel* dppartmodel_create_data () {
	struct dppartmodel* res = (struct dppartmodel*)malloc(sizeof(*res));
	if (!res) {
		fprintf ( stderr, MESSAGES_MEMORY_ALLOC_ERROR, "dppartmodel struct");
		abort ();
	}

	res->id = -1;
	res->name = NULL;
	res->desc = NULL;
	res->dpId = -1;
	return res;
}

/*
 * Creates the model structure filled with database record fields data.
 * Returns the model struct filled with database record fields data.
 * Note: Sqlite statment must point to data record.
 */
void* dppartmodel_fillData_callback (sqlite3_stmt* stmt)
{
	struct dppartmodel* model = dppartmodel_create_data ();
	if (model) {
		// Part Id
		model->id = sqlite3_column_int (stmt, PARTICIPANTS_FIELD_INDEX_DP_PART_ID);
		// Name
		model->name = strdup ((char*)sqlite3_column_text (stmt, PARTICIPANTS_FIELD_INDEX_DP_PART_NAME));
		// Description
		model->desc = strdup ((char*)sqlite3_column_text (stmt, PARTICIPANTS_FIELD_INDEX_DP_PART_DESC));
		// Design patterns Id
		model->dpId = sqlite3_column_int (stmt, PARTICIPANTS_FIELD_INDEX_DP_ID);
	}

	return model;
}

/*
 * Loads participants data for a given design pattern from database.
 * Returns list of records loaded from database.
 */
struct tableRowList* dppartmodel_load_parts (const char* filePath, int dpId, char** retErrMsg)
{
	char* sql = SQLITE_PARTICIPANTS_QUERY;
	struct sqlParameter* params[1];

	params[0] = (struct sqlParameter*)malloc (sizeof(struct sqlParameter));
	params[0]->dataType = SQLITE3_DATA_TYPE_INTEGER;
	params[0]->value = &dpId;

	struct tableRowList* result =
			base_execute_select_params (sql, filePath, params, 1,
										dppartmodel_fillData_callback,
										dppartmodel_freeData_callback,
										retErrMsg);
	free (params[0]);
	return result;
}

/*
 * Function to test model.
 */
void dppartmodel_test (const char* databasePath) {
	char* retErrMsg = 0;
	int dpId = 1;	// Singleton
	printf (MESSAGES_TESTING_PARTICIPANTS_DATA_MODEL, "dppartmodel");
	struct tableRowList* rows = dppartmodel_load_parts (databasePath, dpId, &retErrMsg);

	if (!retErrMsg || (strlen(retErrMsg) == 0)) {
		char msg[50];
		sprintf (msg, MESSAGES_ROWS_LOADED, rows->size);

		char desc[255];
		struct tableRow* row = rows->first;
		for (int i = 0; i < rows->size; ++i) {
			struct dppartmodel* part = (struct dppartmodel*)(row->data);
			printf (MESSAGES_PARTICIPANT_ID, part->id);
			printf (MESSAGES_DP_ID, part->dpId);
			printf (MESSAGES_PARTICIPANT_NAME, part->name);

			int len = strlen (part->desc);
			if (len > 250) {
				len = 250;
				memcpy (desc, part->desc, len);
				desc[len] = ' '; desc[len+1] = '.'; desc[len+2] = '.'; desc[len+3] = '.';
				desc[len+4] = '\0';
			}
			else {
				memcpy (desc, part->desc, len);
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
