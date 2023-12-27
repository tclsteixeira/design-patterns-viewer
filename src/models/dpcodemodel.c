/*
 * dpcodemodel.c
 *
 *  Created on: 26/11/2023
 *      Author: Tiago C. Teixeira
 * Description: Implements the design patterns examples code model.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dpcodemodel.h"
#include "sql.h"
#include <sqlite3.h>

#include "../messages.h"
#include "records.h"
#include "base.h"

/*
 * Releases the model resources.
 */
void dpcodemodel_destroy_model (struct dpcodemodel* model) {
	if (model) {
		free (model->content);
		free (model->langName);
		free (model);
	}
}

/*
 * Creates a code model instance.
 */
struct dpcodemodel* dpcodemodel_create_data () {
	struct dpcodemodel* res = (struct dpcodemodel*)malloc(sizeof(*res));
	if (!res) {
		fprintf (stderr, MESSAGES_MEMORY_ALLOC_ERROR ,"struct dpcodemodel");
		abort();
	}

	res->id = -1;
	res->content = NULL;
	res->dpId = -1;
	res->langId = -1;
	return res;
}

/*
 * Creates the model structure filled with database record fields data.
 * Returns the model struct filled with database record fields data.
 */
void* dpcodemodel_fillData_callback (sqlite3_stmt* stmt)
{
	struct dpcodemodel* model = dpcodemodel_create_data ();
	if (model) {
		model->dpId = sqlite3_column_int (stmt, CODE_FIELDINDEX_DP_ID1);
		model->id = sqlite3_column_int (stmt, CODE_FIELDINDEX_CODE_ID);

		// Allocate memory
		int len = strlen ((char*)sqlite3_column_text (stmt, CODE_FIELDINDEX_CONTENT));
		model->content = strdup ((char*)sqlite3_column_text (stmt, CODE_FIELDINDEX_CONTENT));
		model->content[len] = '\0';

		model->langId = sqlite3_column_int (stmt, CODE_FIELDINDEX_PROG_LANG_ID);

		// Allocate memory
		len = strlen ((char*)sqlite3_column_text (stmt, CODE_FIELDINDEX_PROG_LANG_NAME));
		model->langName = strdup ((char*)sqlite3_column_text (stmt, CODE_FIELDINDEX_PROG_LANG_NAME));
		model->langName[len] = '\0';
	}

	return model;
}

/*
 * Releases data from memory.
 */
void dpcodemodel_freeData_callback (void* data)
{
	dpcodemodel_destroy_model ((struct dpcodemodel*)data);
}

/*
 * Loads code content for a given design pattern the from database.
 * Returns list of records loaded from database.
 */
struct tableRowList* dpcodemodel_load_code (const char* filePath, int dpId, char** retErrMsg)
{
	char* sql = SQLITE_CODE_QUERY;
	struct sqlParameter* params[1];

	params[0] = (struct sqlParameter*)malloc (sizeof(struct sqlParameter));
	params[0]->dataType = SQLITE3_DATA_TYPE_INTEGER;
	params[0]->value = &dpId;

	struct tableRowList* result =
			base_execute_select_params (sql, filePath, params, 1,
										dpcodemodel_fillData_callback,
										dpcodemodel_freeData_callback,
										retErrMsg);
	free (params[0]);
	return result;
}

/*
 * Function to test model.
 */
void dpcodemodel_test (const char* databasePath) {
	char* retErrMsg = 0;
	int dpId = 1;
	printf (MESSAGES_TESTING_CODE_DATA_MODEL, "dpcodemodel");
	struct tableRowList* rows = dpcodemodel_load_code (databasePath, dpId, &retErrMsg);

	if (!retErrMsg || (strlen(retErrMsg) == 0)) {
		char msg[50];
		sprintf (msg, "%i row(s) loaded.\n", rows->size);

		char code[255];
		struct tableRow* row = rows->first;
		for (int i = 0; i < rows->size; ++i) {
			struct dpcodemodel* cat = (struct dpcodemodel*)(row->data);
			printf (MESSAGES_CODE_ID, cat->id);
			printf (MESSAGES_DP_ID, cat->dpId);
			printf (MESSAGES_LANG_ID, cat->langId);
			printf (MESSAGES_LANG, cat->langName);

			int len = strlen (cat->content);
			if (len > 250) {
				len = 250;
				memcpy (code, cat->content, len);
				code[len] = ' '; code[len+1] = '.'; code[len+2] = '.'; code[len+3] = '.';
				code[len+4] = '\0';
			}
			else {
				memcpy (code, cat->content, len);
				code[len] = '\0';
			}

			printf (MESSAGES_CODE, code);
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

