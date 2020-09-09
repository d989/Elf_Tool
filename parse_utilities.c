#include <stdio.h>
#include <stdlib.h>
#include <linux/elf.h>
#include <string.h>
#include "parse_utilities.h"
#include "patch_utilities.h"
#include "general_utilities.h"

char *get_section_name_table(char *file_buffer)
{
	SECTION_HEADER *string_table_shdr;
	if (file_buffer == NULL)
			return NULL;

	ELF_HEADER *elf_header = (ELF_HEADER*)file_buffer;
	SECTION_HEADER *section_headers = get_shdr_table(file_buffer);
	if (elf_header->e_shstrndx == SHN_UNDEF)
			return NULL;

	string_table_shdr = section_headers + elf_header->e_shstrndx;
	return(file_buffer + string_table_shdr->sh_offset);
}

char *get_shdr_name(char *file_buffer, SECTION_HEADER *shdr)
{
	char *name_table = get_section_name_table(file_buffer);
	return name_table + shdr->sh_name;
}

PROGRAM_HEADER *get_phdr_by_type(char *file_buffer, unsigned int p_type)
{
	ELF_HEADER *elf_header = (ELF_HEADER*)file_buffer;
	PROGRAM_HEADER *program_headers = (PROGRAM_HEADER*)(file_buffer + elf_header->e_phoff);
	unsigned short num_pht_entries = elf_header->e_phnum;

	for (int i=0; i < num_pht_entries; i++) {
		if((program_headers + i)->p_type == p_type)
				return (program_headers + i);
	}

	return NULL;
}

PROGRAM_HEADER *get_phdr_table(char *file_buffer)
{
	if (file_buffer == NULL)
			return NULL;

	ELF_HEADER *elf_header = (ELF_HEADER*)file_buffer;
	OFFSET phdr_table_offset = elf_header->e_phoff;
	return (PROGRAM_HEADER*)(file_buffer + phdr_table_offset);
}

SECTION_HEADER *get_shdr_by_type(char *file_buffer, unsigned int sh_type)
{
	ELF_HEADER *elf_header = (ELF_HEADER*)file_buffer;
	SECTION_HEADER *section_headers = (SECTION_HEADER*)(file_buffer + elf_header->e_shoff);
	unsigned short num_sht_entries = elf_header->e_shnum;

	for (int i=0; i < num_sht_entries; i++) {
		if ((section_headers + i)->sh_type == sh_type)
				return (section_headers + i);
	}

	return NULL;
}

SECTION_HEADER *get_shdr_table(char *file_buffer)
{
	if (file_buffer == NULL)
			return NULL;

	ELF_HEADER *elf_header = (ELF_HEADER*)file_buffer;
	OFFSET shdr_table_offset = elf_header->e_shoff;
	return (SECTION_HEADER*)(file_buffer + shdr_table_offset);
}

void *get_section_by_name(char *file_buffer, char *section_name)
{
	ELF_HEADER *elf_header = (ELF_HEADER*)file_buffer;
	SECTION_HEADER *section_headers = get_shdr_table(file_buffer);
	char *strtab = get_section_name_table(file_buffer);

	for (int i=0; i < elf_header->e_shnum; i++) {
		char *name = strtab + (section_headers + i)->sh_name;
		if (!strcmp(section_name, name))
			return(void*)(file_buffer + (section_headers + i)->sh_offset);
	}

	return NULL;
}


