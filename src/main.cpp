#include "subleq.hpp"
#include "parser.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <assembly_file>" << '\n';
        return 1;
    }

    AssemblyParser parser;
    std::vector<int> program;
    try {
        program = parser.parse(argv[1]);
    } catch (const std::exception& e) {
        std::cerr << "Error parsing assembly file: " << e.what() << '\n';
        return 1;
    }

    SubleqInterpreter interpreter(1024);
    interpreter.load_program(program);
    interpreter.run();
    return 0;
}