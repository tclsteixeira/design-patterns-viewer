/*
 * filepaths.c
 *
 *  Created on: 28/12/2023
 *      Author: Tiago C. Teixeira
 * Description: Implements functions to obtain common app file paths.
 */

#include <stdlib.h>
#include "globals.h"
#include <stdbool.h>
#include <string.h>
#include "filepaths.h"

#ifdef _WIN32
	#include <io.h>
	#define access _access
	#define F_OK 0
#else
	#include <unistd.h>
#endif

/*
 * Checks a a file exists for a given path.
 */
int filepaths_file_exists (const char *filename) {
    if (access (filename, F_OK) == 0) {
        return 1; // File exists
    } else {
        return 0; // File does not exist
    }
}

#ifdef DEBUG_MODE

/*
 * Gets the relative or full file path as new allocated string.
 * Note: Returned path must be released later with free ().
 */
static char* filepaths_get_file_path_new (const char* rel_path, const char* file_name,
										  const bool rel) {
	char* result = NULL;
	char temp_result[500];

	if (!rel)
		realpath ("./", temp_result);	// it's a full path
	else
		temp_result[0] = 0;

	// build rel path
	if (file_name) {
		strcat (temp_result, rel_path);
		strcat (temp_result, PATH_SEPARATOR);
		strcat (temp_result, file_name);
	}

	result = strdup (temp_result);
	return result;
}

#endif

///*
// *
// */
//void filepaths_extract_parent_dir (const char* path, char* parentDir)
//{
//    // Find the last occurrence of '/' in the path
//    const char* lastSlash = strrchr (path, '/');
//
//    if (lastSlash != NULL) {
//        // Calculate the length of the parent directory (including the last '/')
//        size_t parentDirLength = lastSlash - path + 1;
//
//        // Copy the parent directory to the parentDir buffer
//        strncpy (parentDir, path, parentDirLength);
//        parentDir[parentDirLength] = '\0';
//    } else {
//        // No '/' found, the entire path is considered the parent directory
//        strcpy(parentDir, path);
//    }
//}

/*
 * Gets the full file path of the locale directoty.
 */
char* filepaths_get_locale_dir_path_new () {
#ifdef DEBUG_MODE
	char* result = NULL;
	char projDirName[500];
	projDirName[0] = 0;
	realpath ("./", projDirName);
	strcat (projDirName, FILE_PATHS_LOCALE);
	result = strdup (projDirName);
	return result;
#else
	// '/usr/share/locale'
	return strdup (FILE_PATHS_DISTR_LOCALE_BASE);
#endif
}

/*
 * Gets the full file path of the logo image file.
 */
char* filepaths_get_logo_image_file_path_new (const bool rel) {
#ifdef DEBUG_MODE
	char* result = NULL;
	result = filepaths_get_file_path_new (FILE_PATHS_DEBUG_IMAGES, APP_LOGO, rel);
	return result;
#else
	// '/usr/share/' 'app_name' '/images' '/' 'databasefilename'
	int len = strlen (FILE_PATHS_DISTR_IMAGES_BASE)
			+ strlen (APP_NAME)	// + strlen (PATH_SEPARATOR)
			+ strlen (FILE_PATHS_IMAGES) + strlen (PATH_SEPARATOR)
			+ strlen (APP_LOGO);

	char result[len];
	result[0] = 0;

	strcat (result, FILE_PATHS_DISTR_IMAGES_BASE);
	strcat (result, APP_NAME);
	strcat (result, FILE_PATHS_IMAGES);
	strcat (result, PATH_SEPARATOR);
	strcat (result, APP_LOGO);
	return strdup (result);
#endif
}

/*
 * Gets the full file path of the database file.
 */
char* filepaths_get_db_file_path_new (const bool rel) {
#ifdef DEBUG_MODE
		char* result = NULL;
		result = filepaths_get_file_path_new (FILE_PATHS_DEBUG_DATABASE, APP_DATABASE_NAME, rel);
		return result;
#else
		// '/var/' 'app_name' '/database' '/' 'databasefilename'
		int len = strlen (FILE_PATHS_DISTR_DATABASE_BASE)
				+ strlen (APP_NAME)	// + strlen (PATH_SEPARATOR)
				+ strlen (FILE_PATHS_DATABASE) + strlen (PATH_SEPARATOR)
				+ strlen (APP_DATABASE_NAME);

		char result[len];
		result[0] = 0;

		strcat (result, FILE_PATHS_DISTR_DATABASE_BASE);
		strcat (result, APP_NAME);
		strcat (result, FILE_PATHS_DATABASE);
		strcat (result, PATH_SEPARATOR);
		strcat (result, APP_DATABASE_NAME);
		return strdup (result);
#endif
}

