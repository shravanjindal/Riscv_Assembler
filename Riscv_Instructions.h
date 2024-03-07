# include<unordered_map>
# include<string.h>

using namespace std;

// Op Codes Map
unordered_map<string,string> opcode_map = {
    // R_Type
    {"add", "0110011"}, {"and", "0110011"}, {"or",  "0110011"}, {"sll", "0110011"},
    {"slt", "0110011"}, {"sra", "0110011"}, {"srl", "0110011"}, {"sub", "0110011"},
    {"xor", "0110011"}, {"slt", "0110011"}, {"mul", "0110011"}, {"div", "0110011"},

    // I_Type
    {"addi", "0010011"}, {"andi", "0010011"}, {"ori", "0010011"}, {"lb", "0000011"},
    {"ld", "0000011"}, {"lh", "0000011"}, {"lw", "0000011"}, {"jalr", "1100111"},

    // S_Type
    {"sb", "0100011"}, {"sw", "0100011"}, {"sd", "0100011"}, {"sh", "0100011"},

    //SB_Type
    {"beq", "1100011"}, {"bne", "1100011"}, {"bge", "1100011"}, {"blt", "1100011"},

    //U_Type
    {"auipc", "0010111"}, {"lui", "0110111"},

    //UJ_Type
    {"jal", "1101111"}
};

// Risc-V Derivatives
class Derivatives {
    int TEXT, DATA, BYTE, HALF,
        WORD, DWORD, ASCIZ;
};

// Instructions according to type
class R_Type {
    int ADD, AND, OR, SLL, SLT, SRA, SRL,
        SUB, XOR, MUL, DIV, REM;
};
class I_Type {
    int ADDI, ANDI, ORI, LB, LD, LH,
    LW, JALR;
};
class S_Type {
    int SB, SW, SD, SH;
};
class SB_Type {
    int BEQ, BNE, BGE, BLT;
};
class U_Type {
    int AUIPC, LUI;
};
class UJ_Type {
    int JAL;
};

// Inherting all instructions into a single class
class RISC_V_Instructions: public R_Type, public I_Type, public S_Type, public SB_Type,
                         public U_Type, public UJ_Type
{
    int rd = NULL, rs1 = NULL, rs2 = NULL, imm1 = NULL,
        Opcode = NULL, func3 = NULL, func7 = NULL, func6 = NULL,
        imm2 = NULL;
};
