#include "../subleq.hpp"
#include <cassert>
#include <iostream>
#include <vector>

void run_subtraction_test() {
    std::cout << "Running subtraction test..." << std::endl;
    std::vector<int> program = {
        20, -1, 3,  // Read input into M[20]
        21, -1, 6,  // Read input into M[21]
        20, 21, 9,  // M[20] = M[20] - M[21]
        -2, 20, 12, // Output M[20]
        -1, -1, -1  // Halt
    };
    std::vector<int> input = {8, 3};
    SubleqEmulatorNonInteractive emulator(30, input);
    emulator.load_program(program);
    emulator.run(100);

    assert(emulator.output_vector.size() == 1);
    assert(emulator.output_vector[0] == 5);
    std::cout << "Subtraction test passed!" << std::endl;
}

void run_addition_test() {
    std::cout << "Running addition test..." << std::endl;
    std::vector<int> program = {
        20, -1, 3,  // Read input into M[20]
        21, -1, 6,  // Read input into M[21]
        20, 21, 9,  // M[20] = M[20] - M[21]
        -2, 20, 12, // Output M[20]
        -1, -1, -1  // Halt
    };
    std::vector<int> input = {5, -3};
    SubleqEmulatorNonInteractive emulator(30, input);
    emulator.load_program(program);
    emulator.run(100);

    assert(emulator.output_vector.size() == 1);
    assert(emulator.output_vector[0] == 8);
    std::cout << "Addition test passed!" << std::endl;
}

void run_cat_program_test() {
    std::cout << "Running cat program test..." << std::endl;
    std::vector<int> program = {
        10, -1, 3, // Read input to M[10]
        -2, 10, 6, // Output M[10]
        15, 15, 0  // Unconditional jump to 0
    };
    std::vector<int> input = {10, 20, 30};
    SubleqEmulatorNonInteractive emulator(20, input);
    emulator.load_program(program);
    emulator.run(100);

    assert(emulator.output_vector.size() == 3);
    assert(emulator.output_vector[0] == 10);
    assert(emulator.output_vector[1] == 20);
    assert(emulator.output_vector[2] == 30);
    std::cout << "Cat program test passed!" << std::endl;
}

int main() {
    run_subtraction_test();
    run_addition_test();
    run_cat_program_test();
    return 0;
}