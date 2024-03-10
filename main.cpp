// To Resolve
// Consider x  6

#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include "Riscv_Instructions.h"
#include "Instructions_Func.h"
#include "Auxiliary_Functions.h"

using namespace std;

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
    // program counter
    int program_counter = 0;
    // count the labels in the asm file
    while (getline(input_file, line))
    {
        program_counter++;
        // get first word of each line
        stringstream ss(line);
        string temp_word = "";
        ss >> temp_word;

        if(!line.empty() && (line.back() == ':')){
            // if label detected, insert in into branch_address map
            branch_address[temp_word] = program_counter;
        }
    }
    program_counter = 0;
    // If file is open
    if (output_file.is_open())
    {
        while (getline(input_file, line))
        {
            program_counter++;
            // Empty or comments
            if (line.empty() || line[0] == '#')
                continue; // Ignore Comments and empty lines

            // Removing leading and trailing spaces an instruction
            line = trim(line);
            // If derivatives
            if (!isDirective(line))
            {
                Instruction = InitializeInstruction(line, program_counter);
                Instruction.printInstruction();
            }
        }
    }
}
