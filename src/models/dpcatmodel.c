/*
 * dpcatmodel.c
 *
 *  Created on: 19/11/2023
 *      Author: Tiago C. Teixeira
 * Description: Implements the Design Categories data model.
 */


#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "base.h"
#include "records.h"
#include "dpcatmodel.h"
#include "sql.h"
#include "../gettext.h"
#include "../messages.h"


/*
 * Creates a table record instance.
 */
struct dpcatmodel* dpcatmodel_create_data () {
	struct dpcatmodel* res = (struct dpcatmodel*)malloc(sizeof(*res));
	if (!res) {
		fprintf (stderr, MESSAGES_MEMORY_ALLOC_ERROR, "struct dpcatmodel");
		abort();
	}

	res->id = -1;
	res->name = NULL;
	res->desc = NULL;
	return res;
}

/*
 * Creates the model structure filled with database record fields data.
 * Returns the model struct filled with database record fields data.
 */
void* dpcatmodel_fillData_callback (sqlite3_stmt* stmt)
{
	struct dpcatmodel* model = dpcatmodel_create_data ();
	if (model) {
		model->id = sqlite3_column_int (stmt, DP_CAT_FIELDINDEX_DP_CAT_ID);
		// Category name
		model->name = strdup ((char*)sqlite3_column_text (stmt, DP_CAT_FIELDINDEX_DP_CAT_NAME));
		// Category name
		model->desc = strdup ((char*)sqlite3_column_text (stmt, DP_CAT_FIELDINDEX_DP_CAT_DESC));
	}

	return model;
}

/*
 * Releases data from memory.
 */
void dpcatmodel_destroy_model (struct dpcatmodel* model)
{
	if (model) {
		free (model->name);
		free (model->desc);
		free (model);
	}
}

/*
 * Releases data from memory.
 */
void dpcatmodel_freeData_callback (void* data)
{
	dpcatmodel_destroy_model ((struct dpcatmodel*) data);
}

/*
 * Loads categories from database.
 * Returns list of records loaded from database.
 */
struct tableRowList* dpcatmodel_load_categories (const char* filePath, char** retErrMsg)
{
	char* sql = SQLITE_DPCAT_QUERY;
	struct tableRowList* result =
			base_execute_select (sql, filePath,
								 dpcatmodel_fillData_callback,
								 dpcatmodel_freeData_callback,
								 retErrMsg);
	return result;
}

/*
 * Function to test model.
 */
void dpcatmodel_test (const char* databasePath) {
	char* retErrMsg = 0;

	printf (MESSAGES_TESTING_CATEGORIES_DATA_MODEL, "dpcatmodel");
	struct tableRowList* rows = dpcatmodel_load_categories (databasePath, &retErrMsg);

	if (!retErrMsg || (strlen(retErrMsg) == 0)) {
		char msg[50];
		sprintf(msg, MESSAGES_ROWS_LOADED, rows->size);

		struct tableRow* row = rows->first;
		for (int i = 0; i < rows->size; ++i) {
			struct dpcatmodel* cat = (struct dpcatmodel*)(row->data);
			printf (MESSAGES_CATEGORY_ID, cat->id);
			printf (MESSAGES_CATEGORY_NAME, cat->name);
			printf (MESSAGES_CATEGORY_DESCRIPTION, cat->desc);
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
