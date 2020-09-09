#include <linux/elf.h>

#ifdef __x86_64__

#define ELF_HEADER Elf64_Ehdr
#define PROGRAM_HEADER Elf64_Phdr
#define SECTION_HEADER Elf64_Shdr
#define SYMBOL_INFO Elf64_Sym
#define OFFSET Elf64_Off
#define POINTER Elf64_Addr
#define print_half(message, val) printf("%-30s0x%x\n", message, val)
#define print_offset(message, val) printf("%-30s0x%llx\n", message, val)
#define print_xword(message, val) print_offset(message, val)
#define xtract_sym_binding(val)	ELF64_ST_BIND(val)
#define xtract_sym_type(val) ELF64_ST_TYPE(val)

#elif defined __i386__

#define ELF_HEADER Elf32_Ehdr
#define PROGRAM_HEADER Elf32_Phdr
#define SECTION_HEADER Elf32_Shdr
#define SYMBOL_INFO Elf32_Sym
#define OFFSET Elf32_Off
#define POINTER Elf32_Addr
#define print_half(message, val) printf("%-30s%x\n", message, val)
#define print_offset(message, val) printf("%-30s%x\n", message, val)
#define print_xword(message, val) print_offset(message, val)
#define xtract_sym_binding(val) ELF32_ST_BIND(val)
#define xtract_sym_type(val) ELF32_ST_TYPE(val)

#endif

#define print_field(type) printf("%-30s", type)
#define print_string(type, val) printf("%-30s%s\n", type, val)
#define print_pointer(message, val) printf("%-30s%p\n", message, val)

PROGRAM_HEADER *get_phdr_by_type(char *file_buffer, unsigned int ph_type);

SECTION_HEADER *get_shdr_by_type(char *file_buffer, unsigned int sh_type);

void *get_section_by_name(char *file_buffer, char *section_name);

char *get_section_name_table(char *file_buffer);

char *get_shdr_name(char *file_buffer, SECTION_HEADER *shdr);

SECTION_HEADER *get_shdr_table(char *file_buffer);

PROGRAM_HEADER *get_phdr_table(char *file_buffer); 

