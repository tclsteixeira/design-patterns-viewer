/*
 * dpucasescontroller.h
 *
 *  Created on: 03/12/2023
 *      Author: Tiago C. Teixeira
 * Description: C headers for the design pattern usage cases controller.
 */

#ifndef CONTROLLERS_DPUCASESCONTROLLER_H_
	#define CONTROLLERS_DPUCASESCONTROLLER_H_
	#include "../viewinterface.h"

	/*
	 * Updates the usage cases text view with design paterns cases from database.
	 */
	void dpucasescontroller_refresh_cases (const char* databasePath, int dpId,
												struct viewInterface* wi,
												char** retErrMsg);
#endif /* CONTROLLERS_DPUCASESCONTROLLER_H_ */
