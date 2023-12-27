/*
 * dppartmodel.h
 *
 *  Created on: 29/11/2023
 *      Author: Tiago C. Teixeira
 * Description: C headers for the design patterns participants data model.
 */

#ifndef MODELS_DPPARTMODEL_H_
	#define MODELS_DPPARTMODEL_H_

	// Database query field indexes
	#define PARTICIPANTS_FIELD_INDEX_DP_PART_ID 	0
	#define PARTICIPANTS_FIELD_INDEX_DP_PART_NAME 	1
	#define PARTICIPANTS_FIELD_INDEX_DP_PART_DESC 	2
	#define PARTICIPANTS_FIELD_INDEX_DP_ID 			3

	// Database query field names
	#define PARTICIPANTS_FIELD_DP_PART_ID 	"DP_PART_ID"
	#define PARTICIPANTS_FIELD_DP_PART_NAME "DP_PART_NAME"
	#define PARTICIPANTS_FIELD_DP_PART_DESC "DP_PART_DESC"
	#define PARTICIPANTS_FIELD_DP_ID 		"DP_ID"

	// Declares the participants model structure
	struct dppartmodel {
		int id;
		char* name;
		char* desc;
		int dpId;
	};

	/*
	 * Releases the model resources.
	 */
	void dppartmodel_destroy_model (struct dppartmodel* model);

	/*
	 * Loads participants content for a given design pattern the from database.
	 * Returns list of records loaded from database.
	 */
	struct tableRowList* dppartmodel_load_parts (const char* filePath, int dpId, char** retErrMsg);

	/*
	 * Function to test model.
	 */
	void dppartmodel_test (const char* databasePath);

#endif /* MODELS_DPPARTMODEL_H_ */
