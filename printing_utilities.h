#include "parse_utilities.h"

int print_segment_permissions(unsigned int flags);

int print_section_flags(unsigned int flags);

int print_sym_binding(unsigned char symtab_entry);

int print_sym_type(unsigned char symtab_entry);

int print_header(char *file_buffer);

int print_program_headers(char *file_buffer);

int print_section_headers (char *file_buffer);

int print_linking_info(char *file_buffer);

int print_symbol_info(char *file_buffer, unsigned int sym_type);

