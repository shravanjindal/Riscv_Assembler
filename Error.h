#ifndef ERROR_H // This needs to be unique in each header
#define ERROR_H

#include <iostream>
#include <string>
#include <sstream>
#include "Riscv_Instructions.h"

using namespace std;

// Current Instruction
RISC_V_Instructions Current_Instruction;

// Enum to have error code
enum ErrorType
{
    ERROR_NONE = 0x00,          // No Error
    ERROR_SYNTAX = 0x01,        // Syntax is wrong
    INVALID_INSTRUCTION = 0x02, // Instruction is not valid
};

// Defining Structure of Return Errors
class Error
{
private:
    // Getting Type of error
    ErrorType err;
    // String error message
    string msg;

public:
    // A Constructor
    Error(ErrorType error, string message)
    {
        err = error;
        msg = message;
    }
    // To Alter error
    void AlterError(ErrorType error, string message)
    {
        err = error;
        msg = message;
    }
};

// Function to check if it a valid instruction
const Error CheckInstruction(const string &inst)
{
    // For error output with a default of no error (Constructor)
    Error output_error = Error(ERROR_NONE, "Code executed Successfully!!!");
    // String stream to get word by word
    stringstream ss(inst);
    string temp_word = "";
    // Character by Character
    for (int i = 0; i <= inst.length(); i++)
    {
        // Getting first word
        ss >> temp_word;
        // If it is an instruction
        if (opcode_map[temp_word] != "")
            Current_Instruction.OpCode = opcode_map[temp_word];
        // Not an instruction
        else
        {
            // Altering error to invalid instruction
            output_error.AlterError(INVALID_INSTRUCTION, "Typed Instruction " + temp_word + " is invalid");
            // Exiting with error code
            exit(INVALID_INSTRUCTION);
        }
    }
}

#endif
