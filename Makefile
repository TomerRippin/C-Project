# Usage Example:
# * make all
# * make first_pass
# * make clean

CC=gcc
CFLAGS=-ansi -Wall -pedantic -g -I HeaderFiles/
SOURCES=HeaderFiles/constants.h HeaderFiles/logger.h SourceFiles/logger.c SourceFiles/errors.c SourceFiles/assembly_line_parser.c SourceFiles/binary_code.c SourceFiles/symbol_table.c SourceFiles/binary_codes_table.c SourceFiles/utils.c SourceFiles/files_utils.c SourceFiles/macros_list.c SourceFiles/pre_assembler.c SourceFiles/first_pass.c SourceFiles/second_pass.c
SOURCES_ASSEMBLER=$(SOURCES) SourceFiles/assembler.c 

all: assembler pre_assembler first_pass second_pass

assembler: $(SOURCES_ASSEMBLER)
	$(CC) $(CFLAGS) $(SOURCES_ASSEMBLER) -o assembler

clean:
	rm -f *.exe
	rm -rf *.o
	rm -rf */*.as-clean
	rm -rf */*.am
	rm -rf */*.ent
	rm -rf */*.ext
	rm -rf */*.ob
