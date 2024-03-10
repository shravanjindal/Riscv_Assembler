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
    ofstream output_file("main.mc", ios::out);

    // Reading file line by line (instruction by instruction)
    string line;
    bool isText = true, isData = true;
    if (input_file.is_open())
    {
        while (getline(input_file, line))
        {
            // Empty or comments
            if (line.empty() || line[0] == '#')
                continue; // Ignore Comments and empty lines

            // Removing leading and trailing spaces an instruction
            line = trim(line);
            // If derivatives
            if (line == ".data")
            {
                isText = false;
                isData = true;
                continue;
            }
            else if(line == ".text")
            {
                isText = true;
                isData = false;
                continue;
            }

            // To get entire block
            if(isData == true)
                dataDirectiveInst.push_back(line);
            else if(isText == true)
            {
                /* Setting Program Counter for labels */
                try
                {
                    // Getting colon position if label
                    size_t colonPos = line.find(':');
                    if(colonPos != string::npos)
                    {
                        line = trim(line.substr(0, line.length()-1));
                        labels[line] = pc;
                    }
                    else
                        textDirectiveInst.push_back(line);
                }
                catch(exception ex)
                {

                }
                pc++;
            }
        }
    }
}
