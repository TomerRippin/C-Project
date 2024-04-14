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

char *getOpcodeBinaryCode(AssemblyLine *parsedLine)
{
    char *binaryCode = (char *)malloc(sizeof(char) * BINARY_CODE_LEN);
    memset(binaryCode, '0', BINARY_CODE_LEN);

    /* bits 0-1: ARE codex - 'A' */
    strncpy(&binaryCode[0], "00", 3);

    /* bits 2-3: src operand address type */
    int srcType = parsedLine->src->adrType;
    switch (srcType)
    {
    case -1:
        strncpy(&binaryCode[2], "00", 3);
        break;
    case 0:
        strncpy(&binaryCode[2], "00", 3);
        break;
    case 1:
        strncpy(&binaryCode[2], "01", 3);
        break;
    case 2:
        strncpy(&binaryCode[2], "10", 3);
        break;
    case 3:
        strncpy(&binaryCode[2], "11", 3);
        break;
    default:
        break;
    }

    /* bits 4-5: dst operand address type */
    int dstType = parsedLine->dst->adrType;
    switch (dstType)
    {
    case -1:
        strncpy(&binaryCode[4], "00", 3);
        break;
    case 0:
        strncpy(&binaryCode[4], "00", 3);
        break;
    case 1:
        strncpy(&binaryCode[4], "01", 3);
        break;
    case 2:
        strncpy(&binaryCode[4], "10", 3);
        break;
    case 3:
        strncpy(&binaryCode[4], "11", 3);
        break;
    default:
        break;
    }

    /* bits 6-9: opcode */
    int opcodeCode = getOpcodeCode(parsedLine->instruction);
    strncpy(&binaryCode[6], convertIntToBinary(opcodeCode, 4), 4);

    /* bits 10-13: 0 (not in use) */
    strncpy(&binaryCode[10], "0000", 5);

    reverseBits(binaryCode);

    /* a null character at the end of the binary code string */
    binaryCode[BINARY_CODE_LEN] = '\0';

    return binaryCode;
}

char *getOperandsBinaryCode(AssemblyLine *parsedLine)
{
    char *binaryCode = (char *)malloc(sizeof(char) * BINARY_CODE_LEN);
    memset(binaryCode, '0', BINARY_CODE_LEN);
    return binaryCode;
}