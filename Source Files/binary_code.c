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

char *handleOpcodeBinaryCode(AssemblyLine *parsedLine)
{
    int opcodeCode = getOpcodeCode(parsedLine->instruction);
    int binaryCode = 0;

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

    /*
        logger(LOG_LEVEL_DEBUG, "insert to binaryCodesTable: <%s> at location: <%d>", binaryCode, IC);
        insertToBinaryCodesTable(binaryCodesTable, IC, parsedLine, binaryCode, parsedLine->src->value);
    */

    return convertIntToBinary(binaryCode, BINARY_CODE_LEN);
}