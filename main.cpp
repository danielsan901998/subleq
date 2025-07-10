#include <iostream> // Required for input/output operations (std::cout, std::cin, std::cerr)
#include <vector>   // Required for dynamic array (std::vector) to represent memory
#include <stdexcept> // Required for standard exception types (std::invalid_argument, std::runtime_error)

// Define special memory addresses for I/O operations.
// In Subleq, I/O is typically handled by using special addresses that the emulator
// intercepts. Here, we use negative integers as markers, as actual memory addresses
// for std::vector are non-negative.
const int INPUT_ADDR_MARKER = -1;  // Marker for reading input into a memory location
const int OUTPUT_ADDR_MARKER = -2; // Marker for printing the content of a memory location

/**
 * @class SubleqEmulator
 * @brief Implements a Subleq (Subtract and Branch if Less than or Equal to Zero)
 * single-instruction computer emulator.
 *
 * Subleq is a One-Instruction Set Computer (OISC) where the only instruction is:
 * SUBLEQ A, B, C
 * This instruction performs two actions:
 * 1. Subtracts the value at memory address B from the value at memory address A,
 * and stores the result back into memory address A (M[A] = M[A] - M[B]).
 * 2. If the new value at M[A] is less than or equal to zero (M[A] <= 0),
 * the program counter (PC) jumps to memory address C.
 * Otherwise, the PC increments by 3 (to the next instruction).
 *
 * Special I/O handling:
 * - If B_addr is INPUT_ADDR_MARKER (-1), the character input from stdin is
 * stored into M[A_addr]. The instruction then proceeds to the next instruction.
 * - If A_addr is OUTPUT_ADDR_MARKER (-2), the character value at M[B_addr] is
 * printed to stdout. The instruction then proceeds to the next instruction.
 */
class SubleqEmulator {
public:
    std::vector<int> memory;      // The main memory of the emulator
    int memory_size;              // Total size of the memory
    int program_counter;          // Points to the current instruction in memory

    /**
     * @brief Constructor for the SubleqEmulator.
     * @param size The total size of the memory in integer units.
     * @throws std::invalid_argument if the memory size is not positive.
     */
    SubleqEmulator(int size) : memory_size(size), program_counter(0) {
        if (size <= 0) {
            throw std::invalid_argument("Memory size must be positive.");
        }
        memory.resize(memory_size, 0); // Initialize all memory locations to 0
    }

    /**
     * @brief Loads a program into the emulator's memory.
     * The program starts at memory address 0.
     * @param program_code A vector of integers representing the Subleq program.
     * @throws std::runtime_error if the program is too large for the allocated memory.
     */
    void load_program(const std::vector<int>& program_code) {
        if (program_code.size() > static_cast<size_t>(memory_size)) {
            throw std::runtime_error("Program too large for memory.");
        }
        // Copy the program instructions into memory
        for (size_t i = 0; i < program_code.size(); ++i) {
            memory[i] = program_code[i];
        }
        program_counter = 0; // Reset program counter to the beginning of the program
        std::cout << "Program loaded. Memory size: " << memory_size << ", Program size: " << program_code.size() << std::endl;
    }

    /**
     * @brief Runs the loaded Subleq program.
     * The execution continues until the program counter points to an
     * invalid memory address or an error occurs.
     */
    void run() {
        std::cout << "Starting emulator execution..." << std::endl;
        // The main execution loop
        while (program_counter >= 0 && program_counter < memory_size) {
            // Each Subleq instruction consists of three operands: A, B, C
            // Check if there are enough memory locations for the current instruction
            if (program_counter + 2 >= memory_size) {
                std::cerr << "Error: Program counter out of bounds or incomplete instruction at memory address "
                          << program_counter << ". Halting." << std::endl;
                break; // Halt execution
            }

            // Fetch the three operands for the current instruction
            int A_addr = memory[program_counter];
            int B_addr = memory[program_counter + 1];
            int C_addr = memory[program_counter + 2];

            // --- Handle Special I/O Cases ---

            // Case 1: Output operation (M[B_addr] is printed)
            if (A_addr == OUTPUT_ADDR_MARKER) {
                // Ensure B_addr is a valid memory address for reading
                if (B_addr < 0 || B_addr >= memory_size) {
                    std::cerr << "Error: Invalid memory address for output source: " << B_addr << ". Halting." << std::endl;
                    break; // Halt execution
                }
                std::cout << static_cast<int>(memory[B_addr]); // Print the value at M[B_addr] as a character
                program_counter += 3; // Move to the next instruction
                continue; // Skip the rest of the instruction logic for this cycle
            }

            // Case 2: Input operation (character input is stored into M[A_addr])
            if (B_addr == INPUT_ADDR_MARKER) {
                // Ensure A_addr is a valid memory address for writing
                if (A_addr < 0 || A_addr >= memory_size) {
                    std::cerr << "Error: Invalid memory address for input destination: " << A_addr << ". Halting." << std::endl;
                    break; // Halt execution
                }
                int input_char;
                std::cin >> input_char; // Read a single character from stdin
                memory[A_addr] = input_char; // Store its ASCII value
                program_counter += 3; // Move to the next instruction
                continue; // Skip the rest of the instruction logic for this cycle
            }

            // --- Normal SUBLEQ Operation ---

            // Validate A_addr and B_addr for normal memory access
            if (A_addr < 0 || A_addr >= memory_size) {
                std::cerr << "Error: Invalid memory address A: " << A_addr << ". Halting." << std::endl;
                break; // Halt execution
            }
            if (B_addr < 0 || B_addr >= memory_size) {
                std::cerr << "Error: Invalid memory address B: " << B_addr << ". Halting." << std::endl;
                break; // Halt execution
            }

            // Perform the subtraction: M[A] = M[A] - M[B]
            memory[A_addr] = memory[A_addr] - memory[B_addr];

            // Conditional jump: If M[A] <= 0, jump to C_addr; otherwise, increment PC by 3
            if (memory[A_addr] <= 0) {
                program_counter = C_addr; // Jump to address C
            } else {
                program_counter += 3; // Move to the next instruction
            }
        }
        std::cout << "\nEmulator halted." << std::endl;
    }

    /**
     * @brief Dumps a specified range of memory to the console.
     * @param start The starting memory address for the dump.
     * @param end The ending memory address for the dump.
     */
    void dump_memory(int start, int end) const {
        std::cout << "\n--- Memory Dump (" << start << " to " << end << ") ---" << std::endl;
        for (int i = start; i <= end && i < memory_size; ++i) {
            std::cout << "[" << i << "]: " << memory[i] << std::endl;
        }
        std::cout << "--------------------------" << std::endl;
    }
};

/**
 * @brief Creates a simple Subleq program that takes a character input,
 * stores it in memory location 10, and then prints it back to the console.
 * This demonstrates both input and output functionalities.
 * @return A vector of integers representing the Subleq program.
 */
std::vector<int> create_echo_program() {
    std::vector<int> program;

    // Reserve memory for the program and data.
    // M[10] will be used to store the input character.

    // Instruction 1 (Program Counter = 0):
    // SUBLEQ M[10], INPUT_ADDR_MARKER, 3
    // Action: Read a character from stdin and store its ASCII value into M[10].
    // Next PC: 3 (unconditionally jump to the next instruction)
    program.push_back(10);             // A_addr: Memory address 10 (destination for input)
    program.push_back(INPUT_ADDR_MARKER); // B_addr: Special marker for input
    program.push_back(3);              // C_addr: Next instruction address

    // Instruction 2 (Program Counter = 3):
    // SUBLEQ M[10], OUTPUT_ADDR_MARKER, 6
    // Action: Print the character stored in M[10] to stdout.
    // Next PC: 6 (unconditionally jump to the next instruction)
    program.push_back(OUTPUT_ADDR_MARKER); // A_addr: Special marker for output
    program.push_back(10);              // B_addr: Memory address 10 (source for output)
    program.push_back(6);               // C_addr: Next instruction address

    // Instruction 3 (Program Counter = 6):
    // SUBLEQ -1, -1, -1
    // Action: Halt the emulator by attempting to jump to an invalid address (-1).
    program.push_back(-1); // A_addr: Invalid address
    program.push_back(-1); // B_addr: Invalid address
    program.push_back(-1); // C_addr: Invalid address (causes emulator to halt)
		for(int i = 0; i< 10; i++)
			program.push_back(0);

    return program;
}
std::vector<int> negate_program() {
    std::vector<int> program;

    // Reserve memory for the program and data.
    // M[10] will be used to store the input character.

    // Instruction 1 (Program Counter = 0):
    // SUBLEQ M[10], INPUT_ADDR_MARKER, 3
    // Action: Read a character from stdin and store its ASCII value into M[10].
    // Next PC: 3 (unconditionally jump to the next instruction)
    program.push_back(OUTPUT_ADDR_MARKER); // A_addr: Special marker for output
    program.push_back(INPUT_ADDR_MARKER); // B_addr: Special marker for input
    program.push_back(3);              // C_addr: Next instruction address

    // Instruction 3 (Program Counter = 6):
    // SUBLEQ -1, -1, -1
    // Action: Halt the emulator by attempting to jump to an invalid address (-1).
    program.push_back(-1); // A_addr: Invalid address
    program.push_back(-1); // B_addr: Invalid address
    program.push_back(-1); // C_addr: Invalid address (causes emulator to halt)
		for(int i = 0; i< 10; i++)
			program.push_back(0);

    return program;
}

/**
 * @brief Main function to demonstrate the Subleq emulator.
 */
int main() {
    try {
        // Create an emulator with 20 memory locations.
        // The memory size should be sufficient for the program and any data.
        SubleqEmulator emulator(20);

        // Create the echo program
        std::vector<int> echo_program = negate_program();

        // Load the program into the emulator's memory
        emulator.load_program(echo_program);

        // Dump the initial memory state to see the loaded program
        emulator.dump_memory(0, 15); // Dump relevant part of memory

        std::cout << "\nEnter a single character to echo: ";
        // Run the emulator. It will prompt for input and then print it.
        emulator.run();

        // Dump the memory after execution to see any changes (e.g., input character in M[10])
        emulator.dump_memory(0, 15);

    } catch (const std::exception& e) {
        // Catch and print any exceptions that occur during execution
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1; // Indicate an error
    }

    return 0; // Indicate successful execution
}

