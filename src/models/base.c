/*
 * base.c
 *
 *  Created on: 19/11/2023
 *      Author: Tiago C. Teixeira
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <stdbool.h>
#include "base.h"
#include "records.h"
#include "../messages.h"
#include "../gettext.h"

#define BASE_MSG_CANNOT_OPEN_DATABASE 	_("Cannot open database")
#define BASE_MSG_FAILED_FETCH_DATA 		_("Failed to fetch data")
#define BASE_MSG_PAR_TYPE_NOT_SUPPORTED _("The parameter type '%s' is not supported in 'Select' queries.")
#define BASE_MSG_UNKNOWN_PAR_TYPE 		_("Unknown parameter type '%i'.")

#ifdef _WIN32
	#include <io.h>
	#define access _access
	#define F_OK 0
#else
	#include <unistd.h>
#endif

/*
 * Checks a a file exists for a given path.
 */
static int file_exists (const char *filename) {
    if (access (filename, F_OK) == 0) {
        return 1; // File exists
    } else {
        return 0; // File does not exist
    }
}

/*
 * Checks if a sqlite returned code is an error code.
 * Returns '1' if it's an error code, '0' otherwise.
 */
int base_haserror(int code) {
	return (code != SQLITE_OK);
}

/*
 * Writes error message to stderr.
 */
void base_handle_error (char* msg, sqlite3* db) {
	fprintf(stderr, "%s: %s\n", msg, sqlite3_errmsg(db));
}

/*
 * Gets the current sqlite version.
 * Note: output arg 'res' must have enouth space for storing version string.
 * ex: "3.8.1"
 */
int base_sqlite3_version(char* output) {
	sqlite3 *db;
	sqlite3_stmt *res;

	int rc = sqlite3_open(":memory:", &db);
	if (rc != SQLITE_OK) {
		base_handle_error (BASE_MSG_CANNOT_OPEN_DATABASE, db);
		sqlite3_close(db);
		return 0;
	}

	rc = sqlite3_prepare_v2 (db, "SELECT SQLITE_VERSION()", -1, &res, 0);

	if (rc != SQLITE_OK) {
		base_handle_error (BASE_MSG_FAILED_FETCH_DATA, db);
		sqlite3_close(db);
		return 0;
	}

	rc = sqlite3_step (res);	// execute statment

	// only one row expected
	if (rc == SQLITE_ROW) {
//		int a = strlen("");
		size_t len = strlen ((char*)(sqlite3_column_text(res, 0)));
		memcpy(output, sqlite3_column_text(res, 0), len * sizeof(char));
		// append null terminater at end
		output[len] = 0;
	}

	// free resoueces
	sqlite3_finalize(res);
	sqlite3_close(db);

	return 1;	// return true
}

/*
 * Executes a select statment and returns result.
 * Returns result list of rows if succeeded, NULL otherwise.
 */
struct tableRowList* base_execute_select (const char * sqlStr,
										  const char* dbFilePath,
										  fillDataFunc filldatacallback,
										  freeDataFunc freedatacallback,
										  char** retErrMsg)
{
	struct tableRowList* result = NULL;
	sqlite3* db;
	sqlite3_stmt* stat = NULL;

	if (!file_exists (dbFilePath)) {
		int len =  strlen (MESSAGES_DATABASE_FILE_NOT_FOUND)
				+ strlen (dbFilePath) - 1;
		fprintf (stderr, MESSAGES_DATABASE_FILE_NOT_FOUND, dbFilePath);
//		retErrMsg = (char**) malloc (sizeof (char**));
		*retErrMsg = (char*)malloc (len * sizeof (char));
		sprintf (*retErrMsg, MESSAGES_DATABASE_FILE_NOT_FOUND, dbFilePath);
		return NULL;
//		abort();
	}

	// this is anoying but just realize that the open statment must be in same
	// function as prepare_v2 otherwise 'out of memory' error will be raised. :(
	int rc = sqlite3_open(dbFilePath, &db);
//	free(path);	// not needed anymore.

	if (rc != SQLITE_OK) {
		base_handle_error (BASE_MSG_CANNOT_OPEN_DATABASE, db);
		sqlite3_close(db);
		return 0;
	}

	rc = sqlite3_prepare_v2 (db, sqlStr, -1, &stat, 0);

	if (rc != SQLITE_OK) {
		*retErrMsg = strdup ((char*)sqlite3_errmsg (db));
		sqlite3_close(db);
		return result;
	}

	// goto to first row
	rc = sqlite3_step(stat);
//	int i = -1;
	result = records_create_table_row_list (freedatacallback);

	// iterate row by row
	while (rc == SQLITE_ROW) {
		void* data = filldatacallback (stat);
		records_list_append (result, data);

		// goto to next row
		rc = sqlite3_step(stat);
	}

	sqlite3_finalize (stat);
	sqlite3_close(db);

	return result;
}

/*
 * Executes a select statment with parameters and returns result.
 * Returns result list of rows if succeeded, NULL otherwise.
 */
struct tableRowList* base_execute_select_params (
								  const char * sqlStr,
								  const char* dbFilePath,
								  struct sqlParameter** parameters,	// parameters array
								  const int numParams, 				// parameters array size
								  fillDataFunc filldatacallback,
								  freeDataFunc freedatacallback,
								  char** retErrMsg)
{
	struct tableRowList* result = NULL;
	sqlite3* db;
	sqlite3_stmt* stat = NULL;

	// this is anoying but just realize that the open statment must be in same
	// function as prepare_v2 otherwise 'out of memory' error will be raised. :(
	int rc = sqlite3_open (dbFilePath, &db);
//	free(path);	// not needed anymore.

	if (rc != SQLITE_OK) {
		base_handle_error (BASE_MSG_CANNOT_OPEN_DATABASE, db);
		sqlite3_close(db);
		return 0;
	}

	// Compile sql statment
	rc = sqlite3_prepare_v2 (db, sqlStr, -1, &stat, 0);

	if (rc != SQLITE_OK) {
		*retErrMsg = strdup ((char*)sqlite3_errmsg (db));
		goto TERMINATE;
	}

	struct sqlParameter* par = NULL;
	// bind parameters
	for (int i = 0; i < numParams; ++i) {
		par = parameters[i];

		switch (par->dataType)
		{
			case SQLITE3_DATA_TYPE_INTEGER:
				sqlite3_bind_int (stat, i+1, *((int*)(par->value)));
				break;
			case SQLITE3_DATA_TYPE_REAL:
				sqlite3_bind_double (stat, i+1, *(double*)(par->value));
				break;
			case SQLITE3_DATA_TYPE_TEXT:
				char* txt = par->value;
				int len = strlen (txt);
				sqlite3_bind_text (stat, i+1, (char*)(par->value), len, NULL);
				break;
			case SQLITE3_DATA_TYPE_BLOB:
				sprintf (*retErrMsg, BASE_MSG_PAR_TYPE_NOT_SUPPORTED, "BLOB");
				goto TERMINATE;
			default:
				sprintf (*retErrMsg, BASE_MSG_UNKNOWN_PAR_TYPE, par->dataType);
				goto TERMINATE;
		}	// switch ...
	}	// for ...

	// goto to first row
	rc = sqlite3_step (stat);
	result = records_create_table_row_list (freedatacallback);

	// iterate row by row
	while (rc == SQLITE_ROW) {
		void* data = filldatacallback (stat);
		records_list_append (result, data);

		// goto to next row
		rc = sqlite3_step (stat);
	}

TERMINATE:
	sqlite3_finalize (stat);
	sqlite3_close (db);
	return result;
}
