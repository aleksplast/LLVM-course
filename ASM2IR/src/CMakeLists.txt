target_compile_features(asm2ir PUBLIC cxx_std_20)
target_include_directories(asm2ir PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_sources(asm2ir PUBLIC CPU.cpp sim.c IRGen.cpp ASMParser.cpp EmuIR.cpp InstrInfo.cpp main.cpp)

set(LLVM_LINK_COMPONENTS
  BitWriter
  Core
  IRReader
  Linker
  Support
  )

find_package(LLVM REQUIRED)
target_include_directories(asm2ir PUBLIC ${LLVM_INCLUDE_DIRS})
target_link_libraries(asm2ir PRIVATE LLVM SDL2)

find_package(CLI11 REQUIRED)
target_link_libraries(asm2ir PRIVATE CLI11::CLI11)
