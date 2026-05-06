#include "BinParser.hpp"
#include "RISC_VI_VIIUtils.hpp"
#include <algorithm>
#include <cassert>
#include <filesystem>
#include <elfio/elfio.hpp>

namespace KIRCulator {

void BinParser::parse(const std::filesystem::path &bin_path) {
    using namespace ELFIO;

    elfio reader;
    assert(reader.load(bin_path));

    auto *text_sec = reader.sections[".text"];
    assert(text_sec);

    const RISC_VI_VII::RawInstruction *text_data =
        reinterpret_cast<const RISC_VI_VII::RawInstruction *>(text_sec->get_data());
    assert(text_data);

    auto *symtab = reader.sections[".symtab"];
    assert(symtab);

    struct FuncSym { std::string name; Elf_Xword byte_off; Elf_Xword byte_size; };
    std::vector<FuncSym> func_syms;
    std::unordered_map<std::string, Elf_Xword> sym_byte_value;

    const symbol_section_accessor symbols(reader, symtab);
    for (unsigned i = 0; i < symbols.get_symbols_num(); ++i) {
        std::string name;
        Elf64_Addr value;
        Elf_Xword size;
        unsigned char bind, type, other;
        Elf_Half section_index;
        symbols.get_symbol(i, name, value, size, bind, type, section_index, other);

        if (!name.empty())
            sym_byte_value[name] = value;

        if (type == STT_FUNC && size > 0)
            func_syms.push_back({name, value, size});
    }

    std::sort(func_syms.begin(), func_syms.end(),
              [](const FuncSym &a, const FuncSym &b){ return a.byte_off < b.byte_off; });

    struct RelaEntry { std::string target_name; Elf_Sxword addend; };
    std::unordered_map<Elf_Xword, RelaEntry> rela_map;

    auto *rela_sec = reader.sections[".rela.text"];
    if (rela_sec) {
        const relocation_section_accessor relocs(reader, rela_sec);
        for (unsigned i = 0; i < relocs.get_entries_num(); ++i) {
            Elf64_Addr offset;
            Elf_Word sym_idx, reloc_type;
            Elf_Sxword addend;
            relocs.get_entry(i, offset, sym_idx, reloc_type, addend);

            std::string sym_name;
            Elf64_Addr sym_val;
            Elf_Xword sym_size;
            unsigned char sym_bind, sym_type, sym_other;
            Elf_Half sym_shndx;
            symbols.get_symbol(sym_idx, sym_name, sym_val, sym_size,
                               sym_bind, sym_type, sym_shndx, sym_other);

            rela_map[offset / RISC_VI_VII::INSTR_SIZE] = {sym_name, addend};
        }
    }

    auto parse_func = [&](const FuncSym &fs) {
        FuncInfo fi;
        fi.name = fs.name;

        Elf_Xword func_start = fs.byte_off / RISC_VI_VII::INSTR_SIZE;
        Elf_Xword func_size = fs.byte_size / RISC_VI_VII::INSTR_SIZE;

        fi.pc2bb[0] = "L0";
        fi.bb2pc["L0"] = 0;

        for (Elf_Xword local_pc = 0; local_pc < func_size; ++local_pc) {
            Elf_Xword global_instr = func_start + local_pc;
            RISC_VI_VII::RawInstruction raw = text_data[global_instr];

            Instr I;
            I.opcode = (raw >> 56) & 0xFF;
            I.r1 = (raw >> 52) & 0xF;
            I.r2 = (raw >> 48) & 0xF;
            I.r3imm = (int32_t)(raw & 0xFFFFFFFF);

            if (I.opcode == Instr::B ||
                I.opcode == Instr::BR_COND ||
                I.opcode == Instr::CALL) {
                auto rela_it = rela_map.find(global_instr);
                if (rela_it != rela_map.end()) {
                    const auto &re = rela_it->second;
                    Elf_Xword sym_byte = sym_byte_value.count(re.target_name)
                                        ? sym_byte_value[re.target_name] : 0;
                    int32_t patched = (int32_t)((int64_t)sym_byte + re.addend - (int64_t)(global_instr * RISC_VI_VII::INSTR_SIZE)) / 4;
                    I.r3imm = patched;
                    I.call_target = re.target_name;
                } else {
                    int32_t target_local = (int32_t)I.r3imm / 2 + (int32_t)local_pc;
                    I.r3imm = target_local;

                    if (target_local >= 0 && (Elf_Xword)target_local < func_size) {
                        std::string lbl = "L" + std::to_string(target_local);
                        fi.pc2bb[target_local] = lbl;
                        fi.bb2pc[lbl] = target_local;
                    }
                }
            }

            bool needs_fallthough = (I.opcode == Instr::BR_COND ||
                                    I.opcode == Instr::B ||
                                    I.opcode == Instr::CALL);
            if (needs_fallthough && local_pc + 1 < func_size) {
                uint32_t fall = (uint32_t)local_pc + 1;
                if (fi.pc2bb.find(fall) == fi.pc2bb.end()) {
                    std::string lbl = "L" + std::to_string(fall);
                    fi.pc2bb[fall] = lbl;
                    fi.bb2pc[lbl]  = fall;
                }
            }

            fi.insns.push_back(std::move(I));
        }

        fi.bb_names.reserve(fi.pc2bb.size());
        for (auto &[pc, name] : fi.pc2bb)
            fi.bb_names.push_back(name);
        std::sort(fi.bb_names.begin(), fi.bb_names.end(),
                  [&fi](const std::string &a, const std::string &b){
                      return fi.bb2pc.at(a) < fi.bb2pc.at(b);
                  });

        return fi;
    };

    FuncInfo *app_slot = nullptr;
    for (auto &fs : func_syms) {
        if (fs.name == "app") {
            funcs.push_back(parse_func(fs));
            app_slot = &funcs.back();
            break;
        }
    }
    for (auto &fs : func_syms) {
        if (fs.name != "app")
            funcs.push_back(parse_func(fs));
    }

    (void)app_slot;
}

} // namespace KIRCulator
