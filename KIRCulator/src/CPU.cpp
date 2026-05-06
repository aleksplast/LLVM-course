#include "CPU.hpp"
#include "Instr.hpp"
#include "Parser.hpp"
#include <iostream>
#include <algorithm>
#include <assert.h>

extern "C" {
    void sim_init();
    void sim_fini();
}

namespace KIRCulator {
    CPU *CPU::cpu;

void CPU::dump_cpu_state() const {
    std::cout << "pc: " << std::hex << pc << '\n';
    std::cout << "next_pc: " << next_pc << '\n';
    for (uint32_t reg_i = 0; reg_i < kRegNum; ++reg_i) {
        std::cout << "x" << reg_i << ": " << reg_file[reg_i] << '\n';
    }
    std::cout << '\n';
}

void CPU::exec(const Parser &parser) {
    next_pc = 0;
    bool run = 1;
    set_cpu(this);

    reg_file[9] = CPU::kStackSize;
    sim_init();

    const auto *run_func = &parser.funcs.front();
    while (run) {
        assert(run_func);
        auto &I = run_func->insns[pc];
        switch (I.opcode) {
        default:
        return;
    #define ISA(Opcode_, Name_, SkipArgs_, ReadArgs_, WriteArgs_, Execute_,       \
                IRGenExecute_)                                                     \
    case (Opcode_):                                                                \
        do_##Name_(I.r1, I.r2, I.r3imm);                                           \
        break;
    #include "ISA.hpp"
    #undef ISA
        }
        if (!I.call_target.empty()) {
            auto it = std::find_if(parser.funcs.begin(), parser.funcs.end(), [&I](const auto &func) {
                return func.name == I.call_target;
            });

            assert(it != parser.funcs.end());
            callstack.emplace_back(run_func->name);
            run_func = &*it;
        } else if (I.opcode == Instr::BR) {
            assert(!callstack.empty());
            std::string ret_func_name = callstack.back();
            callstack.pop_back();
            auto it = std::find_if(parser.funcs.begin(), parser.funcs.end(), [&ret_func_name](const auto &func) {
                return func.name == ret_func_name;
            });
            assert(it != parser.funcs.end());
            run_func = &*it;
        }
        pc = next_pc;
    }

    sim_fini();
    return;
}
} // namespace KIRCulator
