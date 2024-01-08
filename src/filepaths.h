/*
 * filepaths.h
 *
 *  Created on: 28/12/2023
 *      Author: Tiago C. Teixeira
 * Description: C headers for app files relative locations.
 */

#ifndef FILEPATHS_H_
	#define FILEPATHS_H_
	#include <stdbool.h>

//	#define CONCATENATE(x, y,z) x ## y ## z
//	#define STRINGIZE(x) #x


	#if defined(_WIN32) || defined(_WIN64)
		#define PATH_SEPARATOR "\\"
		#define FILE_PATHS_SOURCES "\\src"
		#define FILE_PATHS_CONF "\\etc"
		#define FILE_PATHS_LOCALE "\\locales"
		#define FILE_PATHS_DATABASE "\\database"
		#define FILE_PATHS_IMAGES "\\images"

		#define FILE_PATHS_DEBUG_DATABASE "../database"
		#define FILE_PATHS_DEBUG_IMAGES "../images"

		#define FILE_PATHS_DISTR_DATABASE_BASE "/xpto"
		#define FILE_PATHS_DISTR_IMAGES_BASE "/xpto"
		#define FILE_PATHS_DISTR_LOCALE_BASE "/xpto"

	#else
		#define PATH_SEPARATOR "/"
		#define FILE_PATHS_SOURCES "/src"
		#define FILE_PATHS_CONF "/etc"
		#define FILE_PATHS_LOCALE "/locale"
		#define FILE_PATHS_DATABASE "/database"

		#define FILE_PATHS_IMAGES "/images"

		#define FILE_PATHS_DEBUG_DATABASE "../database"
		#define FILE_PATHS_DEBUG_IMAGES "../images"

		#define FILE_PATHS_DISTR_DATABASE_BASE "/var/"
		#define FILE_PATHS_DISTR_IMAGES_BASE "/usr/share/"
		#define FILE_PATHS_DISTR_LOCALE_BASE "/usr/share/locale"

	#endif

//	#define FILE_PATHS_LOGO_REL_PATH STRINGIZE(CONCATENATE(FILE_PATHS_IMAGES, PATH_SEPARATOR, APP_LOGO))

	/*
	 * Checks a a file exists for a given path.
	 */
	int filepaths_file_exists (const char *filename);

	/*
	 * Gets the full file path of the logo image file.
	 */
	char* filepaths_get_logo_image_file_path_new (const bool rel);

	/*
	 * Gets the full file path of the database file.
	 */
	char* filepaths_get_db_file_path_new (const bool rel);


#endif /* FILEPATHS_H_ */
