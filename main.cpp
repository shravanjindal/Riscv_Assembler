#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include "Riscv_instructions.h"

using namespace std;

// Object of instruction being executed
RISC_V_Instructions Instruction;

int main()
{
    // Opening asm file
    ifstream input_file("main.asm");
    // Output machine code file
    ofstream output_file("main.mc", ios::binary);

    //Reading file line by line (instruction by instruction)
    string line;
    while(getline(input_file, line))
    {
        // Empty or comments
        if(line.empty() || line[0] == '#')
            continue;                                       //Ignore Comments and empty lines
        
        
    }
}
