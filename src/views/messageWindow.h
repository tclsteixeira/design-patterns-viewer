/*
 * messageWindow.h
 *
 *  Created on: 20/11/2023
 *      Author: Tiago C. Teixeira
 * Description: Utility functions to simplify displaying simple message windows
 * 				like error messages.
 */

#ifndef VIEWS_MESSAGEWINDOW_H_
	#define VIEWS_MESSAGEWINDOW_H_

	/*
	 * Displays an error message dialog.
	 */
	void messageWindow_show_error (GtkWindow* parentWindow,
							   	   const char* title, const char* message);

	/*
	 * Displays an info message dialog.
	 */
	void messageWindow_show_info (GtkWindow* parentWindow,
								  const char* title, const char* message);


#endif /* VIEWS_MESSAGEWINDOW_H_ */
