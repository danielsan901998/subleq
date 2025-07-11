#include "parser.hpp"
#include "subleq.hpp" // For INPUT_ADDR_MARKER and OUTPUT_ADDR_MARKER
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

// Helper function to resolve operand values
int resolve_operand(const std::string& operand_str_orig, const std::map<std::string, int>& symbolTable) {
    std::string operand_str = operand_str_orig;
    // Remove any trailing non-alphanumeric characters from the operand string
    while (!operand_str.empty() && !std::isalnum(operand_str.back())) {
        operand_str.pop_back();
    }

    if (operand_str == "@IN") {
        return INPUT_ADDR_MARKER;
    } else if (operand_str == "@OUT") {
        return OUTPUT_ADDR_MARKER;
    } else {
        try {
            return std::stoi(operand_str);
        } catch (const std::invalid_argument& e) {
            // Not a simple integer, check for label expressions
            size_t plus_pos = operand_str.find('+');
            size_t minus_pos = operand_str.find('-');

            if (plus_pos != std::string::npos) {
                std::string label_str = operand_str.substr(0, plus_pos);
                std::string offset_str = operand_str.substr(plus_pos + 1);
                if (symbolTable.count(label_str)) {
                    return symbolTable.at(label_str) + std::stoi(offset_str);
                } else {
                    throw std::runtime_error("Undefined symbol in expression: " + label_str);
                }
            } else if (minus_pos != std::string::npos) {
                std::string label_str = operand_str.substr(0, minus_pos);
                std::string offset_str = operand_str.substr(minus_pos + 1);
                if (symbolTable.count(label_str)) {
                    return symbolTable.at(label_str) - std::stoi(offset_str);
                } else {
                    throw std::runtime_error("Undefined symbol in expression: " + label_str);
                }
            } else {
                // Not a number or expression, assume it's a label
                if (symbolTable.count(operand_str)) {
                    return symbolTable.at(operand_str);
                } else {
                    throw std::runtime_error("Undefined symbol: " + operand_str);
                }
            }
        } catch (const std::out_of_range& e) {
            throw std::runtime_error("Number out of range: " + operand_str);
        }
    }
}


std::vector<int> AssemblyParser::parse(const std::string& filename) {
    first_pass(filename);
    second_pass(filename);
    return machine_code;
}

void AssemblyParser::first_pass(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        ss >> token;

        if (token.empty() || token[0] == ';') {
            continue;
        }

        if (token.back() == ':') {
            std::string label = token.substr(0, token.size() - 1);
            // Remove any trailing non-alphanumeric characters from the label
            while (!label.empty() && !std::isalnum(label.back())) {
                label.pop_back();
            }
            symbolTable[label] = location_counter;
            // Read the next token, which should be the instruction/directive
            ss >> token;
        }

        if (token == "subleq") {
            location_counter += 3;
        } else if (token == ".data") {
            location_counter++;
        }
    }
}

void AssemblyParser::second_pass(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::string line;
    int current_address = 0;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        ss >> token;

        if (token.empty() || token[0] == ';') {
            continue;
        }

        if (token.back() == ':') {
            // Remove any trailing non-alphanumeric characters from the label
            std::string label_token = token.substr(0, token.size() - 1);
            while (!label_token.empty() && !std::isalnum(label_token.back())) {
                label_token.pop_back();
            }
            // We don't need to use label_token here, just consume the next token
            ss >> token;
        }

        if (token == "subleq") {
            std::string a_str, b_str;
            ss >> a_str >> b_str;

            std::string rest_of_line;
            std::getline(ss, rest_of_line);

            // Find the start of the third argument (if any) or a comment
            size_t first_char_pos = rest_of_line.find_first_not_of(" \t");
            if (first_char_pos != std::string::npos) {
                if (rest_of_line[first_char_pos] == ';') {
                    // Only a comment, no third argument
                    machine_code.push_back(resolve_operand(a_str, symbolTable));
                    machine_code.push_back(resolve_operand(b_str, symbolTable));
                    machine_code.push_back(current_address + 3);
                } else {
                    // There is a third argument
                    std::string c_str_with_comment = rest_of_line.substr(first_char_pos);
                    size_t comment_pos = c_str_with_comment.find(';');
                    std::string c_str = c_str_with_comment.substr(0, comment_pos);
                    
                    // Trim trailing whitespace from c_str
                    size_t last_char = c_str.find_last_not_of(" \t");
                    if (last_char != std::string::npos) {
                        c_str = c_str.substr(0, last_char + 1);
                    } else {
                        c_str = "";
                    }

                    machine_code.push_back(resolve_operand(a_str, symbolTable));
                    machine_code.push_back(resolve_operand(b_str, symbolTable));
                    machine_code.push_back(resolve_operand(c_str, symbolTable));
                }
            } else {
                // No third argument or comment
                machine_code.push_back(resolve_operand(a_str, symbolTable));
                machine_code.push_back(resolve_operand(b_str, symbolTable));
                machine_code.push_back(current_address + 3);
            }
            current_address += 3;
        } else if (token == ".data") {
            std::string value_str;
            ss >> value_str;
            machine_code.push_back(resolve_operand(value_str, symbolTable));
            current_address++;
        }
    }
}
