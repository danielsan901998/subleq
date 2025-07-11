#include "../subleq.hpp"
#include "../parser.hpp"
#include <cassert>
#include <iostream>
#include <vector>

void run_subtraction_test() {
    std::cout << "Running subtraction test..." << std::endl;
    AssemblyParser parser;
    std::vector<int> program;
    try {
        program = parser.parse("tests/asm/subtraction.asm");
    } catch (const std::exception& e) {
        std::cerr << "Error parsing subtraction.asm: " << e.what() << std::endl;
        assert(false && "Parser failed for subtraction.asm");
    }

    std::vector<int> input = {8, 3};
    SubleqInterpreterNonInteractive interpreter(30, input);
    interpreter.load_program(program);
    interpreter.run(100);

    assert(interpreter.output_vector.size() == 1);
    assert(interpreter.output_vector[0] == 5);
    std::cout << "Subtraction test passed!" << std::endl;
}

void run_addition_test() {
    std::cout << "Running addition test..." << std::endl;
    AssemblyParser parser;
    std::vector<int> program;
    try {
        program = parser.parse("tests/asm/addition.asm");
    } catch (const std::exception& e) {
        std::cerr << "Error parsing addition.asm: " << e.what() << std::endl;
        assert(false && "Parser failed for addition.asm");
    }

    std::vector<int> input = {5, 3};
    SubleqInterpreterNonInteractive interpreter(30, input);
    interpreter.load_program(program);
    interpreter.run(100);

    assert(interpreter.output_vector.size() == 1);
    assert(interpreter.output_vector[0] == 8);
    std::cout << "Addition test passed!" << std::endl;
}

void run_cat_program_test() {
    std::cout << "Running cat program test..." << std::endl;
    AssemblyParser parser;
    std::vector<int> program;
    try {
        program = parser.parse("tests/asm/cat.asm");
    } catch (const std::exception& e) {
        std::cerr << "Error parsing cat.asm: " << e.what() << std::endl;
        assert(false && "Parser failed for cat.asm");
    }

    std::vector<int> input = {10, 20, 30, 0, -5};
    SubleqInterpreterNonInteractive interpreter(20, input);
    interpreter.load_program(program);
    interpreter.run(100);

    assert(interpreter.output_vector.size() == 5);
    assert(interpreter.output_vector[0] == 10);
    assert(interpreter.output_vector[1] == 20);
    assert(interpreter.output_vector[2] == 30);
    assert(interpreter.output_vector[3] == 0);
    assert(interpreter.output_vector[4] == -5);
    std::cout << "Cat program test passed!" << std::endl;
}

void run_sequence_reverser_test() {
    std::cout << "Running stack example test..." << std::endl;
    AssemblyParser parser;
    std::vector<int> program;
    try {
        program = parser.parse("tests/asm/sequence_reverser.asm");
    } catch (const std::exception& e) {
        std::cerr << "Error parsing stack_example.asm: " << e.what() << std::endl;
        assert(false && "Parser failed for stack_example.asm");
    }

    std::vector<int> input = {10, 20, 30, 0};
    SubleqInterpreterNonInteractive interpreter(100, input);
    interpreter.load_program(program);
    interpreter.run(1000);

    assert(interpreter.output_vector.size() == 4);
    assert(interpreter.output_vector[0] == 30);
    assert(interpreter.output_vector[1] == 20);
    assert(interpreter.output_vector[2] == 10);
    assert(interpreter.output_vector[3] == 0);
    std::cout << "Stack example test passed!" << std::endl;
}

int main() {
    run_subtraction_test();
    run_addition_test();
    run_cat_program_test();
    run_sequence_reverser_test();
    return 0;
}
