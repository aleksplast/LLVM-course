#pragma once

#include "Parser.hpp"

namespace KIRCulator {

struct BinParser final : public Parser {
    void parse(const std::filesystem::path &bin_path) override;
};

}
