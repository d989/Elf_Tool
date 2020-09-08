int read_input_from_user(char *buffer, size_t buf_len);
static inline void clear_input_buffer(void){char c; while((c = getchar()) != '\n' && c != EOF);}
int check_file_overwrite(char *filename);
unsigned long get_file_size(FILE *finout);
int save_file(char *file_buffer, unsigned long file_size);
