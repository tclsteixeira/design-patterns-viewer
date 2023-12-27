/*
 * commonerrmsg.h
 *
 *  Created on: 20/12/2023
 *      Author: Tiago C. Teixeira
 * Description: Stores text and ranslated messages used by the application.
 */

#ifndef MESSAGES_H_
	#define MESSAGES_H_
	#include "gettext.h"

	#define MESSAGES_MEMORY_ALLOC_ERROR _("Memory error: failed to allocate memory for '%s'!")
	#define MESSAGES_TESTING_CODE_DATA_MODEL _("Testing design patterns code data model '%s'\n")
	#define MESSAGES_ROWS_LOADED _("%i row(s) loaded.\n")
	#define MESSAGES_CODE_ID _("Code Id: %d\n")
	#define MESSAGES_DP_ID _("Design pattern Id: %d\n")
	#define MESSAGES_LANG_ID _("Language Id: %d\n")
	#define MESSAGES_LANG _("Language: %s\n")
	#define MESSAGES_CODE _("Code:\n%s\n")
	#define MESSAGES_SUCCEEDED _("Succeeded: %s\n")
	#define MESSAGES_ERROR _("Error: %s\n")
	#define MESSAGES_TESTING_CATEGORIES_DATA_MODEL _("Testing design patterns categories data model '%s'\n")
	#define MESSAGES_CATEGORY_ID _("Category Id: %d\n")
	#define MESSAGES_CATEGORY_NAME _("Category Name: %s\n")
	#define MESSAGES_CATEGORY_DESCRIPTION _("Category Description: %s\n")
	#define MESSAGES_TESTING_USAGE_CASES_DATA_MODEL _("Testing design patterns usage cases data model '%s'\n")
	#define MESSAGES_UCASE_ID _("Usage case Id: %d\n")
	#define MESSAGES_DESIGNATION _("Designation: %s\n")
	#define MESSAGES_DESCRIPTION _("Description:\n%s\n")
	#define MESSAGES_TESTING_PARTICIPANTS_DATA_MODEL _("Testing design patterns participants data model '%s'\n")
	#define MESSAGES_PARTICIPANT_ID _("Participant Id: %d\n")
	#define MESSAGES_PARTICIPANT_NAME _("Participant name: %s\n")
	#define MESSAGES_TESTING_DESIGN_PATTERNS_DATA_MODEL _("Testing design patterns data model '%s'\n")
	#define MESSAGES_PATTERN_NAME _("Pattern Name: %s\n")
	#define MESSAGES_FREQ_USAGE	_("Freq. usage: %i - ")

	#define MESSAGES_THEME_NAME_SYSTEM _("System")
	#define MESSAGES_THEME_NAME_DARK _("Dark")
	#define MESSAGES_THEME_NAME_LIGHT _("Light")

	#define MESSAGES_ABOUT _("About")
	#define MESSAGES_THEME_AND_SPACE _("Theme: ")
	#define MESSAGES_ERROR_TITLE _("Error")
	#define MESSAGES_CODE_EXAMPLE _("Code example")
	#define MESSAGES_FREQUENCY_USE_AND_SPACE _("Frequency of use: ")
	#define MESSAGES_DESCRIPTION_WORD _("Description")
	#define MESSAGES_PARTICIPANTS_WORD _("Participants")
	#define MESSAGES_USAGE_CASES_WORD _("Usage Cases")

	#define MESSAGES_PATTERNS_WORD _("Patterns")
	#define MESSAGES_DATABASE_FILE_NOT_FOUND _("Database file not found at '%s'!\n")


//	#define MESSAGES_ = _("")
//	#define MESSAGES_ = _("")
//	#define MESSAGES_ = _("")


#endif /* MESSAGES_H_ */
