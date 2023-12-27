/*
 * dpcodemodel.h
 *
 *  Created on: 26/11/2023
 *      Author: Tiago C. Teixeira
 * Description: C headers for code model.
 */

#ifndef MODELS_DPCODEMODEL_H_
	#define MODELS_DPCODEMODEL_H_

	// Database query field indexes
	#define CODE_FIELDINDEX_DP_ID1 			0
	#define CODE_FIELDINDEX_CODE_ID 		1
	#define CODE_FIELDINDEX_CONTENT 		2
	#define CODE_FIELDINDEX_PROG_LANG_ID 	3
	#define CODE_FIELDINDEX_PROG_LANG_NAME 	4

	// Database query field names
	#define CODE_FIELD_DP_ID1 			"DP_ID1"
	#define CODE_FIELD_CODE_ID 			"CODE_ID"
	#define CODE_FIELD_CONTENT 			"CONTENT"
	#define CODE_FIELD_PROG_LANG_ID 	"PROG_LANG_ID"
	#define CODE_FIELD_PROG_LANG_NAME 	"PROG_LANG_NAME"

	// Declares the code model structure
	struct dpcodemodel {
		int id;
		char* content;
		int dpId;
		int langId;
		char* langName;
	};

	/*
	 * Loads code content for a given design pattern the from database.
	 * Returns list of records loaded from database.
	 */
	struct tableRowList* dpcodemodel_load_code (const char* filePath, int dpId, char** retErrMsg);

	/*
	 * Function to test model.
	 */
	void dpcodemodel_test (const char* databasePath);

#endif /* MODELS_DPCODEMODEL_H_ */
