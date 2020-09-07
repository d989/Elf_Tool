#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "parse_utilities.h"

FILE *finout;
unsigned long file_size;
static char option;

unsigned long get_file_size(FILE *finout)
{
	unsigned long size;
	
	if(fseek(finout, 0L, SEEK_END) == 0)
		size = ftell(finout);
	else
		return -1;
		
	if(fseek(finout, 0L, SEEK_SET) != 0)
		return -1;

	return size;
}

int display_menu(char *file_content)
{
	while(1) {
		printf("\n ===== Select Option =====\n"\
			   "1 - Display ELF Header Info\n"\
			   "2 - Display Program Header Info\n"\
			   "3 - Display Section Header Info\n"\
			   "q - Quit\n"\
			   "> ");

		option = getchar();
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
			case 'q':
				return 0;
			default:
				puts(" Invalid option.");
		}
		while((option = getchar()) != '\n' && option != EOF);
	}
}

int main(int argc, char **argv) 
{	
	if(argc != 2) {
		puts("Usage: ./elf_tool <ELF file name>");
		return EXIT_FAILURE;
	}

	finout = fopen(argv[1], "r");

	if(finout == NULL) {
		perror("Could not open file");
		return EXIT_FAILURE;
	}

	file_size = get_file_size(finout);

	if(file_size < 0) {
		perror("Could not obtain file size");
		return EXIT_FAILURE;
	}

	char *file_content = calloc(file_size, sizeof(char));
	
	fread((void*)file_content,\
				   	sizeof(char),\
					file_size,\
				   	finout);

	display_menu(file_content);

	return EXIT_SUCCESS;
}
