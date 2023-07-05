# cs3113-game-programming
[Intro to Game Programming](https://github.com/sebastianromerocruz/CS3113-material)

## Prerequisites
* CMake 3.10 or later
* C++14 compiler
* SDL2
* SDL2_image
* OpenGL

## Build
1. Clone this repo
2. Create a build directory.
3. Compile with CMake.
4. Copy all files in /assets into the build directory.

For instance:
```shell
git clone https://github.com/sigsith/cs3113-game-programming.git && cd cs3113-game-programming
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
cp -r ../assets/* .
```