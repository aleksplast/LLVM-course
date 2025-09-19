#include "sim.h"

int main() {
    sim_init();
    app();
    sim_fini();

    return 0;
}
