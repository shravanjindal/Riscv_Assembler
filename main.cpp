#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <string>
#include "Riscv_instructions.h"
#include "Error.h"

using namespace std;

// Function to remove leading and trailing spaces from a string
const string trim(const string &str)
{
    // Find the first non-space character
    auto start = find_if_not(str.begin(), str.end(), [](unsigned char c)
                             { return isspace(c); });

    // Find the last non-space character
    auto end = find_if_not(str.rbegin(), str.rend(), [](unsigned char c)
                           { return isspace(c); })
                   .base();

    // If start and end are the same, the string is either all spaces or empty
    if (start == end)
        return "";

    // Return the substring without leading and trailing spaces
    return string(start, end);
}

// Object of instruction being executed
RISC_V_Instructions Instruction;

int main()
{
    // Opening asm file
    ifstream input_file("main.asm");
    // Output machine code file
    ofstream output_file("main.mc", ios::binary);

    // Reading file line by line (instruction by instruction)
    string line;
    // If file is open
    if (output_file.is_open())
    {
        while (getline(input_file, line))
        {
            // Empty or comments
            if (line.empty() || line[0] == '#')
                continue; // Ignore Comments and empty lines

            // Removing leading and trailing spaces an instruction
            line = trim(line);

            // If derivatives
            if (!isDirective(line))
            {
                CheckInstruction(line);
            }
        }
    }
}
