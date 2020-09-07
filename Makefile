SHELL = /bin/sh
CC = gcc
OUTPUT = elf_tool

all:
	@${CC} elf_tool.c parse_utilities.c -o ${OUTPUT}
