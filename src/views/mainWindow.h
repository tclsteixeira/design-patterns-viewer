/*
 * mainWindow.h
 *
 *  Created on: 22/12/2023
 *      Author: Tiago C. Teixeira
 * Description: C headers for views main window.
 */

#ifndef VIEWS_MAINWINDOW_H_
	#define VIEWS_MAINWINDOW_H_
	#include <gtk/gtk.h>
	#include "../viewinterface.h"

	/*
	 * Creates the applicatiion main window.
	 * Note: Tipicaly can be invoked in the GtkApplication 'activate' signal
	 * 		 event handler.
	 */
	void mainWindow_create_main_window (GtkApplication* app, struct viewInterface* wi,
										char* dbFullPath);


#endif /* VIEWS_MAINWINDOW_H_ */
