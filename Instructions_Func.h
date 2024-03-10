#ifndef ERROR_H // This needs to be unique in each header
#define ERROR_H

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <math.h>
#include "Riscv_Instructions.h"
#include "Auxiliary_Functions.h"

using namespace std;

// Enum to have error code
enum ErrorType
{
    ERROR_NONE = 0x00,              // No Error
    ERROR_SYNTAX = 0x01,            // Syntax is wrong
    INVALID_INSTRUCTION = 0x02,     // Instruction is not valid
    INVALID_REGISTER = 0x03,        // Register is invalid
    INVALID_IMMEDIATE_VALUE = 0x04, // Register is invalid
    INVALID_LABEL = 0x05            // Branch Target is invalid
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
    void PrintError()
    {
        cout << "Code is exited with error code " << err << ". \"" << msg << "\"";
    }
};

extern map<string, int> branch_address;
const int Normal_XNum_Parameter(const string &given_parameter, Error *output_error)
{
    int reg_number;
    if (given_parameter[0] == 'x')
    {
        // Try catch if string it not fully integer
        try
        {
            if (isNumber(given_parameter.substr(1, given_parameter.length() - 1)))
            {
                // Getting register number
                reg_number = stoi(given_parameter.substr(1, given_parameter.length() - 1));
                // Throwing exception if register number is > 31
                if (reg_number > 31)
                    throw invalid_argument("");
                // Reading if it is valid
                return reg_number;
            }
            else
                throw invalid_argument("");
        }
        catch (exception e)
        {
            // Altering error to invalid register
            (*output_error).AlterError(INVALID_REGISTER, "Typed Register is invalid");
            (*output_error).PrintError();
            // Exiting with error code
            exit(INVALID_REGISTER);
        }
    }
    else
    {
        // Altering error to invalid register
        (*output_error).AlterError(INVALID_REGISTER, "Typed Register is invalid");
        (*output_error).PrintError();
        // Exiting with error code
        exit(INVALID_REGISTER);
    }
}

void Bracketed_Immediate_Parameter(RISC_V_Instructions *Current_Instruction, const string &given_parameter, Error *output_error)
{
    string temp_word;
    int reg_number;
    stringstream temp(given_parameter);
    getline(temp, temp_word, '(');
    // Removing trailing and leading spaces
    temp_word = trim(temp_word);
    // Getting register number
    reg_number = stoi(temp_word.substr(1, temp_word.length() - 1));
    // Throwing exception if register number is > 31
    if (reg_number > 31)
        throw invalid_argument("");
    // Reading if it is valid
    (*Current_Instruction).rs1 = reg_number;
    getline(temp, temp_word, '(');
    // Removing trailing and leading spaces
    temp_word = trim(temp_word.substr(0, temp_word.length() - 1));
    if (isNumber(temp_word))
    {
        reg_number = stoi(temp_word);
        (*Current_Instruction).imm = reg_number;
    }
    else
        throw invalid_argument("");
}

int Label_Offset_Parameter(const string &label, int program_counter, Error *output_error)
{
    // This function will return the number of instructions to skip
    if (branch_address.find(label) != branch_address.end())
    {
        return (branch_address[label] - program_counter);
    }
    else
    {
        // Altering error to invalid register
        (*output_error).AlterError(INVALID_LABEL, "Typed Branch Target is invalid");
        (*output_error).PrintError();
        // Exiting with error code
        exit(INVALID_LABEL);
        return -1;
    }
}

int Calculate_Immediate(const string &hex, Error *output_error)
{
    // This function will check if the Immediate is of less than 20 bits and convert it into decimal
    // Remove "0x" prefix if present
    string hexString = hex;
    if (hexString.substr(0, 2) == "0x")
        hexString = hexString.substr(2);
    int decimalValue = 0;
    if (hexString.length() > 5)
    {
        // Altering error to invalid register
        (*output_error).AlterError(INVALID_IMMEDIATE_VALUE, "Typed IMMEDIATE_VALUE is invalid");
        (*output_error).PrintError();
        // Exiting with error code
        exit(INVALID_IMMEDIATE_VALUE);
        return INT_MIN;
    }
    int power = hexString.length() - 1;
    for (char c : hexString)
    {
        int digit;
        if (c >= '0' && c <= '9')
        {
            digit = c - '0';
        }
        else if (c >= 'a' && c <= 'f')
        {
            digit = c - 'a' + 10;
        }
        else if (c >= 'A' && c <= 'F')
        {
            digit = c - 'A' + 10;
        }
        else
        {
            // Altering error to invalid register
            (*output_error).AlterError(INVALID_IMMEDIATE_VALUE, "Typed IMMEDIATE_VALUE is invalid");
            (*output_error).PrintError();
            // Exiting with error code
            exit(INVALID_IMMEDIATE_VALUE);
            return INT_MIN;
        }
        decimalValue += digit * pow(16, power);
        power--;
    }
    return decimalValue;
}
// Function to check if it a valid instruction
const RISC_V_Instructions InitializeInstruction(const string &inst, int program_counter)
{
    RISC_V_Instructions Current_Instruction;
    Current_Instruction.Instruction = inst;
    // For error output with a default of no error (Constructor)
    Error output_error = Error(ERROR_NONE, "Code executed Successfully!!!");
    // String stream to get word by word
    stringstream ss(inst);
    // Just some varibles for specific purpose
    string temp_word = "";
    int reg_number = -1, values = 0;
    // Character by Character
    // Getting first word
    ss >> temp_word;
    // This is for R-type instructions
    if (R_opcode_map.find(temp_word) != R_opcode_map.end())
    {
        // func3 and func7 Initialization
        Current_Instruction.func3 = func3_map[temp_word];
        Current_Instruction.func7 = func7_map[temp_word];
        // Reading Op Code for the instruction
        Current_Instruction.OpCode = R_opcode_map[temp_word];
        // Getting rest of the values from the instruction
        while (!ss.eof()) // Run file end of instruction
        {
            values++;
            // Spliting and reading word by delimiter as comma
            getline(ss, temp_word, ',');
            // Removing trailing and leading spaces
            temp_word = trim(temp_word);

            // Reading if it is valid
            if (values == 1)
                Current_Instruction.rd = Normal_XNum_Parameter(temp_word, &output_error);
            else if (values == 2)
                Current_Instruction.rs1 = Normal_XNum_Parameter(temp_word, &output_error);
            else if (values == 3)
                Current_Instruction.rs2 = Normal_XNum_Parameter(temp_word, &output_error);
        }
    }
    // I Type
    else if (I_opcode_map.find(temp_word) != I_opcode_map.end())
    {
        // Initializing func3
        Current_Instruction.func3 = func3_map[temp_word];
        // Reading Op Code for the instruction
        Current_Instruction.OpCode = I_opcode_map[temp_word];
        // Getting rest of the values from the instruction
        while (!ss.eof()) // Run file end of instruction
        {
            values++;
            // Spliting and reading word by delimiter as comma
            getline(ss, temp_word, ',');
            // Removing trailing and leading spaces
            temp_word = trim(temp_word);
            // Reading register value
            // If lw, ld, lh, lb
            if (Current_Instruction.OpCode == "0000011" && values == 2 && temp_word[0] == 'x')
                Bracketed_Immediate_Parameter(&Current_Instruction, temp_word, &output_error);
            // addi, andi, ori, jalr
            else if (temp_word[0] == 'x' && values != 3)
            {
                // Reading if it is valid
                if (values == 1)
                    Current_Instruction.rd = Normal_XNum_Parameter(temp_word, &output_error);
                else if (values == 2)
                    Current_Instruction.rs1 = Normal_XNum_Parameter(temp_word, &output_error);
            }
            else if (values == 3)
            {
                // Try catch if string it not fully integer
                try
                {
                    // Getting Immediate value
                    reg_number = stoi(temp_word);
                    // Storing Immediate value
                    Current_Instruction.imm = reg_number;
                }
                catch (exception e)
                {
                    // Altering error to invalid register
                    output_error.AlterError(INVALID_IMMEDIATE_VALUE, "Typed Immediate value is invalid");
                    output_error.PrintError();
                    // Exiting with error code
                    exit(INVALID_IMMEDIATE_VALUE);
                }
            }
            else
            {
                // Altering error to invalid register
                output_error.AlterError(INVALID_REGISTER, "Typed Register is invalid");
                output_error.PrintError();
                // Exiting with error code
                exit(INVALID_REGISTER);
            }
        }
    }
    // S Type
    else if (S_opcode_map.find(temp_word) != S_opcode_map.end())
    {
        // Initializing func3
        Current_Instruction.func3 = func3_map[temp_word];
        // Reading Op Code for the instruction
        Current_Instruction.OpCode = S_opcode_map[temp_word];
        // Getting rest of the values from the instruction
        while (!ss.eof()) // Run file end of instruction
        {
            values++;
            // Spliting and reading word by delimiter as comma
            getline(ss, temp_word, ',');
            // Removing trailing and leading spaces
            temp_word = trim(temp_word);
            // Reading register value
            if (temp_word[0] == 'x' && values != 2)
                // Reading if it is valid
                if (values == 1)
                    Current_Instruction.rd = Normal_XNum_Parameter(temp_word, &output_error);
                else if (values == 2)
                    Bracketed_Immediate_Parameter(&Current_Instruction, temp_word, &output_error);
                else
                {
                    // Altering error to invalid register
                    output_error.AlterError(INVALID_REGISTER, "Typed Register is invalid");
                    output_error.PrintError();
                    // Exiting with error code
                    exit(INVALID_REGISTER);
                }
        }
    }
    // SB Type
    else if (SB_opcode_map.find(temp_word) != SB_opcode_map.end())
    {
        // Initializing func3
        Current_Instruction.func3 = func3_map[temp_word];
        // Reading Op Code for the instruction
        Current_Instruction.OpCode = S_opcode_map[temp_word];
        // Getting rest of the values from the instruction
        while (!ss.eof()) // Run file end of instruction
        {
            values++;
            // Spliting and reading word by delimiter as comma
            getline(ss, temp_word, ',');
            // Removing trailing and leading spaces
            temp_word = trim(temp_word);

            // Reading if it is valid
            if (values == 1)
                Current_Instruction.rs1 = Normal_XNum_Parameter(temp_word, &output_error);
            else if (values == 2)
                Current_Instruction.rs2 = Normal_XNum_Parameter(temp_word, &output_error);
            else if (values == 3)
                Current_Instruction.imm = Label_Offset_Parameter(temp_word, program_counter, &output_error);
            else
            {
                // Altering error to invalid register
                output_error.AlterError(ERROR_SYNTAX, "Typed Syntax is invalid");
                output_error.PrintError();
                // Exiting with error code
                exit(ERROR_SYNTAX);
            }
        }
    }
    // U Type
    else if (U_opcode_map.find(temp_word) != U_opcode_map.end())
    {
        // Reading Op Code for the instruction
        Current_Instruction.OpCode = U_opcode_map[temp_word];

        while (!ss.eof()) // Run file end of instruction
        {
            values++;
            // Spliting and reading word by delimiter as comma
            getline(ss, temp_word, ',');
            // Removing trailing and leading spaces
            temp_word = trim(temp_word);

            // Reading if it is valid
            if (values == 1)
                Current_Instruction.rd = Normal_XNum_Parameter(temp_word, &output_error);
            else if (values == 2)
            {
                if (temp_word.substr(0, 2) == "0x")
                    Current_Instruction.imm = Calculate_Immediate(temp_word, &output_error);
                else if (stoi(temp_word) <= 1048576)
                    Current_Instruction.imm = stoi(temp_word);
                else
                {
                    // Altering error to invalid register
                    output_error.AlterError(INVALID_IMMEDIATE_VALUE, "Typed Immediate value is invalid");
                    output_error.PrintError();
                    // Exiting with error code
                    exit(INVALID_IMMEDIATE_VALUE);
                }
            }

            else
            {
                // Altering error to invalid register
                output_error.AlterError(ERROR_SYNTAX, "Typed Syntax is invalid");
                output_error.PrintError();
                // Exiting with error code
                exit(ERROR_SYNTAX);
            }
        }
    }
    else if (U_opcode_map.find(temp_word) != U_opcode_map.end())
    {
        // Reading Op Code for the instruction
        Current_Instruction.OpCode = U_opcode_map[temp_word];
        // rd for the instruction
        Current_Instruction.rd = program_counter + 4; // next instruction will be executed after return
        while (!ss.eof())                             // Run file end of instruction
        {
            values++;
            // Spliting and reading word by delimiter as comma
            getline(ss, temp_word, ',');
            // Removing trailing and leading spaces
            temp_word = trim(temp_word);

            // Reading if it is valid
            if (values == 1)
                Current_Instruction.imm = Label_Offset_Parameter(temp_word, program_counter, &output_error);
            else
            {
                // Altering error to invalid register
                output_error.AlterError(ERROR_SYNTAX, "Typed Syntax is invalid");
                output_error.PrintError();
                // Exiting with error code
                exit(ERROR_SYNTAX);
            }
        }
    }
    // Not an instruction
    else
    {
        // Altering error to invalid instruction
        output_error.AlterError(INVALID_INSTRUCTION, "Typed Instruction " + temp_word + " is invalid");
        output_error.PrintError();
        // Exiting with error code
        exit(INVALID_INSTRUCTION);
    }
    return Current_Instruction;
}

#endif
