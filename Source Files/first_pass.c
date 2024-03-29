#include <stdio.h>
#include <string.h>
#include "../Header Files/errors.h"
#include "../Header Files/constants.h"
#include "../Header Files/linked_list.h"

int handleDefine(char *line, LinkedList *symbolTable)
{
    printf("handleDefine");
    char symbol[MAX_LINE_LEN];
    int value;
    if (sscanf(line, ".define %80s = %d", symbol, &(value)))
    {
        if (searchList(symbolTable, *symbol) == NULL) {
            return ERROR_SYMBOL_ALREADY_EXIST;
        } else {
            insertToList(symbolTable, symbol, value);
            return SUCCESS;
        }
    }
    else {
        return ERROR_PARSE_DEFINE_INSTRUCTION;
    }
}

int handleDataInstruction(char *line, LinkedList *symbolTable, int isSymbol)
{
    printf("handleDataInstruction");
    /* TODO */
}

int getOpcodeInd(char *opcode) {
    for (int i = 0; i < NUM_OPCODES; i++) {
        if (opcode == OPCODES[i].name) {
            return SUCCESS;
        }
    }
    return ERROR_OPCODE_NOT_FOUND;
}

/* Hanhaia, maybe dlete */
int isInstructionLine(char *line)
{
    for (int i = 0; i < NUM_INSTRUCTIONS; i++)
    {
        /* TODO: change, .data can be after : */
        if (strncmp(line, INSTRUCTIONS[i], strlen(INSTRUCTIONS[i])) == 0)
        {
            return 1;
        }
    }
    return 0;
}

struct AssemblyLine {
    char *label;
    char *instruction;
    char *operands;
};

struct AssemblyLine parseAssemblyLine(const char *line)
{
    struct AssemblyLine result;
    result.label = NULL;
    result.instruction = NULL;
    result.operands = NULL;

    // Find the first occurrence of ':'
    char *colonPos = strchr(line, ':');
    if (colonPos != NULL)
    {
        // Extract the label
        size_t labelLen = colonPos - line;
        result.label = (char *)malloc(labelLen + 1);
        strncpy(result.label, line, labelLen);
        result.label[labelLen] = '\0';

        // Move the pointer past the colon
        line = colonPos + 1;
    }

    // Find the instruction (e.g., .data, .string, MOV, jmp, etc.)
    char *spacePos = strchr(line, ' ');
    if (spacePos != NULL)
    {
        size_t instrLen = spacePos - line;
        result.instruction = (char *)malloc(instrLen + 1);
        strncpy(result.instruction, line, instrLen);
        result.instruction[instrLen] = '\0';

        // Move the pointer past the space
        line = spacePos + 1;
    }

    // The remaining part is the operands
    result.operands = strdup(line);

    return result;
}

void printAssemblyLine(struct AssemblyLine *parsedLine)
{
    printf("Label: %s\n", parsedLine->label ? parsedLine->label : "(none)");
    printf("Instruction: %s\n", parsedLine->instruction ? parsedLine->instruction : "(none)");
    printf("Operands: %s\n", parsedLine->operands);
}

void freeAssemblyLine(struct AssemblyLine *line) {
    free(line->label);
    free(line->instruction);
    free(line->operands);
}

int firstPass(FILE *inputFile, FILE *outputFile)
{
    int IC = 0; /* Insturctions Counter */
    int DC = 0; /* Data Counter */
    char line[MAX_LINE_LEN];
    char symbol[MAX_LINE_LEN];
    LinkedList *symbolTable = createList();
    int isSymbol = 0;
    struct AssemblyLine parsedLine;
    int operandsNum;

    while (fgets(line, sizeof(line), inputFile) != NULL)
    {
        parsedLine = parseAssemblyLine(line);
        if (parsedLine.instruction == DEFINE_DIRECTIVE)
        {
            handleDefine(*line, symbolTable);
            continue;
        }
        if (parsedLine.label)
        {
            isSymbol = 1;
            if (searchList(symbolTable, parsedLine.label)) {
                return ERROR_SYMBOL_ALREADY_EXIST;
            }
        }

        if (parsedLine.instruction == ".data" || parsedLine.instruction == ".string")
        {
            if (isSymbol) {
                insertToList(symbolTable, parsedLine.label, ".data", DC);
                /* handle .data or .string - line 9 */
            }
        }
        else if (parsedLine.instruction == ".extern") {
            continue;
        }
        else if (parsedLine.instruction == ".entry") {
            continue;
        }

        if (isSymbol) {
            insertToList(symbolTable, parsedLine.label, ".code", IC + 100);
        }

        /* If we are here - this is a line of operations */
        for (int i = 0; i < NUM_OPCODES; i++)
        {
            if (parsedLine.instruction == OPCODES[i].name)
            {
                return SUCCESS;
            }
        }

            if (isValidOpcode)

            /**
             * if (isInstructionLine(*line))
            {
                handleInstruction(*line, symbolTable);
                continue;
            }
            */
        }