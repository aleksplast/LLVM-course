cmake_minimum_required(VERSION 3.14)
project(lang2ir LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_program(LLVM_CONFIG llvm-config REQUIRED)
find_program(JAVA java REQUIRED)

execute_process(
    COMMAND ${LLVM_CONFIG} --cppflags
    OUTPUT_VARIABLE LLVM_CPPFLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND ${LLVM_CONFIG} --ldflags
    OUTPUT_VARIABLE LLVM_LDFLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND ${LLVM_CONFIG} --libs
    OUTPUT_VARIABLE LLVM_LIBS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(GRAMMAR_NAME "SilverLang")
execute_process(
    COMMAND ${JAVA} -jar ${CMAKE_CURRENT_SOURCE_DIR}/tools/antlr.jar -Dlanguage=Cpp -visitor -no-listener -o ${CMAKE_CURRENT_SOURCE_DIR}/generated ${CMAKE_CURRENT_SOURCE_DIR}/grammar/${GRAMMAR_NAME}.g4
)

separate_arguments(LLVM_CPPFLAGS_LIST NATIVE_COMMAND ${LLVM_CPPFLAGS})
add_compile_options(${LLVM_CPPFLAGS_LIST})

find_package(SDL2 REQUIRED)

file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sim.c
    ${CMAKE_CURRENT_SOURCE_DIR}/generated/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

message(${SOURCES})

add_executable(lang2ir)
target_sources(lang2ir PUBLIC ${SOURCES})

target_include_directories(lang2ir PRIVATE
    /usr/local/include/antlr4-runtime
    generated
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${SDL2_INCLUDE_DIRS}
)

target_link_directories(lang2ir PRIVATE /usr/local/lib)

target_link_libraries(lang2ir PRIVATE antlr4-runtime)

if(SDL2_LIBRARIES)
    target_link_libraries(lang2ir PRIVATE ${SDL2_LIBRARIES})
else()
    target_link_libraries(lang2ir PRIVATE ${SDL2_LDFLAGS})
endif()

target_link_options(lang2ir PRIVATE ${LLVM_LDFLAGS})
target_link_libraries(lang2ir PRIVATE ${LLVM_LIBS})
