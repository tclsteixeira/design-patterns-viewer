/*
 * dpucasesmodel.h
 *
 *  Created on: 01/12/2023
 *      Author: Tiago C. Teixeira
 * Description: C headers for design pattern usage cases model.
 */

#ifndef MODELS_DPUCASEMODEL_H_
	#define MODELS_DPUCASEMODEL_H_
	#include "records.h"

		// Database query field indexes
	#define UCASES_FIELD_INDEX_DP_UCASE_ID 		0
	#define UCASES_FIELD_INDEX_DP_UCASE_DESIG 	1
	#define UCASES_FIELD_INDEX_DP_UCASE_DESC 	2
	#define UCASES_FIELD_INDEX_DP_ID 			3

	// Database query field names
	#define UCASES_FIELD_DP_UCASE_ID 	"DP_UCASE_ID"
	#define UCASES_FIELD_DP_UCASE_DESIG "DP_UCASE_DESIG"
	#define UCASES_FIELD_DP_UCASE_DESC 	"DP_UCASE_DESC"
	#define UCASES_FIELD_DP_ID 			"DP_ID"

	// Declares the model structure
	struct dpucasemodel {
		int id;
		char* desig;
		char* desc;
		int dpId;
	};

	/*
	 * Releases the model resources.
	 */
	void dpucasemodel_destroy_model (struct dpucasemodel* model);

	/*
	 * Loads model content for a given design pattern from database.
	 * Returns list of records loaded from database.
	 */
	struct tableRowList* dpucasemodel_load_cases (const char* filePath, int dpId, char** retErrMsg);

	/*
	 * Function to test model.
	 */
	void dpucasemodel_test (const char* databasePath);



#endif /* MODELS_DPUCASEMODEL_H_ */
