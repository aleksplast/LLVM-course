### GRAPHIC APP

## Description

This directory contains files needed to build graphic app, that implements John Conway's `Game of life`. Starting state of a game is determined randomly, 1 to 3 chance of cell being alive at the start. Each cell has its own age, at each iteration, if cell is becoming alive from dead ones, its age is set to 1. If it continues being alive, its age is incremented by 1, if the cell is dead, its age is 0. Colours for cells is picked differently, according to age:
1. 0 - Black
2. 1 - Green
3. 2 - Blue
4. 3 - Red
5. 4 and more - White

Also, you can find LLVM-IR file, acquired from app.c source code, which is main logic when painting a frame.

## Build

To build and run this graphic app you will need SDL2 library installed:

`sudo apt install libsdl2-dev`

```
mkdir build && cd build
cmake ..
make
```

To run resulting app:
`./graphic_app`
