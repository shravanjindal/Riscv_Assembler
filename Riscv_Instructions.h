#ifndef RISCV_INSTRUCTIONS_H // This needs to be unique in each header
#define RISCV_INSTRUCTIONS_H

#include <unordered_map>
#include <string.h>

using namespace std;

// Op Codes Map
unordered_map<string, string> opcode_map = {
    // R_Type
    {"add", "0110011"},
    {"and", "0110011"},
    {"or", "0110011"},
    {"sll", "0110011"},
    {"slt", "0110011"},
    {"sra", "0110011"},
    {"srl", "0110011"},
    {"sub", "0110011"},
    {"xor", "0110011"},
    {"slt", "0110011"},
    {"mul", "0110011"},
    {"div", "0110011"},

    // I_Type
    {"addi", "0010011"},
    {"andi", "0010011"},
    {"ori", "0010011"},
    {"lb", "0000011"},
    {"ld", "0000011"},
    {"lh", "0000011"},
    {"lw", "0000011"},
    {"jalr", "1100111"},

    // S_Type
    {"sb", "0100011"},
    {"sw", "0100011"},
    {"sd", "0100011"},
    {"sh", "0100011"},

    // SB_Type
    {"beq", "1100011"},
    {"bne", "1100011"},
    {"bge", "1100011"},
    {"blt", "1100011"},

    // U_Type
    {"auipc", "0010111"},
    {"lui", "0110111"},

    // UJ_Type
    {"jal", "1101111"}};

// Risc-V Directives
class Directives
{
    int TEXT, DATA, BYTE, HALF,
        WORD, DWORD, ASCIZ;
};

// Inherting all instructions into a single class
class RISC_V_Instructions
{
    // To Make elements available for public
public:
    int rd, rs1, rs2, imm1, func3, func7, func6, imm2;
    string OpCode;
};

// Function Definations
const bool isDirective(const string &line)
{
    return line[0] == '.';
}

#endif
