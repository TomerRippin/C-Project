.define sz = 2
.entry TO
.extern W
MAIN: mov r10, LIST[sz]
LOOP: jmp L1
mcr m_mcr
    cmp r3, #sz
    bne END
endmcr
TO: .data 7
prn #-5
mov     STR[5], STR[2]
 sub r1, r4
    m_mcr
L1: inc K
lea STR, W
bne LOOP
   END: hlt
.define len = 4
K: .data 22
STR: .string "abcdef"
LIST: .data 6, -9, len