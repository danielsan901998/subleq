#pragma once
#include <string>
#include <vector>
#include <map>

class AssemblyParser {
public:
    std::vector<int> parse(const std::string& filename);

private:
    void first_pass(const std::string& filename);
    void second_pass(const std::string& filename);

    std::map<std::string, int> symbolTable;
    std::vector<int> machine_code;
    int location_counter = 0;
};
