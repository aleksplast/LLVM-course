target_compile_features(kirculator PUBLIC cxx_std_20)
target_include_directories(kirculator PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_sources(kirculator PUBLIC CPU.cpp sim.c IRGen.cpp EmuIR.cpp FullIR.cpp InstrInfo.cpp BinParser.cpp main.cpp)

set(LLVM_LINK_COMPONENTS
  BitWriter
  Core
  IRReader
  Linker
  Support
  )

find_package(LLVM REQUIRED)
target_include_directories(kirculator PUBLIC ${LLVM_INCLUDE_DIRS})
target_link_libraries(kirculator PRIVATE LLVM SDL2)

find_package(CLI11 REQUIRED)
target_link_libraries(kirculator PRIVATE CLI11::CLI11)
