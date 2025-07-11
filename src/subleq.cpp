#include "subleq.hpp"
#include <stdexcept>
#include <iostream>

SubleqInterpreter::SubleqInterpreter(int size) : memory_size(size), program_counter(0) {
    if (size <= 0) {
        throw std::invalid_argument("Memory size must be positive.");
    }
    memory.resize(memory_size, 0);
}

void SubleqInterpreter::load_program(const std::vector<int>& program_code) {
    if (program_code.size() > static_cast<size_t>(memory_size)) {
        throw std::runtime_error("Program too large for memory.");
    }
    for (size_t i = 0; i < program_code.size(); ++i) {
        memory[i] = program_code[i];
    }
    program_counter = 0;
}

bool SubleqInterpreter::step() {
    if (program_counter < 0 || program_counter + 2 >= memory_size) {
        return false; // Halt condition
    }

    int A_addr = memory[program_counter];
    int B_addr = memory[program_counter + 1];
    int C_addr = memory[program_counter + 2];

    if (A_addr == OUTPUT_ADDR_MARKER) {
        int value;
        if (B_addr == INPUT_ADDR_MARKER) {
            int input_value;
            if (!get_input(input_value)) {
                return false; // Halt on input error
            }
            value = -input_value;
        } else {
            if (B_addr < 0 || B_addr >= memory_size) return false;
            value = -memory[B_addr];
        }
        put_output(value);
        _handle_branch(value, C_addr);
        return true;
    }

    if (B_addr == INPUT_ADDR_MARKER) {
        if (A_addr < 0 || A_addr >= memory_size) return false;
        int input_value;
        if (!get_input(input_value)) {
            return false; // Halt on input error
        }
        memory[A_addr] -= input_value;
        program_counter += 3;
        return true;
    }

    if (A_addr < 0 || A_addr >= memory_size || B_addr < 0 || B_addr >= memory_size) {
        return false; // Halt condition
    }

    memory[A_addr] -= memory[B_addr];

    _handle_branch(memory[A_addr], C_addr);
    return true;
}

void SubleqInterpreter::run() {
    while (step()) {}
}

void SubleqInterpreter::dump_memory(int start, int end) const {
    std::cout << "--- Memory Dump (" << start << " to " << end << ") ---\n";
    for (int i = start; i <= end && i < memory_size; ++i) {
        std::cout << memory[i] << ' ';
    }
    std::cout << "\n--------------------------\n";
}

void SubleqInterpreter::_handle_branch(int value, int C_addr) {
    if (value <= 0) {
        program_counter = C_addr;
    } else {
        program_counter += 3;
    }
}

bool SubleqInterpreter::get_input(int& value) {
    return bool(std::cin >> value);
}

void SubleqInterpreter::put_output(int value) {
    std::cout << value << '\n';
}

SubleqInterpreterNonInteractive::SubleqInterpreterNonInteractive(int size, const std::vector<int>& input) : SubleqInterpreter(size), input_vector(input), input_ptr(0) {}

void SubleqInterpreterNonInteractive::run(int max_steps) {
    for (int i = 0; i < max_steps && step(); ++i) {}
}

bool SubleqInterpreterNonInteractive::get_input(int& value) {
    if (input_ptr >= input_vector.size()) {
        return false;
    }
    value = input_vector[input_ptr++];
    return true;
}

void SubleqInterpreterNonInteractive::put_output(int value) {
    output_vector.push_back(value);
}
