#include "../HeaderFiles/binary_code.h"

char *convertIntToBinary(int num, int len)
{
    char *binary = (char *)malloc(sizeof(char) * len);
    int i, k = 0;

    /* Iterate over each bit in the integer */
    for (i = (1 << (len - 1)); i > 0; i = i / 2)
        /** Use bitwise AND to check if the current bit is set or not
         * If the bit is set, add '1' to the binary string
         * If the bit is not set, add '0' to the binary string */
        binary[k++] = (num & i) ? '1' : '0';

    /* Add a null character at the end of the binary string */
    binary[k] = '\0';
    return binary;
}

int convertBinaryToDecimal(char *binary)
{
    return (binary[0] - '0') * 2 + (binary[1] - '0');
}

/* TODO: delete? not in use anymore */
char *convertIntToTCBinary(int num, int len)
{
    char *binary = (char *)malloc(sizeof(char) * len);
    int i, k = 0;

    /* If the number is negative, compute its two's complement */
    if (num < 0)
    {
        num = (1 << len) + num;
    }

    /* Iterate over each bit in the integer */
    for (i = (1 << (len - 1)); i > 0; i = i / 2)
        /** Use bitwise AND to check if the current bit is set or not
         * If the bit is set, add '1' to the binary string
         * If the bit is not set, add '0' to the binary string */
        binary[k++] = (num & i) ? '1' : '0';

    /* Add a null character at the end of the binary string */
    binary[k] = '\0';
    return binary;
}

/* TODO: delete? not in use anymore */
void reverseBits(char *bitsArray)
{
    int start = 0;
    int end = strlen(bitsArray) - 1;

    /* Loop until the two pointers meet in the middle */
    while (start < end)
    {
        /* Swap the bits at the start and end positions */
        char temp = bitsArray[start];
        bitsArray[start] = bitsArray[end];
        bitsArray[end] = temp;

        /* Move the start pointer forward and the end pointer backward */
        start++;
        end--;
    }
}

char* decodeBinaryCode(char *binaryCode)
{
    int i, num;
    char *decodedBinaryCode = (char*) malloc(sizeof(char) * DECODED_BINARY_CODE_LEN);

    /* Loop through pairs of bits */
    for (i = 0; i < BINARY_CODE_LEN; i += 2)
    {
        char pair[3] = {binaryCode[i], binaryCode[i+1], '\0'};
        /* Convert the pair of bits to an integer */
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
    int opcodeCode = getOpcodeCode(parsedLine->instruction);
    int binaryCode = 0;
    int funcRetVal;

    /* Store the number of operands in the higher bits of binary */
    binaryCode |= (opcodeCode << 6);

    if (parsedLine->dst->adrType != -1)
    {
        /* Store the address type of the destination operand in bits 2-3 of binary */
        binaryCode |= (parsedLine->dst->adrType << 2);
    }
    if (parsedLine->src->adrType != -1)
    {
        /* Store the address type of the source operand in bits 4-5 of binary */
        binaryCode |= (parsedLine->src->adrType << 4);
    }

    funcRetVal = insertToBinaryCodesTable(binaryCodesTable, *IC, parsedLine, convertIntToBinary(binaryCode, BINARY_CODE_LEN), parsedLine->instruction);

    return funcRetVal;
}

int handleAdrType0(Operand *operand, AssemblyLine *parsedLine, BinaryCodesTable *binaryCodesTable, SymbolTable *symbolTable, int *IC)
{
    int binaryCode, handlerRetVal, num;
    char *token;
    binaryCode = handlerRetVal = 0;

    if (isNumber(operand->value + 1)){
        num = atoi(operand->value + 1);
    }
    else {
        token = operand->value + 1;
        SymbolNode *searchResult = searchSymbolNameInTable(symbolTable, token);
        if (searchResult == NULL){
            return ERROR_GIVEN_SYMBOL_NOT_EXIST;
        } 
        else if (strcmp(searchResult->symbolType, SYMBOL_TYPE_MDEFINE) == 0){
            logger(LOG_LEVEL_DEBUG, "Found a valid defiend label");
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
    int binaryCode, handlerRetVal;
    int found = 1;
    SymbolNode *searchResult = symbolTable->head;
    binaryCode = handlerRetVal = 0;

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
            /* insert to list that the external label was used in this IC */
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
    int labelAddressBinaryCode, indexBinaryCode, handlerRetVal;
    labelAddressBinaryCode = indexBinaryCode = handlerRetVal = 0;
    const char *labelEnd = strchr(operand->value, '[');
    const char *indexEnd = strchr(labelEnd, ']');
    char *label;
    char *index;
    int found = 1;
    int labelLen, indexLen;
    SymbolNode *searchResult = symbolTable->head;

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
        /* if label exists with type data or code */
        if ((strcmp(searchResult->symbolName, label) == 0)
            && ((strcmp(searchResult->symbolType, SYMBOL_TYPE_DATA) == 0
                 || strcmp(searchResult->symbolType, SYMBOL_TYPE_CODE) == 0)))
        {
            /* bits 0-1: ARE codex - 'R' - 10, label is internal */
            labelAddressBinaryCode |= 2;
            /* bits 2-13 are the address of the label */
            labelAddressBinaryCode |= (searchResult->symbolValue << 2);
            logger(LOG_LEVEL_WARNING, "search address found! line number: %d\n", searchResult->symbolValue);

            found = 0;
        }
        else if ((strcmp(searchResult->symbolName, operand->value) == 0) && (strcmp(searchResult->symbolType, SYMBOL_TYPE_EXTERNAL) == 0))
        {
            /* bits 0-1: ARE codex - 'E' - 01, label is external */
            labelAddressBinaryCode |= 1;
            /* insert to list that the external label was used in this IC */
            insertToSymbolTable(symbolTable, operand->value, SYMBOL_TYPE_EXTERNAL_USAGE, *IC);

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
    /* handle the index */
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
        /* search for the index if it is defiend elsewhere in the code */
        /* TODO: replace to searchWithType - check if it can be only mdefine or LIST[2] also... */
        searchResult = searchSymbolNameInTable(symbolTable, index);
        if (searchResult == NULL)
        {
            return ERROR_GIVEN_SYMBOL_NOT_EXIST;
        }
        if (strcmp(searchResult->symbolType, SYMBOL_TYPE_MDEFINE))
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
    int binaryCode, handlerRetVal;
    int num;
    binaryCode = handlerRetVal = 0;

    /* Parse the number from the operand */
    num = atoi(operand->value + 1);

    /* Check if the register number is valid */
    /* TODO: maybe change to is valid register? maybe no need because the parse is checking this? */
    if (num < 1 || num > 8){
        return ERROR_NOT_VALID_REGISTER;
    }

    if (isSource){
        /* Add the bits representing the src number to bits 5-7 */
        binaryCode |= (num << 5);
    }
    else {
        /* Add the bits representing the dst number to bits 2-4 */
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
        return ERROR_NOT_VALID_REGISTER;
    }

    /* Parse the number from the operand */
    srcNum = atoi(parsedLine->src->value + 1);
    dstNum = atoi(parsedLine->dst->value + 1);

    /* Check if the register number is valid
    if ((srcNum < 1 || srcNum > 8) || (dstNum < 1 || dstNum > 8)){
        return ERROR_NOT_VALID_REGISTER;
    }*/

    /* Add the bits representing the src number to bits 5-7 */
    binaryCode |= (srcNum << 5);

    /* Add the bits representing the dst number to bits 2-4 */
    binaryCode |= (dstNum << 2);

    handlerRetVal = insertToBinaryCodesTable(binaryCodesTable, *IC, parsedLine, convertIntToBinary(binaryCode, BINARY_CODE_LEN), parsedLine->operands);
    *IC = *IC + 1;

    return handlerRetVal;
}

int handleOperandsBinaryCode(AssemblyLine *parsedLine, BinaryCodesTable *binaryCodesTable, SymbolTable *symbolTable, int IC)
{
    Operand *srcOperand = parsedLine->src;
    Operand *dstOperand = parsedLine->dst;
    int handlerRetVal = 0;

    if (srcOperand->adrType == 3 && dstOperand->adrType == 3)
    {
        handlerRetVal = handleAdrType3EdgeCase(parsedLine, binaryCodesTable, &IC);
    }
    else
    {
        switch (parsedLine->src->adrType)
        {
        case -1:
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