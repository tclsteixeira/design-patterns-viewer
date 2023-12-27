/*
 * records.h
 *
 *  Created on: 19/11/2023
 *      Author: Tiago C. Teixeira
 */

#ifndef MODELS_RECORDS_H_
	#define MODELS_RECORDS_H_
	#include <sqlite3.h>

	// Declare table row
	struct tableRow {
		void* data;
		struct tableRow* next;
	};

	typedef void* (*fillDataFunc)(sqlite3_stmt* stmt);
	typedef void (*freeDataFunc)(void* data);

	// Declare list of table rows
	struct tableRowList {
		int size;
		struct tableRow* first;
		struct tableRow* last;
		freeDataFunc freedata;
	};

	/*
	 * Creates a table row instance.
	 */
	struct tableRow* records_create_table_row(void* data);

	/*
	 * Creates a table row list instance.
	 */
	struct tableRowList* records_create_table_row_list (freeDataFunc freedata);

	/*
	 * Appends a new row filled with data at the end of table rows list.
	 * Returns total number of records in the list,
	 */
	int records_list_append (struct tableRowList* list, void* data);

	/*
	 * Releases list resources and frees memory.
	 */
	void records_list_destroy(struct tableRowList* list);

#endif /* MODELS_RECORDS_H_ */
