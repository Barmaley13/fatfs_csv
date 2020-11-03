#include <ff.h>
#include <csv.h>
#include "log.h"

FATFS	fat;
FIL		file;


int main ()
{
	FRESULT res;
	const TCHAR *file_path = "0:/csv/test.csv";
	const char *new_header = "Test";

	// Mount drive
	log_debug("Mounting SD card");
	res = f_mount(&fat, "", 1);
	if (res != FR_OK)
	{
		log_error("f_mount pb: %d", res);
		return;
	}
	
	res = f_open(&file, file_path, FA_OPEN_EXISTING | FA_READ | FA_WRITE);
	if (res != FR_OK)
	{
		log_error("f_open pb: %d", res);
	}

	log_info("Creating buffer");
	CSV_BUFFER *buffer = csv_create_buffer();
	
	log_info("Loading csv");
	csv_load(buffer, &file);
	
	// Print buffer
	uint32_t i, j;
	for (i = 0; i < buffer->rows; i++)
	{
		for (j = 0; j < buffer->width[i]; j++)
		{
			//printf("%-10s\t", buffer->field[i][j]->text);
			log_info("buff[%d][%d] = %s", i, j, buffer->field[i][j]->text);
		}
		//printf("\n");
	}
	
	// Let try overwriting one of the header fields
	log_info("Saving csv");
	csv_set_field(buffer, 0, 1, (char *) new_header);
	csv_save(&file, buffer);

	log_info("Destroying buffer");
	csv_destroy_buffer(buffer);
	
	// Close file
	res = f_close(&file);
	if (res != FR_OK)
	{
		log_error("f_close pb: %d", res);
	}

	while (1);
}
