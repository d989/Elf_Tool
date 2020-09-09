#include <stdio.h>
#include <stdlib.h>
#include <linux/elf.h>
#include <string.h>
#include "parse_utilities.h"
#include "patch_utilities.h"
#include "general_utilities.h"

int print_segment_permissions(unsigned int flags)
{
	print_field("Segment perms");
	(flags & PF_R) ? printf("r") : printf("-");
	(flags & PF_W) ? printf("w") : printf("-");
	(flags & PF_X) ? puts("x") : puts("-");
	
	return 0;
}

int print_section_flags(unsigned int flags) 
{
	if ((SHF_WRITE | SHF_ALLOC | SHF_EXECINSTR) & flags) {
		print_field("Section flags");
		if (flags & SHF_WRITE) printf("Writable\t"); 
		if (flags & SHF_ALLOC) printf("Memory-Resident\t"); 
		(flags & SHF_EXECINSTR) ? puts("Instructions") : puts("");
	}

	return 0;
}

int print_sym_binding(unsigned char symtab_entry)
{
	unsigned char sym_binding = xtract_sym_binding(symtab_entry);
	print_field("Symbol Binding");
	switch(sym_binding) {
		case STB_LOCAL:
			puts("Local");
			break;
		case STB_GLOBAL:
			puts("Global");
			break;
		case STB_WEAK:
			puts("Weak");
			break;
		default:
			puts("*** Unrecognised Value ***");
			return -1;
	}

	return 0;
}

int print_sym_type(unsigned char symtab_entry)
{
	unsigned char sym_type = xtract_sym_type(symtab_entry);
	print_field("Symbol Type");
	switch(sym_type) {
		case STT_NOTYPE:
				puts("Undefined");
				break;
		case STT_OBJECT:
				puts("Data Object");
				break;
		case STT_FUNC:
				puts("Executable Code");
				break;
		case STT_SECTION:
				puts("Section");
				break;
		case STT_FILE:
				puts("Source Filename");
				break;
		default:
				puts("*** Unrecognised Value ***");
				return -1;
	}

	return 0;
}

int print_header(char *file_buffer)
{
	puts("\n********** ELF Header Information **********\n");
	ELF_HEADER *header = (ELF_HEADER*)file_buffer;

	print_field("Type");
   	switch(header->e_type) {
		case ET_NONE:
			puts("Unknown");
			break;
		case ET_REL:
			puts("Relocatable");
			break;
		case ET_EXEC:
			puts("Executable");
			break;
		case ET_DYN:
			puts("Shared Object File");
			break;
		case ET_CORE:
			puts("Core file");
			break;
		default:
			puts("*** Unrecognised Type ***");
	};

	print_field("Machine Type");
	switch(header->e_machine) {
		case EM_NONE:
			puts("Unknown");
			break;
		case EM_M32:
			puts("AT&T WE 32100");
			break;
		case EM_SPARC:
			puts("Sun Microsystems SPARC");
			break;
		case EM_386:
			puts("Intel 80386");
			break;
		case EM_68K:
			puts("Motorola 68000");
			break;
		case EM_88K:
			puts("Motorola 88000");
			break;
		case EM_860:
			puts("Intel 80860");
			break;
		case EM_MIPS:
			puts("MIPS RS3000");
			break;
		case EM_PARISC:
			puts("HP Precision");
			break;
		case EM_SPARC32PLUS:
			puts("SPARC (Enhanced ISA)");
			break;
		case EM_PPC:
			puts("PowerPC");
			break;
		case EM_PPC64:
			puts("PowerPC 64-bit");
			break;
		case EM_S390:
			puts("IBM S/390");
			break;
		case EM_ARM:
			puts("ARM");
			break;
		case EM_SH:
			puts("Renesas SuperH");
			break;
		case EM_SPARCV9:
			puts("SPARC v9 64-bit");
			break;
		case EM_IA_64:
			puts("Intel Itanium");
			break;
		case EM_X86_64:
			puts("AMD x86-64");
			break;
		default:
			puts("*** Unrecognised Machine ***");
	};

	print_field("Version");
	switch(header->e_version) {
		case EV_NONE:
			puts("Invalid");
			break;
		case EV_CURRENT:
			puts("Current");
			break;
		default:
			puts("*** Unrecognized Version ***");
	};

	print_half("Elf Header size", header->e_ehsize);
	print_pointer("Entry Point", (void *)header->e_entry);
	print_offset("PHdr Table Offset", header->e_phoff);
	print_offset("SHdr Table Offset", header->e_shoff);
	print_half("PHdr Entry Size", header->e_phentsize);
	print_half("SHdr Entry Size", header->e_shentsize);
	print_half("PHdr Entries", header->e_phnum);
	print_half("SHdr Entries", header->e_shnum);
	print_half("Section Name Stringtab Index", header->e_shstrndx);

	return 0;
}

int print_program_headers(char *file_buffer)
{
	puts("\n********** Program Header Information **********\n");
	PROGRAM_HEADER *program_headers = get_phdr_table(file_buffer);
	unsigned short num_pht_entries = ((ELF_HEADER*)file_buffer)->e_phnum;
	for(int i=0; i<num_pht_entries; i++) {
		print_field("\nSegment type");
		switch((program_headers+i)->p_type) {
				case PT_NULL:
				puts("Unused");
				break;
			case PT_LOAD:
				puts("Load");
				break;
			case PT_DYNAMIC:
				puts("Dynamic Linking Information");
				break;
			case PT_INTERP:
				puts("Executable Interpreter");
				break;
			case PT_NOTE:
				puts("Note Information");
				break;
			case PT_SHLIB:
				puts("Unspecified");
				break;
			case PT_PHDR:
				puts("Program Header Table");
				break;
			case PT_LOPROC:
				puts("Processor Specific");
				break;
			case PT_HIPROC:
				puts("Processor Specific");
				break;
			case PT_GNU_STACK:
				puts("Stack Information");
				break;
			case PT_GNU_EH_FRAME:
			    puts("Exception Handler Information");
				break;	
			default:
				puts("*** Unrecognised ***");
				break;
		}

		print_offset("Segment file offset", (program_headers+i)->p_offset);
		print_pointer("Segment vaddr", (void*)(program_headers+i)->p_vaddr);
		print_pointer("Segment paddr", (void*)(program_headers+i)->p_paddr);
		print_xword("Segment size (file)", (program_headers+i)->p_filesz);
		print_xword("Segment size (mem)", (program_headers+i)->p_memsz);
		print_segment_permissions((program_headers+i)->p_flags);

	}
	return 0;
}

int print_section_headers(char *file_buffer)
{
	SECTION_HEADER *section_headers = get_shdr_table(file_buffer);
	unsigned long num_section_headers = ((ELF_HEADER*)file_buffer)->e_shnum;
	char *section_name_table = get_section_name_table(file_buffer);
	
	puts("\n********** Section Header Information **********\n");
	for(int i=0; i<num_section_headers; i++) {
		print_string("\nSection name", section_name_table + (section_headers +i)->sh_name);
		print_field("Section type");
		switch((section_headers+i)->sh_type) {
				case SHT_NULL:
						puts("Inactive");
						break;
				case SHT_PROGBITS:
						puts("Program-Specific");
						break;
				case SHT_SYMTAB:
						puts("Symbol Table");
						break;
				case SHT_STRTAB:
						puts("String Table");
						break;
				case SHT_RELA:
						puts("Relocation (Explicit Addends)");
						break;
				case SHT_HASH:
						puts("Symbol Hash Table");
						break;
				case SHT_DYNAMIC:
						puts("Dynamic Linking Info");
						break;
				case SHT_NOTE:
						puts("Notes");
						break;
				case SHT_NOBITS:
						puts("Zero Bytes");
						break;
				case SHT_REL:
						puts("Relocation (No Explicit Addends)");
						break;
				case SHT_SHLIB:
						puts("Reserved (Unspecified)");
						break;
				case SHT_DYNSYM:
						puts("Dynamic Linking Symbols");
						break;
				case SHT_LOPROC:
						puts("Processor-Specified");
						break;
				case SHT_HIPROC:
						puts("Processor-Specified");
						break;
				case SHT_LOUSER:
						puts("Application-Specified (Lower Boundary)");
						break;
				case SHT_HIUSER:
						puts("Application Specified (Higher Boundary)");
						break;
				default:
						puts("*** Unrecognised Section Type ***");
						break;
		}	
		
		print_section_flags((section_headers+i)->sh_flags);
		print_offset("Section file offset", (section_headers+i)->sh_offset);
		print_xword("Section file size", (section_headers+i)->sh_size);

		if ((section_headers+i)->sh_flags & SHF_ALLOC) 
				print_pointer("Section vaddr",(void*)(section_headers+i)->sh_addr); 
	
		if ((section_headers+i)->sh_entsize) 
				print_xword("Section entry size", (section_headers+i)->sh_entsize); 
	}
	return 0;
}

int print_linking_info(char *file_buffer)
{
	SYMBOL_INFO *dynsym;
	SECTION_HEADER *dynsym_sht_entry = get_shdr_by_type(file_buffer, SHT_DYNSYM);
	if (dynsym_sht_entry == NULL) {
		puts("Dynamic Linking Information Unavailable");
		return -1;
	}
	
	unsigned int num_dynsyms = dynsym_sht_entry->sh_size / dynsym_sht_entry->sh_entsize;
	SYMBOL_INFO *dynsym_section = (SYMBOL_INFO*)(file_buffer + dynsym_sht_entry->sh_offset);
	char *dynstr_table = get_section_by_name(file_buffer, ".dynstr");
	if (dynstr_table == NULL) {
		puts("Warning: Could not locate .dynstr section");
		return -1;
	} 

	SECTION_HEADER *shdr_table = get_shdr_table(file_buffer);
	puts("\n********** Linking Information **********\n");
	for (int i=0; i < num_dynsyms; i++) {
		dynsym = dynsym_section + i;
		print_string("Symbol name", dynstr_table + dynsym->st_name);
		print_string("Section", get_shdr_name(file_buffer, shdr_table + dynsym->st_shndx));
		print_sym_binding((dynsym_section + i)->st_info);
		print_sym_type((dynsym_section + i)->st_info);	
		puts("\n");
	}
	
	return 0;
}

int print_symbol_info(char *file_buffer, unsigned int sym_type)
{
	SYMBOL_INFO *sym;
	SECTION_HEADER *sym_sht_entry = get_shdr_by_type(file_buffer, sym_type);
	if (sym_sht_entry == NULL) {
		puts("Symbol Information Unavailable");
		return -1;
	}
	
	unsigned int num_syms = sym_sht_entry->sh_size / sym_sht_entry->sh_entsize;
	SYMBOL_INFO *sym_section = (SYMBOL_INFO*)(file_buffer + sym_sht_entry->sh_offset);
	char *str_table = get_section_by_name(file_buffer,\
														 (sym_type == SHT_DYNSYM ? ".dynstr" : ".strtab"));

	if (str_table == NULL) {
		puts("Warning: Could not locate string table");
		return -1;
	} 

	SECTION_HEADER *shdr_table = get_shdr_table(file_buffer);
	puts("\n********** Linking Information **********\n");
	for (int i=0; i < num_syms; i++) {
		printf("%d.\n", i);
		sym = sym_section + i;
		print_string("Symbol name", str_table + sym->st_name);
		print_sym_binding(sym->st_info);
		print_sym_type(sym->st_info);	
		print_xword("Symbol size", sym->st_size);
		puts("\n");
	}
	
	return 0;
}
