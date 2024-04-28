# C-Project
Lilach and Tomers C Project for the Open University (mmn 14).

### Flow ###
0. Cleans file - removing extra whitespaces, fixes spaces in each line. for example:
                 `LAB:  .data         7, 8,  5` turns into: `LAB: .data 7,8,5`
1. Pre assembler - unpacking macros
2. First Pass - parses all symbols and created binary code of the (except entry)
3. Second Pass - parses entry symbols
4. Export to files - writing entry, external and object file

### Main Code Frames

#### PreAssembler

#### FirstPass

#### SecondPass

### Parsers

#### AssemblyLineParser

#### BinaryCode

### Data Structures

#### LinkedList

#### SymbolTabel

#### BinaryCodesTable