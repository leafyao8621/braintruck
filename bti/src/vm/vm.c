#include <stdio.h>

#include "vm.h"
#include "../util/errcode.h"

uint8_t bt_vm_code[10000] = {0};
uint8_t bt_vm_data[10000] = {0};

int bt_vm_run(uint16_t n_cell, bool verbose) {
    uint16_t pc, ma, mx, my, cnt;
    uint8_t *iter = bt_vm_data;
    uint8_t inst = OPERATOR_NOP;
    pc = ma = mx = my = 0;
    if (n_cell >= 10000) {
        n_cell = 10000;
    }
    for (; !(pc & 0x8000) && pc < 10000 && bt_vm_code[pc]; ++pc) {
        inst = bt_vm_code[pc];
        switch (inst & INSTRUCTION_OPERATOR) {
        case OPERATOR_INCR:
            switch (inst & INSTRUCTION_REGISTER) {
            case 0x0:
                if (ma++ >= 9999) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                break;
            case 0x20:
                if (mx++ >= 9999) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                break;
            case 0x40:
                if (my++ >= 9999) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                break;
            }
            break;
        case OPERATOR_DECR:
            switch (inst & INSTRUCTION_REGISTER) {
            case 0x0:
                if (!(ma--)) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                break;
            case 0x20:
                if (!(mx--)) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                break;
            case 0x40:
                if (!(my--)) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                break;
            }
            break;
        case OPERATOR_J:
            if (inst & INSTRUCTION_SIGN) {
                if (bt_vm_data[ma]) {
                    pc = *(uint16_t*)(bt_vm_code + pc + 1) + 2;
                } else {
                    pc += 2;
                }
            } else {
                if (!bt_vm_data[ma]) {
                    pc = *(uint16_t*)(bt_vm_code + pc + 1) + 2;
                } else {
                    pc += 2;
                }
            }
            break;
        case OPERATOR_INC:
            switch (inst & INSTRUCTION_LENGTH) {
            case 0x0:
                ++bt_vm_data[ma];
                break;
            case 0x20:
                if (ma >= 9998) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                ++*(uint16_t*)(bt_vm_data + ma);
                break;
            case 0x40:
                if (ma >= 9996) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                ++*(uint32_t*)(bt_vm_data + ma);
                break;
            case 0x60:
                if (ma >= 9992) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                ++*(uint64_t*)(bt_vm_data + ma);
                break;
            }
            break;
        case OPERATOR_DEC:
            switch (inst & INSTRUCTION_LENGTH) {
            case 0x0:
                --bt_vm_data[ma];
                break;
            case 0x20:
                if (ma >= 9998) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                --*(uint16_t*)(bt_vm_data + ma);
                break;
            case 0x40:
                if (ma >= 9996) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                --*(uint32_t*)(bt_vm_data + ma);
                break;
            case 0x60:
                if (ma >= 9992) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                --*(uint64_t*)(bt_vm_data + ma);
                break;
            }
            break;
        case OPERATOR_ADD:
            switch (inst & INSTRUCTION_LENGTH) {
            case 0x0:
                bt_vm_data[ma] = bt_vm_data[mx] + bt_vm_data[my];
                break;
            case 0x20:
                if (ma >= 9998 || mx >= 9998 || my >= 9998) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint16_t*)(bt_vm_data + ma) =
                    *(uint16_t*)(bt_vm_data + mx) +
                    *(uint16_t*)(bt_vm_data + my);
                break;
            case 0x40:
                if (ma >= 9996 || mx >= 9996 || my >= 9996) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint32_t*)(bt_vm_data + ma) =
                    *(uint32_t*)(bt_vm_data + mx) +
                    *(uint32_t*)(bt_vm_data + my);
                break;
            case 0x60:
                if (ma >= 9992 || mx >= 9992 || my >= 9992) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint64_t*)(bt_vm_data + ma) =
                    *(uint64_t*)(bt_vm_data + mx) +
                    *(uint64_t*)(bt_vm_data + my);
                break;
            }
            break;
        case OPERATOR_SUB:
            switch (inst & INSTRUCTION_LENGTH) {
            case 0x0:
                bt_vm_data[ma] = bt_vm_data[mx] - bt_vm_data[my];
                break;
            case 0x20:
                if (ma >= 9998 || mx >= 9998 || my >= 9998) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint16_t*)(bt_vm_data + ma) =
                    *(uint16_t*)(bt_vm_data + mx) -
                    *(uint16_t*)(bt_vm_data + my);
                break;
            case 0x40:
                if (ma >= 9996 || mx >= 9996 || my >= 9996) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint32_t*)(bt_vm_data + ma) =
                    *(uint32_t*)(bt_vm_data + mx) -
                    *(uint32_t*)(bt_vm_data + my);
                break;
            case 0x60:
                if (ma >= 9992 || mx >= 9992 || my >= 9992) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint64_t*)(bt_vm_data + ma) =
                    *(uint64_t*)(bt_vm_data + mx) -
                    *(uint64_t*)(bt_vm_data + my);
                break;
            }
            break;
        case OPERATOR_MULT:
            switch (inst & INSTRUCTION_LENGTH) {
            case 0x0:
                bt_vm_data[ma] = bt_vm_data[mx] * bt_vm_data[my];
                break;
            case 0x20:
                if (ma >= 9998 || mx >= 9998 || my >= 9998) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint16_t*)(bt_vm_data + ma) =
                    *(uint16_t*)(bt_vm_data + mx) *
                    *(uint16_t*)(bt_vm_data + my);
                break;
            case 0x40:
                if (ma >= 9996 || mx >= 9996 || my >= 9996) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint32_t*)(bt_vm_data + ma) =
                    *(uint32_t*)(bt_vm_data + mx) *
                    *(uint32_t*)(bt_vm_data + my);
                break;
            case 0x60:
                if (ma >= 9992 || mx >= 9992 || my >= 9992) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint64_t*)(bt_vm_data + ma) =
                    *(uint64_t*)(bt_vm_data + mx) *
                    *(uint64_t*)(bt_vm_data + my);
                break;
            }
            break;
        case OPERATOR_DIV:
            switch (inst & INSTRUCTION_LENGTH) {
            case 0x0:
                bt_vm_data[ma] = bt_vm_data[mx] / bt_vm_data[my];
                break;
            case 0x20:
                if (ma >= 9998 || mx >= 9998 || my >= 9998) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint16_t*)(bt_vm_data + ma) =
                    *(uint16_t*)(bt_vm_data + mx) /
                    *(uint16_t*)(bt_vm_data + my);
                break;
            case 0x40:
                if (ma >= 9996 || mx >= 9996 || my >= 9996) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint32_t*)(bt_vm_data + ma) =
                    *(uint32_t*)(bt_vm_data + mx) /
                    *(uint32_t*)(bt_vm_data + my);
                break;
            case 0x60:
                if (ma >= 9992 || mx >= 9992 || my >= 9992) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint64_t*)(bt_vm_data + ma) =
                    *(uint64_t*)(bt_vm_data + mx) /
                    *(uint64_t*)(bt_vm_data + my);
                break;
            }
            break;
        case OPERATOR_MOD:
            switch (inst & INSTRUCTION_LENGTH) {
            case 0x0:
                bt_vm_data[ma] = bt_vm_data[mx] % bt_vm_data[my];
                break;
            case 0x20:
                if (ma >= 9998 || mx >= 9998 || my >= 9998) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint16_t*)(bt_vm_data + ma) =
                    *(uint16_t*)(bt_vm_data + mx) %
                    *(uint16_t*)(bt_vm_data + my);
                break;
            case 0x40:
                if (ma >= 9996 || mx >= 9996 || my >= 9996) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint32_t*)(bt_vm_data + ma) =
                    *(uint32_t*)(bt_vm_data + mx) %
                    *(uint32_t*)(bt_vm_data + my);
                break;
            case 0x60:
                if (ma >= 9992 || mx >= 9992 || my >= 9992) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint64_t*)(bt_vm_data + ma) =
                    *(uint64_t*)(bt_vm_data + mx) %
                    *(uint64_t*)(bt_vm_data + my);
                break;
            }
            break;
        case OPERATOR_NEG:
            switch (inst & INSTRUCTION_LENGTH) {
            case 0x0:
                bt_vm_data[ma] = -bt_vm_data[mx];
                break;
            case 0x20:
                if (ma >= 9998 || mx >= 9998) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint16_t*)(bt_vm_data + ma) =
                    -*(uint16_t*)(bt_vm_data + mx);
                break;
            case 0x40:
                if (ma >= 9996 || mx >= 9996) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint32_t*)(bt_vm_data + ma) =
                    -*(uint32_t*)(bt_vm_data + mx);
                break;
            case 0x60:
                if (ma >= 9992 || mx >= 9992) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint64_t*)(bt_vm_data + ma) =
                    -*(uint64_t*)(bt_vm_data + mx);
                break;
            }
            break;
        case OPERATOR_BAND:
            switch (inst & INSTRUCTION_LENGTH) {
            case 0x0:
                bt_vm_data[ma] = bt_vm_data[mx] & bt_vm_data[my];
                break;
            case 0x20:
                if (ma >= 9998 || mx >= 9998 || my >= 9998) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint16_t*)(bt_vm_data + ma) =
                    *(uint16_t*)(bt_vm_data + mx) &
                    *(uint16_t*)(bt_vm_data + my);
                break;
            case 0x40:
                if (ma >= 9996 || mx >= 9996 || my >= 9996) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint32_t*)(bt_vm_data + ma) =
                    *(uint32_t*)(bt_vm_data + mx) &
                    *(uint32_t*)(bt_vm_data + my);
                break;
            case 0x60:
                if (ma >= 9992 || mx >= 9992 || my >= 9992) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint64_t*)(bt_vm_data + ma) =
                    *(uint64_t*)(bt_vm_data + mx) &
                    *(uint64_t*)(bt_vm_data + my);
                break;
            }
            break;
        case OPERATOR_BOR:
            switch (inst & INSTRUCTION_LENGTH) {
            case 0x0:
                bt_vm_data[ma] = bt_vm_data[mx] | bt_vm_data[my];
                break;
            case 0x20:
                if (ma >= 9998 || mx >= 9998 || my >= 9998) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint16_t*)(bt_vm_data + ma) =
                    *(uint16_t*)(bt_vm_data + mx) |
                    *(uint16_t*)(bt_vm_data + my);
                break;
            case 0x40:
                if (ma >= 9996 || mx >= 9996 || my >= 9996) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint32_t*)(bt_vm_data + ma) =
                    *(uint32_t*)(bt_vm_data + mx) |
                    *(uint32_t*)(bt_vm_data + my);
                break;
            case 0x60:
                if (ma >= 9992 || mx >= 9992 || my >= 9992) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint64_t*)(bt_vm_data + ma) =
                    *(uint64_t*)(bt_vm_data + mx) |
                    *(uint64_t*)(bt_vm_data + my);
                break;
            }
            break;
        case OPERATOR_BXOR:
            switch (inst & INSTRUCTION_LENGTH) {
            case 0x0:
                bt_vm_data[ma] = bt_vm_data[mx] ^ bt_vm_data[my];
                break;
            case 0x20:
                if (ma >= 9998 || mx >= 9998 || my >= 9998) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint16_t*)(bt_vm_data + ma) =
                    *(uint16_t*)(bt_vm_data + mx) ^
                    *(uint16_t*)(bt_vm_data + my);
                break;
            case 0x40:
                if (ma >= 9996 || mx >= 9996 || my >= 9996) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint32_t*)(bt_vm_data + ma) =
                    *(uint32_t*)(bt_vm_data + mx) ^
                    *(uint32_t*)(bt_vm_data + my);
                break;
            case 0x60:
                if (ma >= 9992 || mx >= 9992 || my >= 9992) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint64_t*)(bt_vm_data + ma) =
                    *(uint64_t*)(bt_vm_data + mx) ^
                    *(uint64_t*)(bt_vm_data + my);
                break;
            }
            break;
        case OPERATOR_LAND:
            switch (inst & INSTRUCTION_LENGTH) {
            case 0x0:
                bt_vm_data[ma] = bt_vm_data[mx] && bt_vm_data[my];
                break;
            case 0x20:
                if (ma >= 9998 || mx >= 9998 || my >= 9998) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint16_t*)(bt_vm_data + ma) =
                    *(uint16_t*)(bt_vm_data + mx) &&
                    *(uint16_t*)(bt_vm_data + my);
                break;
            case 0x40:
                if (ma >= 9996 || mx >= 9996 || my >= 9996) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint32_t*)(bt_vm_data + ma) =
                    *(uint32_t*)(bt_vm_data + mx) &&
                    *(uint32_t*)(bt_vm_data + my);
                break;
            case 0x60:
                if (ma >= 9992 || mx >= 9992 || my >= 9992) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint64_t*)(bt_vm_data + ma) =
                    *(uint64_t*)(bt_vm_data + mx) &&
                    *(uint64_t*)(bt_vm_data + my);
                break;
            }
            break;
        case OPERATOR_LOR:
            switch (inst & INSTRUCTION_LENGTH) {
            case 0x0:
                bt_vm_data[ma] = bt_vm_data[mx] || bt_vm_data[my];
                break;
            case 0x20:
                if (ma >= 9998 || mx >= 9998 || my >= 9998) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint16_t*)(bt_vm_data + ma) =
                    *(uint16_t*)(bt_vm_data + mx) ||
                    *(uint16_t*)(bt_vm_data + my);
                break;
            case 0x40:
                if (ma >= 9996 || mx >= 9996 || my >= 9996) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint32_t*)(bt_vm_data + ma) =
                    *(uint32_t*)(bt_vm_data + mx) ||
                    *(uint32_t*)(bt_vm_data + my);
                break;
            case 0x60:
                if (ma >= 9992 || mx >= 9992 || my >= 9992) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint64_t*)(bt_vm_data + ma) =
                    *(uint64_t*)(bt_vm_data + mx) ||
                    *(uint64_t*)(bt_vm_data + my);
                break;
            }
            break;
        case OPERATOR_BNOT:
            switch (inst & INSTRUCTION_LENGTH) {
            case 0x0:
                bt_vm_data[ma] = ~bt_vm_data[mx];
                break;
            case 0x20:
                if (ma >= 9998 || mx >= 9998) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint16_t*)(bt_vm_data + ma) =
                    ~*(uint16_t*)(bt_vm_data + mx);
                break;
            case 0x40:
                if (ma >= 9996 || mx >= 9996) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint32_t*)(bt_vm_data + ma) =
                    ~*(uint32_t*)(bt_vm_data + mx);
                break;
            case 0x60:
                if (ma >= 9992 || mx >= 9992) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint64_t*)(bt_vm_data + ma) =
                    ~*(uint64_t*)(bt_vm_data + mx);
                break;
            }
            break;
        case OPERATOR_LNOT:
            switch (inst & INSTRUCTION_LENGTH) {
            case 0x0:
                bt_vm_data[ma] = !bt_vm_data[mx];
                break;
            case 0x20:
                if (ma >= 9998 || mx >= 9998) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint16_t*)(bt_vm_data + ma) =
                    !*(uint16_t*)(bt_vm_data + mx);
                break;
            case 0x40:
                if (ma >= 9996 || mx >= 9996) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint32_t*)(bt_vm_data + ma) =
                    !*(uint32_t*)(bt_vm_data + mx);
                break;
            case 0x60:
                if (ma >= 9992 || mx >= 9992) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint64_t*)(bt_vm_data + ma) =
                    !*(uint64_t*)(bt_vm_data + mx);
                break;
            }
            break;
        case OPERATOR_MOV:
            switch (inst & INSTRUCTION_LENGTH) {
            case 0x0:
                bt_vm_data[ma] = bt_vm_data[mx];
                break;
            case 0x20:
                if (ma >= 9998 || mx >= 9998) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint16_t*)(bt_vm_data + ma) =
                    *(uint16_t*)(bt_vm_data + mx);
                break;
            case 0x40:
                if (ma >= 9996 || mx >= 9996) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint32_t*)(bt_vm_data + ma) =
                    *(uint32_t*)(bt_vm_data + mx);
                break;
            case 0x60:
                if (ma >= 9992 || mx >= 9992) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint64_t*)(bt_vm_data + ma) =
                    *(uint64_t*)(bt_vm_data + mx);
                break;
            }
            break;
        case OPERATOR_EQ:
            switch (inst & INSTRUCTION_LENGTH) {
            case 0x0:
                bt_vm_data[ma] = bt_vm_data[mx] == bt_vm_data[my];
                break;
            case 0x20:
                if (ma >= 9998 || mx >= 9998 || my >= 9998) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint16_t*)(bt_vm_data + ma) =
                    *(uint16_t*)(bt_vm_data + mx) ==
                    *(uint16_t*)(bt_vm_data + my);
                break;
            case 0x40:
                if (ma >= 9996 || mx >= 9996 || my >= 9996) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint32_t*)(bt_vm_data + ma) =
                    *(uint32_t*)(bt_vm_data + mx) ==
                    *(uint32_t*)(bt_vm_data + my);
                break;
            case 0x60:
                if (ma >= 9992 || mx >= 9992 || my >= 9992) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint64_t*)(bt_vm_data + ma) =
                    *(uint64_t*)(bt_vm_data + mx) ==
                    *(uint64_t*)(bt_vm_data + my);
                break;
            }
            break;
        case OPERATOR_SL:
            switch (inst & INSTRUCTION_LENGTH) {
            case 0x0:
                bt_vm_data[ma] = bt_vm_data[mx] << bt_vm_data[my];
                break;
            case 0x20:
                if (ma >= 9998 || mx >= 9998 || my >= 9998) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint16_t*)(bt_vm_data + ma) =
                    *(uint16_t*)(bt_vm_data + mx) <<
                    *(uint16_t*)(bt_vm_data + my);
                break;
            case 0x40:
                if (ma >= 9996 || mx >= 9996 || my >= 9996) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint32_t*)(bt_vm_data + ma) =
                    *(uint32_t*)(bt_vm_data + mx) <<
                    *(uint32_t*)(bt_vm_data + my);
                break;
            case 0x60:
                if (ma >= 9992 || mx >= 9992 || my >= 9992) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                *(uint64_t*)(bt_vm_data + ma) =
                    *(uint64_t*)(bt_vm_data + mx) <<
                    *(uint64_t*)(bt_vm_data + my);
                break;
            }
            break;
        case OPERATOR_LT:
            switch (inst & INSTRUCTION_LENGTH) {
            case 0x0:
                if (!(inst & INSTRUCTION_SIGN)) {
                    bt_vm_data[ma] = bt_vm_data[mx] < bt_vm_data[my];
                } else {
                    bt_vm_data[ma] =
                        (int8_t)bt_vm_data[mx] < (int8_t)bt_vm_data[my];
                }
                break;
            case 0x20:
                if (ma >= 9998 || mx >= 9998 || my >= 9998) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                if (!(inst & INSTRUCTION_SIGN)) {
                    *(uint16_t*)(bt_vm_data + ma) =
                        *(uint16_t*)(bt_vm_data + mx) <
                        *(uint16_t*)(bt_vm_data + my);
                } else {
                    *(int16_t*)(bt_vm_data + ma) =
                        *(int16_t*)(bt_vm_data + mx) <
                        *(int16_t*)(bt_vm_data + my);
                }
                break;
            case 0x40:
                if (ma >= 9996 || mx >= 9996 || my >= 9996) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                if (!(inst & INSTRUCTION_SIGN)) {
                    *(uint32_t*)(bt_vm_data + ma) =
                        *(uint32_t*)(bt_vm_data + mx) <
                        *(uint32_t*)(bt_vm_data + my);
                } else {
                    *(int32_t*)(bt_vm_data + ma) =
                        *(int32_t*)(bt_vm_data + mx) <
                        *(int32_t*)(bt_vm_data + my);
                }
                break;
            case 0x60:
                if (ma >= 9992 || mx >= 9992 || my >= 9992) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                if (!(inst & INSTRUCTION_SIGN)) {
                    *(uint64_t*)(bt_vm_data + ma) =
                        *(uint64_t*)(bt_vm_data + mx) <
                        *(uint64_t*)(bt_vm_data + my);
                } else {
                    *(int64_t*)(bt_vm_data + ma) =
                        *(int64_t*)(bt_vm_data + mx) <
                        *(int64_t*)(bt_vm_data + my);
                }
                break;
            }
            break;
        case OPERATOR_GT:
            switch (inst & INSTRUCTION_LENGTH) {
            case 0x0:
                if (!(inst & INSTRUCTION_SIGN)) {
                    bt_vm_data[ma] = bt_vm_data[mx] > bt_vm_data[my];
                } else {
                    bt_vm_data[ma] =
                        (int8_t)bt_vm_data[mx] > (int8_t)bt_vm_data[my];
                }
                break;
            case 0x20:
                if (ma >= 9998 || mx >= 9998 || my >= 9998) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                if (!(inst & INSTRUCTION_SIGN)) {
                    *(uint16_t*)(bt_vm_data + ma) =
                        *(uint16_t*)(bt_vm_data + mx) >
                        *(uint16_t*)(bt_vm_data + my);
                } else {
                    *(int16_t*)(bt_vm_data + ma) =
                        *(int16_t*)(bt_vm_data + mx) >
                        *(int16_t*)(bt_vm_data + my);
                }
                break;
            case 0x40:
                if (ma >= 9996 || mx >= 9996 || my >= 9996) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                if (!(inst & INSTRUCTION_SIGN)) {
                    *(uint32_t*)(bt_vm_data + ma) =
                        *(uint32_t*)(bt_vm_data + mx) >
                        *(uint32_t*)(bt_vm_data + my);
                } else {
                    *(int32_t*)(bt_vm_data + ma) =
                        *(int32_t*)(bt_vm_data + mx) >
                        *(int32_t*)(bt_vm_data + my);
                }
                break;
            case 0x60:
                if (ma >= 9992 || mx >= 9992 || my >= 9992) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                if (!(inst & INSTRUCTION_SIGN)) {
                    *(uint64_t*)(bt_vm_data + ma) =
                        *(uint64_t*)(bt_vm_data + mx) >
                        *(uint64_t*)(bt_vm_data + my);
                } else {
                    *(int64_t*)(bt_vm_data + ma) =
                        *(int64_t*)(bt_vm_data + mx) >
                        *(int64_t*)(bt_vm_data + my);
                }
                break;
            }
            break;
        case OPERATOR_SR:
            switch (inst & INSTRUCTION_LENGTH) {
            case 0x0:
                if (!(inst & INSTRUCTION_SIGN)) {
                    bt_vm_data[ma] = bt_vm_data[mx] >> bt_vm_data[my];
                } else {
                    bt_vm_data[ma] =
                        (int8_t)bt_vm_data[mx] >> (int8_t)bt_vm_data[my];
                }
                break;
            case 0x20:
                if (ma >= 9998 || mx >= 9998 || my >= 9998) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                if (!(inst & INSTRUCTION_SIGN)) {
                    *(uint16_t*)(bt_vm_data + ma) =
                        *(uint16_t*)(bt_vm_data + mx) >>
                        *(uint16_t*)(bt_vm_data + my);
                } else {
                    *(int16_t*)(bt_vm_data + ma) =
                        *(int16_t*)(bt_vm_data + mx) >>
                        *(int16_t*)(bt_vm_data + my);
                }
                break;
            case 0x40:
                if (ma >= 9996 || mx >= 9996 || my >= 9996) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                if (!(inst & INSTRUCTION_SIGN)) {
                    *(uint32_t*)(bt_vm_data + ma) =
                        *(uint32_t*)(bt_vm_data + mx) >>
                        *(uint32_t*)(bt_vm_data + my);
                } else {
                    *(int32_t*)(bt_vm_data + ma) =
                        *(int32_t*)(bt_vm_data + mx) >>
                        *(int32_t*)(bt_vm_data + my);
                }
                break;
            case 0x60:
                if (ma >= 9992 || mx >= 9992 || my >= 9992) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                if (!(inst & INSTRUCTION_SIGN)) {
                    *(uint64_t*)(bt_vm_data + ma) =
                        *(uint64_t*)(bt_vm_data + mx) >>
                        *(uint64_t*)(bt_vm_data + my);
                } else {
                    *(int64_t*)(bt_vm_data + ma) =
                        *(int64_t*)(bt_vm_data + mx) >>
                        *(int64_t*)(bt_vm_data + my);
                }
                break;
            }
            break;
        case OPERATOR_LD:
            bt_vm_data[ma] = bt_vm_code[++pc];
            break;
        case OPERATOR_OUT:
            if (inst & INSTRUCTION_NUMERIC) {
                switch (inst & INSTRUCTION_LENGTH) {
                case 0x0:
                    printf("0x%02X", bt_vm_data[ma]);
                    break;
                case 0x20:
                    if (ma >= 9998) {
                        return ERRCODE_DATA_OVERFLOW;
                    }
                    printf(
                        "0x%04X",
                        *(uint16_t*)(bt_vm_data + ma)
                    );
                    break;
                case 0x40:
                    if (ma >= 9996) {
                        return ERRCODE_DATA_OVERFLOW;
                    }
                    printf(
                        "0x%08X",
                        *(uint32_t*)(bt_vm_data + ma)
                    );
                    break;
                case 0x60:
                    if (ma >= 9992) {
                        return ERRCODE_DATA_OVERFLOW;
                    }
                    printf(
                        "0x%016lX",
                        *(uint64_t*)(bt_vm_data + ma)
                    );
                    break;
                }
            } else {
                if (inst & INSTRUCTION_STR) {
                    printf("%s", bt_vm_data + ma);
                } else {
                    printf("%c", bt_vm_data[ma]);
                }
            }
            break;
        }
        if (verbose && n_cell) {
            puts("MEM:");
            iter = bt_vm_data;
            for (cnt = 0; cnt < n_cell; ++cnt, ++iter) {
                printf(
                    "0x%02X 0x%c\n",
                    *iter,
                    *iter >= 37 && *iter <= 127 ? *iter : '?'
                );
            }
        }
    }
    if (n_cell) {
        puts("MEM:");
        iter = bt_vm_data;
        for (cnt = 0; cnt < n_cell; ++cnt, ++iter) {
            printf(
                "0x%02X %c\n",
                *iter,
                *iter >= 37 && *iter <= 127 ? *iter : '?'
            );
        }
    }
    return ERRCODE_OK;
}
