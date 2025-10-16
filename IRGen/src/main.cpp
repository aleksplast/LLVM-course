#include "IRGen.hpp"
#include <CLI/CLI.hpp>
#include <filesystem>

int main(int argc, char **argv) {
    CLI::App app{"IR Generator"};

    std::optional<std::filesystem::path> out_path;
    app.add_option("-o", out_path);

    IRGen::Generator gen{};
    gen.generate();
    gen.print(out_path);
    gen.interpret();
}
