/*
 * dpmodel.h
 *
 *  Created on: 22/11/2023
 *      Author: Tiago C. Teixeira
 * Description: C headers for Design Patterns data model.
 */

#ifndef MODELS_DPMODEL_H_
	#define MODELS_DPMODEL_H_
	#include "../gettext.h"

	// Database table field indexes
	#define DP_FIELD_INDEX_DP_ID 				0
	#define DP_FIELD_INDEX_DP_NAME 				1
	#define DP_FIELD_INDEX_DP_CAT_ID 			2
	#define DP_FIELD_INDEX_DP_DESC 				3
	#define DP_FIELD_INDEX_DP_FREQ_USAGE 		4
	#define DP_FIELD_INDEX_DP_FREQ_USAGE_DESC 	5


	// Database table field names
	#define DP_FIELD_DP_ID 				"DP_ID"
	#define DP_FIELD_DP_NAME 			"DP_NAME"
	#define DP_FIELD_DP_CAT_ID 			"DP_CAT_ID"
	#define DP_FIELD_DP_DESC 			"DP_DESC"
	#define DP_FIELD_DP_FREQ_USAGE 		"FREQ_USAGE"
	#define DP_FIELD_DP_FREQ_USAGE_DESC "FREQ_USAGE_DESC"

	#define DP_FREQ_LEVEL_MIN 1		// minimum level
	#define DP_FREQ_LEVEL_MAX 5		// maximum level

	#define DP_FREQ_DESC_LOW 		 _("Low")
	#define DP_FREQ_DESC_MEDIUM_LOW  _("Medium-low")
	#define DP_FREQ_DESC_MEDIUM 	 _("Medium")
	#define DP_FREQ_DESC_MEDIUM_HIGH _("Medium-high")
	#define DP_FREQ_DESC_HIGH 		 _("High")


//	char** dpmodel_freqUsageDescArr = { _("Low"), _("Medium-low"), _("Medium"), _("Medium-high"), _("High") };

	// Declares the design pattern model structure
	struct dpmodel {
		int id;
		char* name;
		char* desc;
		int catId;
		int freqUsage;
		char* freqUsageDesc;
	};

	/*
	 * Gets the frequency usage description for a given frequency level.
	 * Returns frequency level description if succeeded, empty string otherwise.
	 * Note: the returned string must be released later with free ().
	 */
	char* dpmodel_get_frequency_desc (const int freqLevel);

	/*
	 * Releases data from memory.
	 */
	void dpmodel_destroy_model (struct dpmodel* model);

	/*
	 * Loads patterns associated with a given category Id from the database.
	 * Returns list of records loaded from database.
	 */
	struct tableRowList* dpmodel_load_patterns (const char* filePath, int catId, char** retErrMsg);

	/*
	 * Function to test model.
	 */
	void dpmodel_test(const char* databasePath);

#endif /* MODELS_DPMODEL_H_ */

