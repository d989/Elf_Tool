#include <stdio.h>
#include <stdlib.h>
#include <linux/elf.h>
#include "parse_utilities.h"

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
	if((SHF_WRITE | SHF_ALLOC | SHF_EXECINSTR) & flags) {
		print_field("Section flags");
		if(flags & SHF_WRITE) printf("Writable\t"); 
		if(flags & SHF_ALLOC) printf("Memory-Resident\t"); 
		(flags & SHF_EXECINSTR) ? puts("Instructions") : puts("");
	}

	return 0;
}

char* get_section_name_table(char *file_buffer)
{
	SECTION_HEADER *section_headers = (SECTION_HEADER*)(file_buffer + ((ELF_HEADER*)file_buffer)->e_shoff);
	SECTION_HEADER *string_table_header = section_headers + ((ELF_HEADER*)file_buffer)->e_shstrndx;
	
	return(file_buffer + string_table_header->sh_offset);
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
	PROGRAM_HEADER *program_headers = (PROGRAM_HEADER*)(file_buffer + ((ELF_HEADER*)file_buffer)->e_phoff);
	unsigned short num_pht_entries = ((ELF_HEADER*)file_buffer)->e_phnum;
	for(int i=0; i<num_pht_entries; i++) {
		print_field("Segment type");
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

		puts("");

	}
	return 0;
}

int print_section_headers(char *file_buffer)
{
	puts("\n********** Section Header Information **********\n");
	SECTION_HEADER *section_headers = (SECTION_HEADER*)(file_buffer + ((ELF_HEADER*)file_buffer)->e_shoff);
	unsigned long num_section_headers = ((ELF_HEADER*)file_buffer)->e_shnum;
	
	char *section_name_table = get_section_name_table(file_buffer);

	for(int i=0; i<num_section_headers; i++) {
		print_string("Section name", section_name_table + (section_headers +i)->sh_name);
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

		if((section_headers+i)->sh_flags & SHF_ALLOC) print_pointer("Section vaddr",(void*)(section_headers+i)->sh_addr); 

		print_offset("Section file offset", (section_headers+i)->sh_offset);
		print_xword("Section file size", (section_headers+i)->sh_size);

		if((section_headers+i)->sh_entsize) print_xword("Section entry size", (section_headers+i)->sh_entsize); 
		puts("");
	}
	return 0;
}
