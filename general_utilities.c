#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "general_utilities.h"

int read_input_from_user(char *buffer, size_t buffer_size)
{
	size_t input_len;

	memset((void*)buffer, buffer_size, '\00');
	if(fgets(buffer, buffer_size, stdin) == NULL) {
		return -1;
	}

	input_len = strlen(buffer);
	if(input_len > 0 && buffer[input_len - 1] == '\n')
		buffer[input_len - 1] = '\00';

	return 0;
}
