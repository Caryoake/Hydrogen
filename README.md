# Hydrogen

A C++ project which integrates NASM assembler and features Malayalam error messages, making it a unique and enjoyable experience for Malayali developers! This project combines C++ with assembly language, using NASM to assemble the generated code. 

## Project Structure

- `src/` - Contains the main source code files
  - `main.cpp` - Main entry point
  - `tokenization.hpp` - Tokenization header
  - `parser.hpp` - Parser header
  - `arena.hpp` - Arena header
  - `generation.hpp` - Code Generation(Assembly) header
- `test.hy` - A sample source file written in the custom Hydrogen language used to test tokenization and code generation
- `CMakeLists.txt` - CMake build configuration

## Features

- C++ and Assembly language integration
- NASM assembler for code generation
- Fun Malayalam error messages for a local touch
- CMake-based build system

## Requirements

- CMake 3.26 or higher
- C++20 compatible compiler
- NASM (Netwide Assembler)

## Building the Project

1. Create a build directory:
```bash
mkdir build
cd build
```

2. Generate build files:
```bash
cmake ..
```

3. Build the project:
```bash
cmake --build .
```

## Running the Project

After building the project, you can run it using the following command from the build directory:

```bash
./hydrogen ../test.hy
```

This will execute the program and process the sample Hydrogen language file (`test.hy`).

## Development

This project is built using CLion IDE and uses CMake as its build system. The project demonstrates the integration of C++ with assembly language, where the compiler generates code that is then assembled using NASM. The error messages are displayed in Malayalam, adding a fun and local touch to the development experience. 

## Project Background

This project was created as a fun way to combine C++ development with assembly language, while adding a personal touch with Malayalam error messages. It's designed to be both educational and entertaining, especially for Malayali developers who want to explore low-level programming concepts.

## Contributing

Contributions are welcome! Please feel free to:
1. Fork the repository
2. Create a new branch for your feature
3. Make your changes
4. Submit a pull request

Whether it's adding new features, fixing bugs, or improving documentation, all contributions are appreciated!

## License

MIT License

Copyright (c) 2024 [Karthik P]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. 