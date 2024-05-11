# Usage:
# * make all
# * make first_pass
# * make clean

CC=gcc
CFLAGS=-ansi -Wall -pedantic -g -I HeaderFiles/
SOURCES=SourceFiles/logger.c SourceFiles/assembly_line_parser.c SourceFiles/binary_code.c SourceFiles/symbol_table.c SourceFiles/binary_codes_table.c SourceFiles/utils.c SourceFiles/files_utils.c SourceFiles/linked_list.c SourceFiles/pre_assembler.c SourceFiles/first_pass.c SourceFiles/second_pass.c
SOURCES_ASSEMBLER=$(SOURCES) SourceFiles/assembler.c 

all: assembler pre_assembler first_pass second_pass

assembler: $(SOURCES_ASSEMBLER)
	$(CC) $(CFLAGS) $(SOURCES_ASSEMBLER) -o assembler.exe

clean_file: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) Tests/test_clean_file/test_clean_file.c -o clean_file.exe

pre_assembler: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) Tests/test_pre_assembler/test_pre_assembler.c -o pre_assembler.exe

first_pass: $(SOURCES) Tests/test_first_pass/test_first_pass.c
	$(CC) $(CFLAGS) $(SOURCES) Tests/test_first_pass/test_first_pass.c -o first_pass.exe

second_pass: $(SOURCES) Tests/test_second_pass/test_second_pass.c
	$(CC) $(CFLAGS) $(SOURCES) Tests/test_second_pass/test_second_pass.c -o second_pass.exe

clean:
	rm -f *.exe
	rm -rf *.o
	rm -rf */*.as-cleaned
	rm -rf */*.am
	rm -rf */*.ent
	rm -rf */*.ext
	rm -rf */*.ob
