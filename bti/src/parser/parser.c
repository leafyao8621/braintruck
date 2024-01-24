#include "parser.h"
#include "../util/errcode.h"
#include "../vm/vm.h"

#include <stdio.h>

#define STATE_SIGN 0xC0
#define STATE_LENGTH_ENABLE 0x20
#define STATE_LENGTH 0x18
#define STATE_REGISTER 0x6

unsigned char state;

int bt_parser_parse(char *fn) {

    unsigned idx = 0;
    int chr = 0;
    unsigned char *iter = bt_vm_code;
    FILE *fin = fopen(fn, "r");
    if (!fin) {
        return ERRCODE_IO;
    }
    state = 0;
    for (chr = fgetc(fin); !feof(fin); chr = fgetc(fin)) {
        switch (chr) {
        case 'a':
            if (state) {
                if ((state & STATE_SIGN) || !(state & STATE_LENGTH_ENABLE)) {
                    fclose(fin);
                    return ERRCODE_INVALID;
                }
                if (idx++ == 9999) {
                    return ERRCODE_CODE_OVERFLOW;
                }
                *(iter++) = OPERATOR_LAND | ((state & STATE_LENGTH) << 2);
            } else {
                state |= 0x2;
            }
            break;
        case 'x':
            if (state & ~STATE_REGISTER) {
                fclose(fin);
                return ERRCODE_INVALID;
            }
            state |= 0x4;
            break;
        case 'y':
            if (state & ~STATE_REGISTER) {
                fclose(fin);
                return ERRCODE_INVALID;
            }
            state |= 0x6;
            break;
        case '>':
            if (state & ~STATE_REGISTER) {
                fclose(fin);
                return ERRCODE_INVALID;
            }
            if (idx++ == 9999) {
                return ERRCODE_CODE_OVERFLOW;
            }
            *(iter++) = OPERATOR_INCR | ((state & STATE_REGISTER) << 4);
            break;
        case '<':
            if (state & ~STATE_REGISTER) {
                fclose(fin);
                return ERRCODE_INVALID;
            }
            if (idx++ == 9999) {
                return ERRCODE_CODE_OVERFLOW;
            }
            *(iter++) = OPERATOR_DECR | ((state & STATE_REGISTER) << 4);
            break;
        case 'b':
            if (state & ~STATE_REGISTER) {
                fclose(fin);
                return ERRCODE_INVALID;
            }
            state |= 0x20;
            break;
        case 'w':
            if (state & ~STATE_REGISTER) {
                fclose(fin);
                return ERRCODE_INVALID;
            }
            state |= 0x28;
            break;
        case 'q':
            if (state & ~STATE_REGISTER) {
                fclose(fin);
                return ERRCODE_INVALID;
            }
            state |= 0x38;
            break;
        case 'i':
            if (!(state & STATE_LENGTH_ENABLE) || (state & STATE_SIGN)) {
                fclose(fin);
                return ERRCODE_INVALID;
            }
            if (idx++ == 9999) {
                return ERRCODE_CODE_OVERFLOW;
            }
            *(iter++) = OPERATOR_INC | ((state & STATE_LENGTH) << 2);
            break;
        case 'd':
            if (state & STATE_SIGN) {
                fclose(fin);
                return ERRCODE_INVALID;
            }
            if (state & STATE_LENGTH_ENABLE) {
                if (idx++ == 9999) {
                    return ERRCODE_CODE_OVERFLOW;
                }
                *(iter++) = OPERATOR_DEC | ((state & STATE_LENGTH) << 2);
            } else {
                state |= 0x30;
            }
            break;
        case '+':
            if (state & STATE_SIGN) {
                fclose(fin);
                return ERRCODE_INVALID;
            }
            if (idx++ == 9999) {
                return ERRCODE_CODE_OVERFLOW;
            }
            if (state & STATE_LENGTH_ENABLE) {
                *(iter++) = OPERATOR_ADD | ((state & STATE_LENGTH) << 2);
            } else {
                *(iter++) = OPERATOR_INC;
            }
            break;
        case '-':
            if (state & STATE_SIGN) {
                fclose(fin);
                return ERRCODE_INVALID;
            }
            if (idx++ == 9999) {
                return ERRCODE_CODE_OVERFLOW;
            }
            if (state & STATE_LENGTH_ENABLE) {
                *(iter++) = OPERATOR_SUB | ((state & STATE_LENGTH) << 2);
            } else {
                *(iter++) = OPERATOR_DEC;
            }
            break;
        }
    }
    fclose(fin);
    return ERRCODE_OK;
}
