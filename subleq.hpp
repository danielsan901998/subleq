#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>

const int INPUT_ADDR_MARKER = -1;
const int OUTPUT_ADDR_MARKER = -2;

class SubleqEmulator {
public:
    std::vector<int> memory;
    int memory_size;
    int program_counter;

    SubleqEmulator(int size) : memory_size(size), program_counter(0) {
        if (size <= 0) {
            throw std::invalid_argument("Memory size must be positive.");
        }
        memory.resize(memory_size, 0);
    }

    void load_program(const std::vector<int>& program_code) {
        if (program_code.size() > static_cast<size_t>(memory_size)) {
            throw std::runtime_error("Program too large for memory.");
        }
        for (size_t i = 0; i < program_code.size(); ++i) {
            memory[i] = program_code[i];
        }
        program_counter = 0;
    }

    bool step() {
        if (program_counter < 0 || program_counter + 2 >= memory_size) {
            return false; // Halt condition
        }

        int A_addr = memory[program_counter];
        int B_addr = memory[program_counter + 1];
        int C_addr = memory[program_counter + 2];

        if (A_addr == OUTPUT_ADDR_MARKER && B_addr == INPUT_ADDR_MARKER) {
            // Special case: directly move input to output
            int input_value;
            if (!(std::cin >> input_value)) {
                return false; // Halt on input error
            }
						input_value=-input_value;
            std::cout << input_value << std::endl;
            if (input_value <= 0) {
                program_counter = C_addr;
            } else {
                program_counter += 3;
            }
            return true;
        }

        if (A_addr == OUTPUT_ADDR_MARKER) {
            if (B_addr < 0 || B_addr >= memory_size) return false;
            int value = memory[B_addr];
            std::cout << value << std::endl;
            if (value <= 0) {
                program_counter = C_addr;
            } else {
                program_counter += 3;
            }
            return true;
        }

        if (B_addr == INPUT_ADDR_MARKER) {
            if (A_addr < 0 || A_addr >= memory_size) return false;
            int input_value;
            if (!(std::cin >> input_value)) {
                return false; // Halt on input error
            }
            memory[A_addr] = input_value;
            program_counter += 3;
            return true;
        }

        if (A_addr < 0 || A_addr >= memory_size || B_addr < 0 || B_addr >= memory_size) {
            return false; // Halt condition
        }

        memory[A_addr] -= memory[B_addr];

        if (memory[A_addr] <= 0) {
            program_counter = C_addr;
        } else {
            program_counter += 3;
        }
        return true;
    }

    void run() {
        while (step()) {}
    }

    void dump_memory(int start, int end) const {
        std::cout << "--- Memory Dump (" << start << " to " << end << ") ---" << std::endl;
        for (int i = start; i <= end && i < memory_size; ++i) {
            std::cout << "[" << i << "]: " << memory[i] << std::endl;
        }
        std::cout << "--------------------------" << std::endl;
    }
};

class SubleqEmulatorNonInteractive : public SubleqEmulator {
public:
    std::vector<int> input_vector;
    std::vector<int> output_vector;
    size_t input_ptr;

    SubleqEmulatorNonInteractive(int size, const std::vector<int>& input) : SubleqEmulator(size), input_vector(input), input_ptr(0) {}

    bool step() {
        if (program_counter < 0 || program_counter >= memory_size || program_counter + 2 >= memory_size) {
            return false; // Halt condition
        }

        int A_addr = memory[program_counter];
        int B_addr = memory[program_counter + 1];
        int C_addr = memory[program_counter + 2];

        if (A_addr == OUTPUT_ADDR_MARKER) {
            int value;
            if (B_addr == INPUT_ADDR_MARKER) {
                if (input_ptr >= input_vector.size()) return false;
                value = -input_vector[input_ptr++];
            } else {
                if (B_addr < 0 || B_addr >= memory_size) return false;
                value = memory[B_addr];
            }
            output_vector.push_back(value);
            if (value <= 0) {
                program_counter = C_addr;
            } else {
                program_counter += 3;
            }
            return true;
        }

        if (B_addr == INPUT_ADDR_MARKER) {
            if (A_addr < 0 || A_addr >= memory_size) return false;
            if (input_ptr >= input_vector.size()) return false;
            memory[A_addr] = input_vector[input_ptr++];
            program_counter += 3;
            return true;
        }

        if (A_addr < 0 || A_addr >= memory_size || B_addr < 0 || B_addr >= memory_size) {
            return false; // Halt condition
        }

        memory[A_addr] -= memory[B_addr];

        if (memory[A_addr] <= 0) {
            program_counter = C_addr;
        } else {
            program_counter += 3;
        }
        return true;
    }

    void run(int max_steps) {
        for (int i = 0; i < max_steps && step(); ++i) {}
    }
};
