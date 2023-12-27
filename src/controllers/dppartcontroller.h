/*
 * dppartcontroller.h
 *
 *  Created on: 29/11/2023
 *      Author: Tiago C. Teixeira
 * Description: C headers for the design pattern participants controller.
 */

#ifndef CONTROLLERS_DPPARTCONTROLLER_H_
	#define CONTROLLERS_DPPARTCONTROLLER_H_

	#include "../viewinterface.h"

	/*
	 * Updates the participants text view with design paterns participants from database.
	 */
	void dppartcontroller_refresh_Participants (const char* databasePath, int dpId,
												struct viewInterface* wi,
												char** retErrMsg);


#endif /* CONTROLLERS_DPPARTCONTROLLER_H_ */
