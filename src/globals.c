/*
 * globals.c
 *
 *  Created on: 27/11/2023
 *      Author: Tiago C. Teixeira
 * Description: Implements global functions.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "globals.h"
#include "messages.h"

/*
 * Checks if there is an error message.
 */
bool globals_hasError(char** retErrMsg) {
	return ((retErrMsg) && (strlen (*retErrMsg) > 0));
}

/*
 * Gets a new string with the app copyright notice.
 * Note: Result string should be free later.
 */
char* globals_get_app_copyright_new () {
	const char* st1 = _("Copyright");
	const char* st2 = APP_COPYRIGHT;
	int len = strlen (st1) + strlen (st2) + 1;
	char* result = (char*)malloc (sizeof(char) * len);
	result[0] = 0;
	result = strcat (result, st1);
	result = strcat (result, st2);
	result = strcat (result, "\0");
	return result;
}

/*
 * Gets a new string with the app comments notice.
 * Note: Result string should be free later.
 */
char* globals_get_app_comments_new () {
	const char* st1 = APP_DB_PROVIDER;
	const char* stSpace = " ";
	const char* st2 = APP_COMMENTS_1;
	const char* stNewLine = "\n";
	const char* st3 = APP_COMMENTS_2;
	const char* st4 = APP_DEVTOOL;

	int len = strlen (st1) + strlen (stSpace) + strlen (st2) +
			  strlen (stNewLine) + strlen (st3) + strlen (stSpace) + strlen (st4) + 1;
	char* result = (char*)malloc (sizeof(char) * len);
	result[0] = 0;
	result = strcat (result, st1);
	result = strcat (result, stSpace);
	result = strcat (result, st2);

	result = strcat (result, stNewLine);

	result = strcat (result, st3);
	result = strcat (result, stSpace);
	result = strcat (result, st4);

	result = strcat (result, "\0");
	return result;
}


