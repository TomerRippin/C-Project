# C Lab Project
Lilach's & Tomer's C Project for the Open University (mmn 14).
<br>
This code is an assembler for an assembly-like language. 

### Usage ###
```bash
make assembler
assembler.exe <.as file path>
```
NOTE: Only LF (unix) formats are supported.

To clean output files (`.ent`, `.ext`, etc), run:
```bash
make clean
```

### Flow ###
0. Cleans file - removing extra whitespaces, fixes spaces in each line. For example: <br>
                 line: `LAB:  .data         7, 8,  5`<br>
                 turns into: `LAB: .data 7,8,5`<br>
                 creates a new file - `.as-clean`
1. Pre assembler - unpacking macros using `LinkedList` to a file - `.am`
2. First Pass - parses all symbols to `SymbolTable` and created binary code of the assembly lines (except entry), inserts to `BinaryCodesTable`
3. Second Pass - parses entry symbols
4. Export to files - writing entry `.ent`, external `.ext` and object file `.ob`

### Main Code Frames
- PreAssembler
- FirstPass
- SecondPass

### Parsers
- AssemblyLineParser
- BinaryCode

### Data Structures
- MacrosList
- SymbolTabel
- BinaryCodesTable
