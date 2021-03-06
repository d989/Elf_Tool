#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "general_utilities.h"

int read_input_from_user(char *buffer, size_t buffer_size)
{
	size_t input_len;

	memset((void*)buffer, buffer_size, '\00');
	if (fgets(buffer, buffer_size, stdin) == NULL) {
		return -1;
	}

	input_len = strlen(buffer);
	if (input_len > 0 && buffer[input_len - 1] == '\n')
		buffer[input_len - 1] = '\00';

	return 0;
}

unsigned long get_file_size(FILE *finout)
{
	unsigned long size;
	
	if (fseek(finout, 0L, SEEK_END)) return -1;
	size = ftell(finout);		
	if (fseek(finout, 0L, SEEK_SET)) return -1;

	return size;
}

int check_file_overwrite(char *filename)
{
	char option;
	FILE *file;
	if (file = fopen(filename, "r")) {
		fclose(file);
		do {
			puts("Overwrite existing file? (y/n)");
			option = getchar();
			clear_input_buffer();
		} while (option !='y' && option != 'n');
		if (option == 'n')
			return -1;
	}

	return 0;
}

int save_file(char *file_buffer, unsigned long file_size)
{
	FILE *file_stream;
	static char filename[64];

	do {
		puts("\nPlease enter a name for the file:");
		if (read_input_from_user(filename, sizeof(filename))) {
			perror("Could not read filename");
			return -1;
		}	
	} while (check_file_overwrite(filename));

	file_stream = fopen(filename, "w");
	fwrite(file_buffer, sizeof(char), file_size, file_stream);
	if (ferror(file_stream)) {
		puts("Could not write to file");
		return -1;
	}

	if (fclose(file_stream)) {
		perror("Could not close file");
		return -1;
	}

	puts("File saved successfully.");
	return 0;

}

