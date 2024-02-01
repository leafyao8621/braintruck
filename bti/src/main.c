#include <stdio.h>
#include "parser/parser.h"
#include "util/errcode.h"
#include "vm/vm.h"

int main(void) {
    int ret = bt_parser_parse("../scripts/cnt_up.bt");
    printf("errcode: %d\nerr: %s\n", ret, bt_errcode_lookup[ret]);
    if (ret) {
        return 0;
    }
    bt_parser_log();
    ret = bt_vm_run(47, 0);
    printf("errcode: %d\nerr: %s\n", ret, bt_errcode_lookup[ret]);
    if (ret) {
        return 0;
    }
    return 0;
}
