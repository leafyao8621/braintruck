#include <stdio.h>
#include <stdlib.h>
#include "parser/parser.h"
#include "util/errcode.h"
#include "vm/vm.h"

int main(int argc, char **argv) {
    int ret = 0;
    bool verbose = false;
    int n_mem = 0;
    if (argc == 1) {
        puts("Usage: bti <file name> <memory slot> <verbose>");
        return 0;
    }
    ret = bt_parser_parse(argv[1]);
    if (ret) {
        printf("errcode: %d\nerr: %s\n", ret, bt_errcode_lookup[ret]);
        return 0;
    }
    if (argc >= 3) {
        n_mem = atoi(argv[2]);
        if (n_mem == -1) {
            puts("Usage: bti <file name> <memory slot> <verbose>");
            return 0;
        }
        if (argc == 4) {
            verbose = true;
        } else if (argc > 4) {
            puts("Usage: bti <file name> <memory slot> <verbose>");
            return 0;
        }
    }
    if (n_mem) {
        bt_parser_log();
    }
    ret = bt_vm_run(n_mem, verbose);
    if (ret) {
        printf("errcode: %d\nerr: %s\n", ret, bt_errcode_lookup[ret]);
    }
    return 0;
}
