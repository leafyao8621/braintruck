#include "parser.h"
#include "../util/errcode.h"
#include "../vm/vm.h"

#include <stdio.h>

#define STATE_SIGN 0xC0
#define STATE_LENGTH_ENABLE 0x20
#define STATE_LENGTH 0x18
#define STATE_REGISTER 0x6

unsigned char state;

void bt_parser_log(void) {
    unsigned char *iter = bt_vm_code;
    unsigned idx = 0;
    for (; idx < 10000 && *iter; ++idx, ++iter) {
        switch (*iter & INSTRUCTION_OPERATOR) {
        case OPERATOR_INCR:
            switch (*iter & INSTRUCTION_REGISTER) {
            case 0x0:
                printf("0x%04X 0x%02X a>\n", idx, *iter);
                break;
            case 0x20:
                printf("0x%04X 0x%02X x>\n", idx, *iter);
                break;
            case 0x40:
                printf("0x%04X 0x%02X y>\n", idx, *iter);
                break;
            }
            break;
        case OPERATOR_DECR:
            switch (*iter & INSTRUCTION_REGISTER) {
            case 0x0:
                printf("0x%04X 0x%02X a<\n", idx, *iter);
                break;
            case 0x20:
                printf("0x%04X 0x%02X x<\n", idx, *iter);
                break;
            case 0x40:
                printf("0x%04X 0x%02X y<\n", idx, *iter);
                break;
            }
            break;
        case OPERATOR_INC:
            switch (*iter & INSTRUCTION_LENGTH) {
            case 0x0:
                printf("0x%04X 0x%02X bi\n", idx, *iter);
                break;
            case 0x20:
                printf("0x%04X 0x%02X wi\n", idx, *iter);
                break;
            case 0x40:
                printf("0x%04X 0x%02X di\n", idx, *iter);
                break;
            case 0x60:
                printf("0x%04X 0x%02X qi\n", idx, *iter);
                break;
            }
            break;
        case OPERATOR_DEC:
            switch (*iter & INSTRUCTION_LENGTH) {
            case 0x0:
                printf("0x%04X 0x%02X bd\n", idx, *iter);
                break;
            case 0x20:
                printf("0x%04X 0x%02X wd\n", idx, *iter);
                break;
            case 0x40:
                printf("0x%04X 0x%02X dd\n", idx, *iter);
                break;
            case 0x60:
                printf("0x%04X 0x%02X qd\n", idx, *iter);
                break;
            }
            break;
        case OPERATOR_ADD:
            switch (*iter & INSTRUCTION_LENGTH) {
            case 0x0:
                printf("0x%04X 0x%02X b+\n", idx, *iter);
                break;
            case 0x20:
                printf("0x%04X 0x%02X w+\n", idx, *iter);
                break;
            case 0x40:
                printf("0x%04X 0x%02X d+\n", idx, *iter);
                break;
            case 0x60:
                printf("0x%04X 0x%02X q+\n", idx, *iter);
                break;
            }
            break;
        case OPERATOR_SUB:
            switch (*iter & INSTRUCTION_LENGTH) {
            case 0x0:
                printf("0x%04X 0x%02X b-\n", idx, *iter);
                break;
            case 0x20:
                printf("0x%04X 0x%02X w-\n", idx, *iter);
                break;
            case 0x40:
                printf("0x%04X 0x%02X d-\n", idx, *iter);
                break;
            case 0x60:
                printf("0x%04X 0x%02X q-\n", idx, *iter);
                break;
            }
            break;
        case OPERATOR_MULT:
            switch (*iter & INSTRUCTION_LENGTH) {
            case 0x0:
                printf("0x%04X 0x%02X b*\n", idx, *iter);
                break;
            case 0x20:
                printf("0x%04X 0x%02X w*\n", idx, *iter);
                break;
            case 0x40:
                printf("0x%04X 0x%02X d*\n", idx, *iter);
                break;
            case 0x60:
                printf("0x%04X 0x%02X q*\n", idx, *iter);
                break;
            }
            break;
        case OPERATOR_DIV:
            switch (*iter & INSTRUCTION_LENGTH) {
            case 0x0:
                printf("0x%04X 0x%02X b/\n", idx, *iter);
                break;
            case 0x20:
                printf("0x%04X 0x%02X w/\n", idx, *iter);
                break;
            case 0x40:
                printf("0x%04X 0x%02X d/\n", idx, *iter);
                break;
            case 0x60:
                printf("0x%04X 0x%02X q/\n", idx, *iter);
                break;
            }
            break;
        case OPERATOR_MOD:
            switch (*iter & INSTRUCTION_LENGTH) {
            case 0x0:
                printf("0x%04X 0x%02X b%%\n", idx, *iter);
                break;
            case 0x20:
                printf("0x%04X 0x%02X w%%\n", idx, *iter);
                break;
            case 0x40:
                printf("0x%04X 0x%02X d%%\n", idx, *iter);
                break;
            case 0x60:
                printf("0x%04X 0x%02X q%%\n", idx, *iter);
                break;
            }
            break;
        }
    }
}

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
        printf("chr: %c state: 0x%02X\n", chr >= 37 && chr <= 127 ? chr : '?', state);
        switch (chr) {
        case 'a':
            if (state & ~STATE_REGISTER) {
                if ((state & STATE_SIGN) || !(state & STATE_LENGTH_ENABLE)) {
                    fclose(fin);
                    return ERRCODE_INVALID;
                }
                if (idx++ == 9999) {
                    return ERRCODE_CODE_OVERFLOW;
                }
                *(iter++) = OPERATOR_LAND | ((state & STATE_LENGTH) << 2);
            } else {
                state &= ~STATE_REGISTER;
            }
            break;
        case 'x':
            if (state & ~STATE_REGISTER) {
                fclose(fin);
                return ERRCODE_INVALID;
            }
            state &= ~STATE_REGISTER;
            state |= 0x2;
            break;
        case 'y':
            if (state & ~STATE_REGISTER) {
                fclose(fin);
                return ERRCODE_INVALID;
            }
            state &= ~STATE_REGISTER;
            state |= 0x4;
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
            state &= ~(STATE_LENGTH_ENABLE | STATE_LENGTH);
            state |= 0x20;
            break;
        case 'w':
            if (state & ~STATE_REGISTER) {
                fclose(fin);
                return ERRCODE_INVALID;
            }
            state &= ~(STATE_LENGTH_ENABLE | STATE_LENGTH);
            state |= 0x28;
            break;
        case 'q':
            if (state & ~STATE_REGISTER) {
                fclose(fin);
                return ERRCODE_INVALID;
            }
            state &= ~(STATE_LENGTH_ENABLE | STATE_LENGTH);
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
            state &= ~(STATE_LENGTH_ENABLE | STATE_LENGTH);
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
                state &= ~(STATE_LENGTH_ENABLE | STATE_LENGTH);
            } else {
                state &= ~(STATE_LENGTH_ENABLE | STATE_LENGTH);
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
                state &= ~(STATE_LENGTH_ENABLE | STATE_LENGTH);
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
                state &= ~(STATE_LENGTH_ENABLE | STATE_LENGTH);
            } else {
                *(iter++) = OPERATOR_DEC;
            }
            break;
        case '*':
            if ((state & STATE_SIGN) || !(state & STATE_LENGTH_ENABLE)) {
                fclose(fin);
                return ERRCODE_INVALID;
            }
            if (idx++ == 9999) {
                return ERRCODE_CODE_OVERFLOW;
            }
            *(iter++) = OPERATOR_MULT | ((state & STATE_LENGTH) << 2);
            state &= ~(STATE_LENGTH_ENABLE | STATE_LENGTH);
            break;
        case '/':
            if ((state & STATE_SIGN) || !(state & STATE_LENGTH_ENABLE)) {
                fclose(fin);
                return ERRCODE_INVALID;
            }
            if (idx++ == 9999) {
                return ERRCODE_CODE_OVERFLOW;
            }
            *(iter++) = OPERATOR_DIV | ((state & STATE_LENGTH) << 2);
            state &= ~(STATE_LENGTH_ENABLE | STATE_LENGTH);
            break;
        case '%':
            if ((state & STATE_SIGN) || !(state & STATE_LENGTH_ENABLE)) {
                fclose(fin);
                return ERRCODE_INVALID;
            }
            if (idx++ == 9999) {
                return ERRCODE_CODE_OVERFLOW;
            }
            *(iter++) = OPERATOR_MOD | ((state & STATE_LENGTH) << 2);
            state &= ~(STATE_LENGTH_ENABLE | STATE_LENGTH);
            break;
        }
    }
    fclose(fin);
    return ERRCODE_OK;
}
