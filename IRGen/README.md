## IR Generator

### Description

This directory contains implementation of IR generator. This generator generates IR module, similar to `app.ll` file, you can find in `graphic_app` directory. Generator can also print resulting module into specified file and interpret it, using LLVM Execution Engine.

## Usage

1. Build
```
mkdir build && cd build
cmake .. && make
```

2. Run generator
```
./src/ir_gen -o out.ll
```
