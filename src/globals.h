/*
 * globals.h
 *
 *  Created on: 17/11/2023
 *      Author: Tiago C. Teixeira
 * Description: Declares global symbols and functions.
 */

#ifndef GLOBALS_H_
	#define GLOBALS_H_
	#include <stdbool.h>
	#include "gettext.h"

	#define APP_TITLE _("Design Patterns Viewer")
	#define APP_AUTHOR "Tiago C. Teixeira\0"
	#define APP_COPYRIGHT " 2023, Tiago C. Teixeira"
	#define APP_VERSION "0.01"
	#define APP_DATABASE_NAME "dpviewerdb.sqlite"
	#define APP_DB_PROVIDER "Sqlite3"
	#define APP_DEVTOOL "Eclipse 2023 CDT"
	#define APP_COMMENTS_1 _("database engine")
	#define APP_COMMENTS_2 _("Developed with")
	#define APP_LOGO "logo.svg"

	/*
	 * Gets the full path of database file as a new string.
	 * Returns the full databse file path if succeeded, NULL otherwise.
	 * Note: Returned string should be released later with free().
	 */
	char* globals_get_db_full_path_new ();

	/*
	 * Gets a new string with the app comments notice.
	 * Note: Result string should be free later.
	 */
	char* globals_get_app_comments_new ();

	/*
	 * Gets a new string with the app copyright notice.
	 * Note: Result string should be free later.
	 */
	char* globals_get_app_copyright_new ();

	/*
	 * Checks if there is an error message.
	 */
	bool globals_hasError(char** retErrMsg);

#endif /* GLOBALS_H_ */
