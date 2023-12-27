/*
 * base.h
 *
 *  Created on: 19/11/2023
 *      Author: Tiago C. Teixeira
 */

#ifndef MODELS_BASE_H_
	#define MODELS_BASE_H_

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <stdbool.h>
	#include <sqlite3.h>
	#include "base.h"
	#include "records.h"

	// sqlite3 data types
	#define SQLITE3_DATA_TYPE_INTEGER  	0
	#define SQLITE3_DATA_TYPE_REAL  	1
	#define SQLITE3_DATA_TYPE_TEXT  	2
	#define SQLITE3_DATA_TYPE_BLOB  	3


	// declares the sql parameter structure
	struct sqlParameter {
		int dataType;	// one of the sqlite3 data type macro constants
		void* value;	// parameter value
	};

	/*
	 * Checks if a sqlite returned code is an error code.
	 * Returns '1' if it's an error code, '0' otherwise.
	 */
	int base_haserror(int code);

	/*
	 * Writes error message to stderr.
	 */
	void base_handle_error (char* msg, sqlite3* db);

	/*
	 * Gets the current sqlite version.
	 * Note: output arg 'res' must have enouth space for storing version string.
	 * ex: "3.8.1"
	 */
	int base_sqlite3_version(char* output);

//	/*
//	 * Try to open a database file.
//	 * Returns true if succeeded, false otherwise.
//	 */
//	bool base_open_database (const char* dbFilePath, sqlite3* db, char** retErrMsg);

	/*
	 * Executes a select statment and returns result.
	 * Returns result list of rows if succeeded, NULL otherwise.
	 */
	struct tableRowList* base_execute_select (const char* sqlStr,
											  const char* dbFilePath,
											  fillDataFunc filldatacallback,
											  freeDataFunc freedatacallback,
											  char** retErrMsg);

	/*
	 * Executes a select statment with parameters and returns result.
	 * Returns result list of rows if succeeded, NULL otherwise.
	 */
	struct tableRowList* base_execute_select_params (
									  const char * sqlStr,
									  const char* dbFilePath,
									  struct sqlParameter** parameters,	// array
									  const int numParams, 						// parameters array size
									  fillDataFunc filldatacallback,
									  freeDataFunc freedatacallback,
									  char** retErrMsg);

#endif /* MODELS_BASE_H_ */
