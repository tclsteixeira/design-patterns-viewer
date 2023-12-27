/*
 * dpcodecontroller.h
 *
 *  Created on: 27/11/2023
 *      Author: Tiago C. Teixeira
 * Description: C Headers for design patterns code controller.
 */

#ifndef CONTROLLERS_DPCODECONTROLLER_H_
	#define CONTROLLERS_DPCODECONTROLLER_H_

	/*
	 * Updates the description content.
	 */
	void dpcodecontroller_update_description (struct viewInterface* wi, const char* desc);

	/*
	 * Updates the source view content with design paterns code examples for a given
	 * design pattern Id from database.
	 */
	void dpcatcontroller_refreshSourceView (const char* databasePath, int dpId, struct viewInterface* wi, char** retErrMsg);


#endif /* CONTROLLERS_DPCODECONTROLLER_H_ */
