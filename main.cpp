#include "subleq.hpp"

int main() {
    SubleqEmulator emulator(1024);
    std::vector<int> program = {
        -2, -1, 0
    };
    emulator.load_program(program);
    emulator.run();
    return 0;
}