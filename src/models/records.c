/*
 * records.c
 *
 *  Created on: 19/11/2023
 *      Author: Tiago C. Teixeira
 * Description: Implements a table row and a list of table rows to store the
 * 				database query results.
 */

#include <stdio.h>
#include <stdlib.h>
#include "records.h"
#include "../gettext.h"
#include "../messages.h"

/*
 * Creates a table row instance.
 */
struct tableRow* records_create_table_row (void* data)
{
	struct tableRow* res = (struct tableRow*)malloc(sizeof(*res));
	if (!res) {
		fprintf(stderr, MESSAGES_MEMORY_ALLOC_ERROR, "tableRow struct");
		abort();
	}

	res->data = data;
	res->next = NULL;
	return res;
}

/*
 * Creates a table row list instance.
 */
struct tableRowList* records_create_table_row_list (freeDataFunc freedata)
{
	struct tableRowList* res = (struct tableRowList*)malloc (sizeof(*res));
	if (!res) {
		fprintf (stderr, MESSAGES_MEMORY_ALLOC_ERROR, "tableRowList struct");
		abort ();
	}

	res->first = res->last = NULL;
	res->size = 0;
	res->freedata = freedata;
	return res;
}

/*
 * Appends a new row filled with data at the end of table rows list.
 * Returns total number of records in the list,
 */
int records_list_append (struct tableRowList* list, void* data)
{
	struct tableRow* row = records_create_table_row (data);
	if (!(list->first))	// empty list?
		list->first = row;
	else
		list->last->next = row;

	list->last = row;
	(list->size)++;

	return list->size;
}

/*
 * Releases list resources and frees memory.
 */
void records_list_destroy (struct tableRowList* list)
{
	struct tableRow* current = list->first;
	struct tableRow* prev = NULL;
	while (current) {
		if ((list->freedata) && (current->data))
			list->freedata (current->data);

		prev = current;
		current = current->next;
		free (prev);
	}

	free (list);
}
