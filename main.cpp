#include "subleq.hpp"
#include <cassert>

// Simple program to test subtraction
std::vector<int> create_subtraction_test_program() {
    std::vector<int> program;
    // M[10] = 5, M[11] = 3
    // SUBLEQ M[10], M[11], 3 ; M[10] = 5 - 3 = 2. PC becomes 3
    program.push_back(10);
    program.push_back(11);
    program.push_back(3);
    // HALT
    program.push_back(-1);
    program.push_back(-1);
    program.push_back(-1);
	for(int i = 0; i < 4; i++)
		program.push_back(0);
	program.push_back(5);
	program.push_back(3);

    return program;
}

void run_subtraction_test() {
    SubleqEmulator emulator(20);
    std::vector<int> program = create_subtraction_test_program();
    emulator.load_program(program);

    emulator.step(); // Execute one instruction

    assert(emulator.memory[10] == 2);
    std::cout << "Subtraction test passed!" << std::endl;
}

int main() {
    run_subtraction_test();
    return 0;
}