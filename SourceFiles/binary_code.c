#include "../HeaderFiles/binary_code.h"

char *convertIntToBinary(int num, int len)
{
    char *binary = (char *)malloc(sizeof(char) * len);
    int i, k = 0;

    /* Iterates over each bit in the integer */
    for (i = (1 << (len - 1)); i > 0; i = i / 2)
        /** Uses bitwise AND to check if the current bit is set or not
         * If the bit is set, add '1' to the binary string
         * If the bit is not set, add '0' to the binary string */
        binary[k++] = (num & i) ? '1' : '0';

    /* Adds a null character at the end of the binary string */
    binary[k] = '\0';
    return binary;
}

int convertBinaryToDecimal(char *binary)
{
    return (binary[0] - '0') * 2 + (binary[1] - '0');
}

char* decodeBinaryCode(char *binaryCode)
{
    int i, num;
    char *decodedBinaryCode;
    decodedBinaryCode = (char *)malloc(sizeof(char) * DECODED_BINARY_CODE_LEN);

    /* Loops through pairs of bits */
    for (i = 0; i < BINARY_CODE_LEN; i += 2)
    {
        char pair[3];
        pair[0] = binaryCode[i];
        pair[1] = binaryCode[i + 1];
        pair[2] = '\0';
        /* Converts the pair of bits to an integer */
        num = convertBinaryToDecimal(pair);
        switch(num) {
            case 0:
                decodedBinaryCode[i/2] = DECODED_SYMBOL_0;
                break;
            case 1:
                decodedBinaryCode[i/2] = DECODED_SYMBOL_1;
                break;
            case 2:
                decodedBinaryCode[i/2] = DECODED_SYMBOL_2;
                break;
            case 3:
                decodedBinaryCode[i/2] = DECODED_SYMBOL_3;
                break;
        }
    }
    decodedBinaryCode[i/2] = '\0';
    return decodedBinaryCode;
}

int handleOpcodeBinaryCode(AssemblyLine *parsedLine, BinaryCodesTable *binaryCodesTable, int *IC)
{
    int opcodeCode, binaryCode, handlerRetVal;

    opcodeCode = getOpcodeCode(parsedLine->instruction);
    binaryCode = handlerRetVal = 0;

    /* Stores the number of operands in the higher bits of binary */
    binaryCode |= (opcodeCode << 6);

    if (parsedLine->dst->adrType != -1)
    {
        /* Stores the address type of the destination operand in bits 2-3 of binary */
        binaryCode |= (parsedLine->dst->adrType << 2);
    }
    if (parsedLine->src->adrType != -1)
    {
        /* Stores the address type of the source operand in bits 4-5 of binary */
        binaryCode |= (parsedLine->src->adrType << 4);
    }

    handlerRetVal = insertToBinaryCodesTable(binaryCodesTable, *IC, parsedLine, convertIntToBinary(binaryCode, BINARY_CODE_LEN), parsedLine->instruction);

    return handlerRetVal;
}

int handleAdrType0(Operand *operand, AssemblyLine *parsedLine, BinaryCodesTable *binaryCodesTable, SymbolTable *symbolTable, int *IC)
{
    int binaryCode, handlerRetVal, num;
    char *token;
    SymbolNode *searchResult;
    binaryCode = handlerRetVal = 0;

    if (isNumber(operand->value + 1)){
        num = atoi(operand->value + 1);
    }
    else {
        token = operand->value + 1;
        searchResult = searchSymbolNameInTable(symbolTable, token);
        if (searchResult == NULL) {
            return ERROR_GIVEN_SYMBOL_NOT_EXIST;
        } 
        else if (strcmp(searchResult->symbolType, SYMBOL_TYPE_MDEFINE) == 0) {
            num = searchResult->symbolValue;
        }
        else {
            return ERROR_GIVEN_SYMBOL_NOT_EXIST;
        }
    }
    /* If the number is negative, compute its two's complement */
    if (num < 0)
    {
        num = (1 << (BINARY_CODE_LEN - 2)) + num;  /* The number is stored in the bits 2-12 */
    }

    /* bits 1-2: ARE codex - 'A' - 00 */
    binaryCode |= (num << 2);
    binaryCode |= 0;

    handlerRetVal = insertToBinaryCodesTable(binaryCodesTable, *IC, parsedLine, convertIntToBinary(binaryCode, BINARY_CODE_LEN), operand->value);
    *IC = *IC + 1;

    return handlerRetVal;
}

int handleAdrType1(Operand *operand, AssemblyLine *parsedLine, BinaryCodesTable *binaryCodesTable, SymbolTable *symbolTable, int *IC)
{
    int binaryCode, handlerRetVal, found;
    SymbolNode *searchResult;

    binaryCode = handlerRetVal = 0;
    found = 1;
    searchResult = symbolTable->head;

    while (searchResult != NULL && found){
        if ((strcmp(searchResult->symbolName, operand->value) == 0) && (strcmp(searchResult->symbolType, SYMBOL_TYPE_DATA) == 0  || strcmp(searchResult->symbolType, SYMBOL_TYPE_CODE) == 0))
        {
            /* bits 1-2: ARE codex - 'R' - 10, label is internal */
            binaryCode |= (searchResult->symbolValue << 2);
            binaryCode |= 2;

            found = 0;
        }
        else if ((strcmp(searchResult->symbolName, operand->value) == 0) && strcmp(searchResult->symbolType, SYMBOL_TYPE_EXTERNAL) == 0)
        {
            /* bits 1-2: ARE codex - 'E' - 01, label is external */
            binaryCode |= 1;
            /* insert to the SymbolTable that the external label was used in this IC */
            insertToSymbolTable(symbolTable, operand->value, SYMBOL_TYPE_EXTERNAL_USAGE, *IC);
            
            found = 0;
        }
        searchResult = searchResult->next;
    }
    if (searchResult == NULL) {
        return ERROR_GIVEN_SYMBOL_NOT_EXIST;
    }

    /* freeNode(searchResult); */

    handlerRetVal = insertToBinaryCodesTable(binaryCodesTable, *IC, parsedLine, convertIntToBinary(binaryCode, BINARY_CODE_LEN), operand->value);
    *IC = *IC + 1;

    return handlerRetVal;
}

int handleAdrType2(Operand *operand, AssemblyLine *parsedLine, BinaryCodesTable *binaryCodesTable, SymbolTable *symbolTable, int *IC)
{
    int labelAddressBinaryCode, indexBinaryCode, handlerRetVal, found, labelLen, indexLen;
    char *labelEnd, *indexEnd, *label, *index;
    SymbolNode *searchResult;

    labelEnd = strchr(operand->value, '[');
    indexEnd = strchr(labelEnd, ']');
    searchResult = symbolTable->head;
    labelAddressBinaryCode = indexBinaryCode = handlerRetVal = 0;
    found = 1;
    handlerRetVal = 0;

    if (labelEnd == NULL || indexEnd == NULL)
    {
        /* Just in case, but this should not be thrown because already checked!! */
        return ERROR_ADDRESSING_TYPE_NOT_MATCHING;
    }

    labelLen = labelEnd - operand->value;
    label = malloc(labelLen + 1);
    strncpy(label, operand->value, labelLen);
    label[labelLen] = '\0';

    while (searchResult != NULL && found)
    {
        /* If symbol exists with type data or code */
        if ((strcmp(searchResult->symbolName, label) == 0)
             && ((strcmp(searchResult->symbolType, SYMBOL_TYPE_DATA) == 0
                  || strcmp(searchResult->symbolType, SYMBOL_TYPE_CODE) == 0)))
        {
            /* bits 0-1: ARE codex - 'R' - 10, label is internal */
            labelAddressBinaryCode |= 2;
            /* bits 2-13 are the address of the label */
            labelAddressBinaryCode |= (searchResult->symbolValue << 2);
            found = 0;
        }
        else if ((strcmp(searchResult->symbolName, label) == 0) && (strcmp(searchResult->symbolType, SYMBOL_TYPE_EXTERNAL) == 0))
        {
            /* bits 0-1: ARE codex - 'E' - 01, label is external */
            labelAddressBinaryCode |= 1;
            /* insert to the SymbolTable that the external label was used in this IC */
            insertToSymbolTable(symbolTable, label, SYMBOL_TYPE_EXTERNAL_USAGE, *IC);

            found = 0;
        }
        searchResult = searchResult->next;
    }

    if (searchResult == NULL && found == 1) {
        return ERROR_GIVEN_SYMBOL_NOT_EXIST;
    }

    
    handlerRetVal = insertToBinaryCodesTable(binaryCodesTable, *IC, parsedLine, convertIntToBinary(labelAddressBinaryCode, BINARY_CODE_LEN), label);

    if (handlerRetVal != SUCCESS)
    {
        return handlerRetVal;
    }

    *IC = *IC + 1;
    /* Handles the index */
    indexLen = indexEnd - labelEnd - 1; /* -1 to exclude the '[' character */
    index = malloc(indexLen + 1);
    strncpy(index, labelEnd + 1, indexLen); /* +1 to exclude the '[' character */
    index[indexLen] = '\0';

    if (isNumber(index))
    {
        /* bits 2-13 represent the index. bits 0-1 always 0 */
        indexBinaryCode |= (atoi(index) << 2);
        handlerRetVal = insertToBinaryCodesTable(binaryCodesTable, *IC, parsedLine, convertIntToBinary(indexBinaryCode, BINARY_CODE_LEN), index);
    }
    else
    {
        /* Searches for the index if it is defiend elsewhere in the code */
        /* NOTE: index can only be a mdefine symbol, or a number */
        searchResult = searchSymbolNameTypeInTable(symbolTable, index, SYMBOL_TYPE_MDEFINE);
        if (searchResult == NULL)
        {
            return ERROR_GIVEN_SYMBOL_NOT_EXIST;
        }
        /* bits 2-13 represent the index. bits 0-1 always 0 */
        indexBinaryCode |= (searchResult->symbolValue << 2);
        handlerRetVal = insertToBinaryCodesTable(binaryCodesTable, *IC, parsedLine, convertIntToBinary(indexBinaryCode, BINARY_CODE_LEN), index);
    }
    if (handlerRetVal != SUCCESS)
    {
        return handlerRetVal;
    }

    *IC = *IC + 1;

    free(label);
    free(index);

    return handlerRetVal;
}

int handleAdrType3(Operand *operand, int isSource, AssemblyLine *parsedLine, BinaryCodesTable *binaryCodesTable, int *IC)
{
    int binaryCode, handlerRetVal, num;

    binaryCode = handlerRetVal = 0;
    /* Parses the number from the operand */
    num = atoi(operand->value + 1);

    /* Checks if the register number is valid */
    if (num < 0 || num > 7){
        return ERROR_REGISTER_NOT_VALID;
    }

    if (isSource)
    {
        /* Adds the bits representing the src number to bits 5-7 */
        binaryCode |= (num << 5);
    }
    else {
        /* Adds the bits representing the dst number to bits 2-4 */
        binaryCode |= (num << 2);
    }

    handlerRetVal = insertToBinaryCodesTable(binaryCodesTable, *IC, parsedLine, convertIntToBinary(binaryCode, BINARY_CODE_LEN), operand->value);
    *IC = *IC + 1;

    return handlerRetVal;
}

int handleAdrType3EdgeCase(AssemblyLine *parsedLine, BinaryCodesTable *binaryCodesTable, int *IC)
{
    int binaryCode, handlerRetVal, srcNum, dstNum;

    binaryCode = handlerRetVal = 0;

    if (isValidRegisterOperand(parsedLine->src->value) != 1 || isValidRegisterOperand(parsedLine->dst->value) != 1)
    {
        return ERROR_REGISTER_NOT_VALID;
    }

    /* Parses the number from the operand */
    srcNum = atoi(parsedLine->src->value + 1);
    dstNum = atoi(parsedLine->dst->value + 1);

    /* Adds the bits representing the src number to bits 5-7 */
    binaryCode |= (srcNum << 5);

    /* Adds the bits representing the dst number to bits 2-4 */
    binaryCode |= (dstNum << 2);

    handlerRetVal = insertToBinaryCodesTable(binaryCodesTable, *IC, parsedLine, convertIntToBinary(binaryCode, BINARY_CODE_LEN), parsedLine->operands);
    *IC = *IC + 1;

    return handlerRetVal;
}

int handleOperandsBinaryCode(AssemblyLine *parsedLine, BinaryCodesTable *binaryCodesTable, SymbolTable *symbolTable, int IC)
{
    int handlerRetVal;
    Operand *srcOperand, *dstOperand;

    srcOperand = parsedLine->src;
    dstOperand = parsedLine->dst;

    handlerRetVal = 0;

    if (srcOperand->adrType == 3 && dstOperand->adrType == 3)
    {
        handlerRetVal = handleAdrType3EdgeCase(parsedLine, binaryCodesTable, &IC);
    }
    else
    {
        switch (parsedLine->src->adrType)
        {
        case -1:
            handlerRetVal = SUCCESS;
            break;
        case 0:
            handlerRetVal = handleAdrType0(srcOperand, parsedLine, binaryCodesTable, symbolTable, &IC);
            break;
        case 1:
            handlerRetVal = handleAdrType1(srcOperand, parsedLine, binaryCodesTable, symbolTable, &IC);
            break;
        case 2:
            handlerRetVal = handleAdrType2(srcOperand, parsedLine, binaryCodesTable, symbolTable, &IC);
            break;
        case 3:
            handlerRetVal = handleAdrType3(srcOperand, 1, parsedLine, binaryCodesTable, &IC);
            break;
        }

        if (handlerRetVal != SUCCESS)
        {
            return handlerRetVal;
        }

        switch (parsedLine->dst->adrType)
        {
        case -1:
            handlerRetVal = SUCCESS;
            break;
        case 0:
            handlerRetVal = handleAdrType0(dstOperand, parsedLine, binaryCodesTable, symbolTable, &IC);
            break;
        case 1:
            handlerRetVal = handleAdrType1(dstOperand, parsedLine, binaryCodesTable, symbolTable, &IC);
            break;
        case 2:
            handlerRetVal = handleAdrType2(dstOperand, parsedLine, binaryCodesTable, symbolTable, &IC);
            break;
        case 3:
            handlerRetVal = handleAdrType3(dstOperand, 0, parsedLine, binaryCodesTable, &IC);
            break;
        }

        if (handlerRetVal != SUCCESS)
        {
            return handlerRetVal;
        }
    }
    return SUCCESS;
}