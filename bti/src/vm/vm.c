#include <stdio.h>

#include "vm.h"
#include "../util/errcode.h"

uint8_t bt_vm_code[10000] = {0};
uint8_t bt_vm_data[10000] = {0};

int bt_vm_run(uint16_t n_cell) {
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
                if (ma++ > 10000) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                break;
            case 0x20:
                if (mx++ > 10000) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                break;
            case 0x40:
                if (my++ > 10000) {
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
                if (ma >= 9999) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                ++*(uint16_t*)(bt_vm_data + ma);
                break;
            case 0x40:
                if (ma >= 9997) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                ++*(uint32_t*)(bt_vm_data + ma);
                break;
            case 0x60:
                if (ma >= 9993) {
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
                if (ma >= 9999) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                --*(uint16_t*)(bt_vm_data + ma);
                break;
            case 0x40:
                if (ma >= 9997) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                --*(uint32_t*)(bt_vm_data + ma);
                break;
            case 0x60:
                if (ma >= 9993) {
                    return ERRCODE_DATA_OVERFLOW;
                }
                --*(uint64_t*)(bt_vm_data + ma);
                break;
            }
            break;
        }
        if (n_cell) {
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
    return ERRCODE_OK;
}
