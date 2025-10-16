target_compile_features(ir_gen PUBLIC cxx_std_20)
target_include_directories(ir_gen PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_sources(ir_gen PUBLIC main.cpp sim.c IRGen.cpp)

set(LLVM_LINK_COMPONENTS
  BitWriter
  Core
  IRReader
  Linker
  Support
  )

find_package(LLVM REQUIRED)
target_include_directories(ir_gen PUBLIC ${LLVM_INCLUDE_DIRS})
target_link_libraries(ir_gen PRIVATE LLVM SDL2)

find_package(CLI11 REQUIRED)
target_link_libraries(ir_gen PRIVATE CLI11::CLI11)
