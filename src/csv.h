/*
* Modified: https://github.com/winobes/libcsv
*/

#ifndef _CSV_H
#define _CSV_H

#include <stdlib.h>
#include <stdio.h>
#include <ff.h>

typedef struct CSV_FIELD {
    char *text;
    size_t length;
} CSV_FIELD;

typedef struct CSV_BUFFER {
    CSV_FIELD ***field;
    size_t rows;
    size_t *width;
    char field_delim;
    char text_delim;
} CSV_BUFFER;


/* Function: csv_create_buffer
 * ---------------------------
 * Must be called before any declared buffer is used. 
 * Allocates memory for the buffer.
 * Sets the number of rows to 0
 * Sets Text Delim to '"' and field delim to ',' by default.
 */
CSV_BUFFER *csv_create_buffer();

/* Function: csv_destroy_buffer
 * ----------------------------
 * Frees memory allocated by csv_create_buffer and any fields
 * that are part of the buffer.
 */
void csv_destroy_buffer(CSV_BUFFER *buffer);

/* Function: csv_load
 * -----------------------
 * Loads the given file into the buffer.
 *
 * Returns:
 *  0: success
 *  1: file not found
 *  2: failure to resize buffer (memory failure)
 */
int csv_load(CSV_BUFFER *buffer, FIL *csv_fp);

/* Function: csv_save
 * -----------------------
 * Saves the csv buffer to a given file. If the file already
 * exists, it is overwritten.
 *
 * Returns:
 *  0: success
 *  1: unable to write to file (invalid name or inufficient
 *     access)
 */
int csv_save(FIL *csv_fp, CSV_BUFFER *buffer);

/* Function: csv_copy_row
 * ----------------------
 * Deep copy of a row of a CSV_BUFFER. Destination row may
 * be in the same buffer as source. Adjusts the length of 
 * the row.
 *
 * Returns:
 *  0: success
 *  1: the requested src row does not exist
 */
int csv_copy_row(CSV_BUFFER *dest, int dest_row,
	CSV_BUFFER *src, int src_row);

/* Function: copy_field
 * ------------------------
 * 
 * Copies a given field from one buffer to another (note, source and
 * dest buffers MAY be the same.
 * 
 * Returns:
 *  0: success
 *  1: memory error (see set_field)
 */
int csv_copy_field(CSV_BUFFER *dest, int dest_row, int dest_entry,
    CSV_BUFFER *source, int source_row, int source_entry);


/* Function: csv_get_field
 * ----------------------------------
 * Copies an entry from a CSV_BUFFER to a string provided.
 * The caller is expected to provide the string's length.
 * If the requested cell does not exist, or is empty, the
 * string is filled with null characters.
 *
 * Returns:
 *  0: the whole entry was copied
 *  1: the entry was trucated to fit the string
 *  2: the request cell was empty (or does not exist)
 *  3: the length given was 0
 */
int csv_get_field(char *dest, size_t dest_len, 
	CSV_BUFFER *src, size_t row, size_t entry);

/* Function: csv_clear_field
 * -------------------------
 * 
 * If the field is the last in the row (and not the first), it is
 * removed (the width of the row is decremented by one). Otherwise,
 * it is just cleared, and a spacer remains.
 *
 * Returns:
 *  0: success
 *  TODO errors
 */
int csv_clear_field(CSV_BUFFER *buffer, size_t row, size_t entry);

/* Function: csv_clear_row
 * -----------------------
 * 
 * Destroys all but one field in the row, and clears that field.
 * Row size is now 1.
 *
 * Returns:
 *  0: success
 *  1: memory allocation falirure (note this function only 
 *      reduces memory used, so reallocation should never fail)
 */
int csv_clear_row(CSV_BUFFER *buffer, size_t row);

/* Function: csv_remove_row
 * ------------------------
 * 
 * Completely removes a row from the buffer such that it's two
 * neighboring rows are now adjacent and buffer height is reduced
 * by one.
 *
 * Returns:
 *  0: success
 *  1: memory allocation falirure (note this function only 
 *      reduces memory used, so reallocation should never fail)
 */
int csv_remove_row(CSV_BUFFER *buffer, size_t row);

void csv_set_text_delim(CSV_BUFFER *buffer, char new_delim);

void csv_set_field_delim(CSV_BUFFER *buffer, char new_delim);

int csv_get_height(CSV_BUFFER *buffer);
/* Returns: height of buffer */

int csv_get_width(CSV_BUFFER *bufer, size_t row);
/* Returns: width of row (or 0 if row does not exist) */

int csv_get_field_length(CSV_BUFFER *buffer, size_t row, size_t entry);

int csv_set_field(CSV_BUFFER *buffer, size_t row, size_t entry, char *field);

int csv_insret_field(CSV_BUFFER *buffer, size_t row, size_t entry, char *field);

#endif /* CSV_H_ */
