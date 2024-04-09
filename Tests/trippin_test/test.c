#include <stdio.h>
#include "../Header Files/operands_parser.h"



int main(int argc, char const *argv[])
{
    char *opcode = "jmp";
    int res = getOpcodeOperandsNum(opcode);
    printf("result = %d", res);
    return 0;
}
