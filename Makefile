## TODO

## Test pre assembler
# gcc --ansi -Wall -I './Header Files/' './Source Files/linked_list.c' './Source Files/pre_assembler.c' './Source Files/test_pre_assembler.c' -o pre_assembler.exe

## Test cleaning file function
# gcc -ansi -Wall -I Header\ Files/ Tests/test_clean_file/test_clean_file.c Source\ Files/utils.c -o test.exe

## Test first_pass + clean_file
# Windows: gcc --ansi -Wall -I './Header Files/' './Source Files/first_pass.c' './Source Files/assembly_line_parser.c' '.\Source Files\linked_list.c' '.\Source Files\utils.c' './Tests/test_first_pass/test_first_pass.c' -o first_pass.exe
# Ubuntu: gcc -ansi -Wall -I Header\ Files/ Source\ Files/first_pass.c Source\ Files/binary_codes_table.c Source\ Files/logger.c Source\ Files/assembly_line_parser.c Source\ Files/linked_list.c Source\ Files/utils.c Tests/test_first_pass/test_first_pass.c -o first_pass.exe

## Test second_pass
# Ubuntu: gcc -ansi -Wall -I Header\ Files/ Source\ Files/logger.c Source\ Files/assembly_line_parser.c Source\ Files/linked_list.c Source\ Files/utils.c Source\ Files/files_utils.c Source\ Files/first_pass.c Source\ Files/second_pass.c Tests/test_second_pass/test_second_pass.c -o second_pass.exe

# gcc -ansi -Wall -I Header\ Files/ Source\ Files/assembly_line_parser.c Source\ Files/linked_list.c Source\ Files/utils.c Source\ Files/logger.c Source\ Files/files_utils.c Source\ Files/first_pass.c Source\ Files/second_pass.c Tests/test_second_pass/test_second_pass.c -o second_pass.exe


