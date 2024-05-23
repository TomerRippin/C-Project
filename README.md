
# ✨ C Lab Project: Assembler ✨
Welcome to Lilach's & Tomer's C Project for the Open University (mmn 14).
<br>
This code is an assembler for an assembly-like language. 

## 🎮 Usage
```bash
# To build the assembler:
make assembler

# To run the assembler:
assembler.exe <.as file path>
```
**Note:** Our assembler only supports LF (unix) formats. Make sure your files are in the right format!

To clean up the output files (`.ent`, `.ext`, etc), just run:
```bash
make clean
```

## 🌊 Flow of the Program
1. **Clean-up**: Our assembler starts by tidying up the file. It removes extra whitespaces and fixes spaces in each line. For example, a line like `LAB:  .data         7, 8,  5` turns into `LAB: .data 7,8,5`. This creates a new, cleaner file - `.as-clean`.
2. **Pre-Assembler**: Next, it unpacks macros using a `LinkedList` and writes them to a file - `.am`.
3. **First Pass**: The first pass parses all symbols to a `SymbolTable` and creates binary code of the assembly lines (except entry), which are then inserted into the `BinaryCodesTable`.
4. **Second Pass**: The second pass parses entry symbols.
5. **Export to Files**: Finally, it writes the entry `.ent` file (optional - only if entries in file), external `.ext` (optional - only if externals in file), and object file `.ob`.

## 🏗️ Main Code Frames
- **PreAssembler**
- **FirstPass**
- **SecondPass**
- **handleEntryFile**
- **handleExternFile**
- **createObjectFile**

## 📚 Parsers
- **AssemblyLineParser**: Responsible for parsing each line to an AssemblyLine struct. Also, determines the adrType of each operand.
- **BinaryCode**: Responsible for creating the binary code of the source code.

## 🧩 Data Structures
- **AssemblyLine**: Represents a parsed assembly line, with a label, instruction and operands.
- **Operand**: Represents an operand, with a value and address type.
- **MacrosList**: A linked list representing the list of macros in the file.
- **SymbolTable**: A linked list represnting the symbol in the file.
- **BinaryCodesTable**: A linked list representing the binary codes of all of the source file code.