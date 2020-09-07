#include <linux/elf.h>

#ifdef __x86_64__

#define ELF_HEADER Elf64_Ehdr
#define PROGRAM_HEADER Elf64_Phdr
#define SECTION_HEADER Elf64_Shdr
#define print_half(message, val) printf("%-30s0x%x\n", message, val)
#define print_offset(message, val) printf("%-30s0x%llx\n", message, val)
#define print_xword(message, val) print_offset(message, val)

#elif defined __i386__

#define ELF_HEADER Elf32_Ehdr
#define PROGRAM_HEADER Elf32_Phdr
#define SECTION_HEADER Elf32_Shdr
#define print_half(message, val) printf("%-30s%x\n", message, val)
#define print_offset(message, val) printf("%-30s%x\n", message, val)
#define print_xword(message, val) print_offset(message, val)

#endif

#define print_field(type) printf("%-30s", type)
#define print_string(type, val) printf("%-30s%s\n", type, val)
#define print_pointer(message, val) printf("%-30s%p\n", message, val)


int print_header(char *file_buffer);

int print_program_headers(char *file_buffer);

int print_section_headers(char *file_buffer);
