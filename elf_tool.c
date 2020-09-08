#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "parse_utilities.h"
#include "patch_utilities.h"
#include "general_utilities.h"

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

int display_menu(char *file_content, unsigned long file_size)
{
	char option;

	while(1) {
		printf("\n ===== Select Option =====\n"\
			   "1 - Display ELF Header Info\n"\
			   "2 - Display Program Header Info\n"\
			   "3 - Display Section Header Info\n"\
			   "4 - Save File\n"\
			   "q - Quit\n"\
			   "> ");

		option = getchar();
		clear_input_buffer();
		switch(option) {
			case '1':
				print_header(file_content);
				break;
			case '2':
				print_program_headers(file_content);
				break;
			case '3':
				print_section_headers(file_content);
				break;
			case '4':
				save_file(file_content, file_size);
				break;
			case 'q':
				return 0;
			default:
				puts(" Invalid option.");
		}
	}
}

int main(int argc, char **argv) 
{	
	FILE *finout;
	char *elf_filename;
	unsigned long file_size;

	if (argc != 2) {
		puts("Usage: ./elf_tool <ELF file name>");
		return EXIT_FAILURE;
	}

	finout = fopen(elf_filename = argv[1], "r");
	if (finout == NULL) {
		perror("Could not open file");
		return EXIT_FAILURE;
	}

	file_size = get_file_size(finout);
	if (file_size < 0) {
		perror("Could not obtain file size");
		return EXIT_FAILURE;
	}

	char *file_content = calloc(file_size, sizeof(char));
	fread((void*)file_content,\
				   	sizeof(char),\
					file_size,\
				   	finout);
	if (ferror(finout)) {
		puts("Could not read file contents");
		return -1;
	}

	fclose(finout);
	display_menu(file_content, file_size);

	return EXIT_SUCCESS;
}
