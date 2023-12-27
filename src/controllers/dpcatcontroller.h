/*
 * dpcatcontroller.h
 *
 *  Created on: 26/11/2023
 *      Author: Tiago C. Teixeira
 * Description: C Headers for design patterns controler.
 */

#ifndef CONTROLLERS_DPCATCONTROLLER_H_
	#define CONTROLLERS_DPCATCONTROLLER_H_

	#include "../models/dpcatmodel.h"
#include "../viewinterface.h"

	/* columns */
	enum
	{
	  ID_COLUMN = 0,
	  NAME_COLUMN,
	  DESC_COLUMN,
	  FREQ_USAGE_COLUMN,
	  FREQ_USAGE_DESC_COLUMN,
	  DEPTH,
	  NUM_COLUMNS
	};

	struct treeItem {
		struct dpcatmodel* cat;
		struct tableRowList* patterns;
	};

	/*
	 * Updates the tree view with categories and design paterns loaded from database.
	 */
	void dpcatcontroller_refreshTreeView (const char* databasePath, struct viewInterface* wi, char** retErrMsg);

#endif /* CONTROLLERS_DPCATCONTROLLER_H_ */
