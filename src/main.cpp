#include <iostream>
#include <sstream>
#include <fstream>
#include <optional>
#include <vector>
#include <sstream>

#include "./arena.hpp"
#include "./generation.hpp"
#include "./tokenization.hpp"
#include "./parser.hpp"

using namespace std;


int main(int argc , char* argv[]) {
    if(argc != 2){
        cerr<<"Incorrect usage. Correct usage is..." << endl;
        cerr<<"hydro <input>" << endl;
        return EXIT_FAILURE;
    }

    string contents;
    {
        stringstream stream;
        fstream input(argv[1], ios::in);
        stream << input.rdbuf();
        contents = stream.str();
    }


    Tokenizer tokenizer(move(contents));
    vector<Token> tokens = tokenizer.tokenize();

    Parser parser(move(tokens));
    optional<NodeProg> prog = parser.parse_prog();

    if(!prog.has_value()){
        cerr<<"program sheri alla"<<endl;
        exit(EXIT_FAILURE);
    }

    Generator generator(prog.value());
    {
        fstream file("out.asm", ios::out);
        file << generator.gen_prog();
    }

    int result;

    result = system("nasm -felf64 out.asm -o out.o");
    if (result != 0) {
        cerr << "Error: nasm command failed with code " << result << endl;
        return EXIT_FAILURE;
    }

    result = system("ld -o out out.o");
    if (result != 0) {
        cerr << "Error: ld command failed with code " << result << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
