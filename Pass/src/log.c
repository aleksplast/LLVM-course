#include <stdio.h>

void insn_logger(char *insn_name, char *op_name) {
    printf("%s <- %s\n", insn_name, op_name);
}
