/*
 * gettext.h
 *
 *  Created on: 18/12/2023
 *      Author: Tiago C. Teixeira
 * Description: Includes translation header libs and macros to use with gettext.
 */

#ifndef GETTEXT_H_
	#define GETTEXT_H_

	// translation libs
	#include <libintl.h>
	#include <locale.h>

	// Macro for standard mark of text to be translated _()
	// This is a common way to mark localized strings.
	#define _(String) gettext(String)

#endif /* GETTEXT_H_ */
