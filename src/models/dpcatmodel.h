/*
 * dpcatmodel.h
 *
 *  Created on: 21/11/2023
 *      Author: Tiago C. Teixeira
 * Description: C Headers for categories data model.
 */

#ifndef MODELS_DPCATMODEL_H_
	#define MODELS_DPCATMODEL_H_

	// Database table field indexes
	#define DP_CAT_FIELDINDEX_DP_CAT_ID 0
	#define DP_CAT_FIELDINDEX_DP_CAT_NAME 1
	#define DP_CAT_FIELDINDEX_DP_CAT_DESC 2

	// Database table field names
	#define DP_CAT_FIELD_DP_CAT_ID "DP_CAT_ID"
	#define DP_CAT_FIELD_DP_CAT_NAME "DP_CAT_NAME"
	#define DP_CAT_FIELD_DP_CAT_DESC "DP_CAT_DESC"

//	#define DP_CAT_FIELD_DP_CAT_NAME_NUMCHARS 50
//	#define DP_CAT_FIELD_DP_CAT_DESC_NUMCHARS 500


	// Declares the design pattern category model structure
	struct dpcatmodel {
		int id;
		char* name;
		char* desc;
	};

	/*
	 * Releases data from memory.
	 */
	void dpcatmodel_destroy_model (struct dpcatmodel* model);

	/*
	 * Loads categories from database.
	 * Returns list of records loaded from database.
	 */
	struct tableRowList* dpcatmodel_load_categories (const char* filePath, char** retErrMsg);

	/*
	 * Function to test model.
	 */
	void dpcatmodel_test(const char* databasePath);

#endif /* MODELS_DPCATMODEL_H_ */
