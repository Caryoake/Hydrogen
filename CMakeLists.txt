cmake_minimum_required(VERSION 3.26)
project(hydrogen)

set(CMAKE_CXX_STANDARD 20)

add_custom_command(
        OUTPUT program.o
        COMMAND nasm -f elf64 -o program.o ${CMAKE_SOURCE_DIR}/program.asm
        DEPENDS program.asm
        COMMENT "Assembling program.asm with NASM"
        )

add_executable(hydro src/main.cpp
        src/tokenization.hpp
        src/parser.hpp
        src/arena.hpp)

