#include "binary_code.h"

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

int handleOpcodeBinaryCode(AssemblyLine *parsedLine, BinaryCodesTable *binaryCodesTable, int *IC)
{
    int opcodeCode = getOpcodeCode(parsedLine->instruction);
    int binaryCode = 0;
    int funcRetVal;

    /* Store the number of operands in the higher bits of binary */
    binaryCode |= (opcodeCode << 6);

    if (parsedLine->src->adrType != -1)
    {
        /* Store the address type of the destination operand in bits 2-3 of binary */
        binaryCode |= (parsedLine->src->adrType << 2);
    }
    if (parsedLine->dst->adrType != -1)
    {
        /* Store the address type of the source operand in bits 4-5 of binary */
        binaryCode |= (parsedLine->dst->adrType << 4);
    }

    funcRetVal = insertToBinaryCodesTable(binaryCodesTable, *IC, parsedLine, convertIntToBinary(binaryCode, BINARY_CODE_LEN), parsedLine->instruction);

    return funcRetVal;
}

int handleAdrType0(Operand *operand)
{
    /* Parse the number from the operand */
    int num = atoi(operand->value + 1);
    int binaryCode = 0;

    /* If the number is negative, compute its two's complement */
    if (num < 0)
    {
        num = (1 << (BINARY_CODE_LEN - 2)) + num;  /* The number is stored in the bits 2-12 */
    }

    /* bits 1-2: ARE codex - 'A' */
    binaryCode |= (num << 2);
    binaryCode |= 0;

    return binaryCode;
}

int handleAdrType1(Operand *operand)
{
    return SUCCESS;
}

int handleAdrType2(Operand *operand)
{
    return SUCCESS;
}

int handleAdrType3(Operand *operand, int isSource)
{
    int binaryCode = 0;
    /* Parse the number from the operand */
    int num = atoi(operand->value + 1);
    if (num < 1 && num > 8){
        return ERROR_NOT_VALID_REGISTER_NUMBER;
    }
    if (isSource){
        binaryCode |=  (num << 2);
    }
    else {
        binaryCode |=  (num << 5);
    }
    return binaryCode;
}

int handleAdrType3EdgeCase(AssemblyLine *parsedLine)
{
    return SUCCESS;
}

int handleOperandsBinaryCode(AssemblyLine *parsedLine, BinaryCodesTable *binaryCodesTable, int *IC)
{
    Operand *srcOperand = parsedLine->src;
    Operand *dstOperand = parsedLine->dst;
    int handlerRetVal;
    int binaryCode;

    if (srcOperand->adrType == 3 && dstOperand->adrType == 3)
    {
        handlerRetVal = handleAdrType3EdgeCase(parsedLine);
    }
    else
    {
        switch (parsedLine->src->adrType)
        {
        case -1:
            break;
        case 0:
            binaryCode = handleAdrType0(srcOperand);
            break;
        case 1:
            binaryCode = handleAdrType1(srcOperand);
            break;
        case 2:
            binaryCode = handleAdrType2(srcOperand);
            break;
        case 3:
            binaryCode = handleAdrType3(srcOperand, 1);
            break;
        }

        handlerRetVal = insertToBinaryCodesTable(binaryCodesTable, *IC, parsedLine, convertIntToBinary(binaryCode, BINARY_CODE_LEN), srcOperand->value);
        IC = IC + 1;

        if (handlerRetVal != SUCCESS)
        {
            return handlerRetVal;
        }

        switch (parsedLine->dst->adrType)
        {
        case -1:
            break;
        case 0:
            binaryCode = handleAdrType0(dstOperand);
            break;
        case 1:
            binaryCode = handleAdrType1(dstOperand);
            break;
        case 2:
            binaryCode = handleAdrType2(dstOperand);
            break;
        case 3:
            binaryCode = handleAdrType3(dstOperand, 0);
            break;
        }
        handlerRetVal = insertToBinaryCodesTable(binaryCodesTable, *IC, parsedLine, convertIntToBinary(binaryCode, BINARY_CODE_LEN), srcOperand->value);
        IC = IC + 1;

        if (handlerRetVal != SUCCESS)
        {
            return handlerRetVal;
        }
    }
    return SUCCESS;
}