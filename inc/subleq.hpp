#pragma once
#include <vector>
#include <cstddef>

const int INPUT_ADDR_MARKER = -1;
const int OUTPUT_ADDR_MARKER = -2;

class SubleqInterpreter {
	public:
		std::vector<int> memory;
		int memory_size;
		int program_counter;

		SubleqInterpreter(int size);

		void load_program(const std::vector<int>& program_code);

		bool step();

		void run();

		void dump_memory(int start, int end) const;

	protected:
		void _handle_branch(int value, int C_addr);

		virtual bool get_input(int& value);

		virtual void put_output(int value);
};

class SubleqInterpreterNonInteractive : public SubleqInterpreter {
	public:
		std::vector<int> input_vector;
		std::vector<int> output_vector;
		size_t input_ptr;

		SubleqInterpreterNonInteractive(int size, const std::vector<int>& input);

		void run(int max_steps);

	protected:
		bool get_input(int& value) override;

		void put_output(int value) override;
};
