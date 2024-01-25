#ifndef VM_VM_H_
#define VM_VM_H

#define OPERATOR_NOP 0
#define OPERATOR_INCR 1
#define OPERATOR_DECR 2
#define OPERATOR_J 3
#define OPERATOR_INC 4
#define OPERATOR_DEC 5
#define OPERATOR_ADD 6
#define OPERATOR_SUB 7
#define OPERATOR_MULT 8
#define OPERATOR_DIV 9
#define OPERATOR_MOD 10
#define OPERATOR_NEG 11
#define OPERATOR_BAND 12
#define OPERATOR_BOR 13
#define OPERATOR_BXOR 14
#define OPERATOR_LAND 15
#define OPERATOR_LOR 16
#define OPERATOR_BNOT 17
#define OPERATOR_LNOT 18
#define OPERATOR_MOV 19
#define OPERATOR_S 20
#define OPERATOR_LT 21
#define OPERATOR_GT 22
#define OPERATOR_LEQ 23
#define OPERATOR_GEQ 24
#define OPERATOR_EQ 25
#define OPERATOR_SETN 26
#define OPERATOR_IN 27
#define OPERATOR_OUT 28

#define INSTRUCTION_OPERATOR 0x1f
#define INSTRUCTION_REGISTER 0x60
#define INSTRUCTION_LENGTH 0x60

extern unsigned char bt_vm_data[10000];
extern unsigned char bt_vm_code [10000];

#endif
