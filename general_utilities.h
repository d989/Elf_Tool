int read_input_from_user(char *buffer, size_t buf_len);
static inline void clear_input_buffer(void){char c; while((c = getchar()) != '\n' && c != EOF);}
