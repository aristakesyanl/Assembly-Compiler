#ifndef ASSEMBLYCOMPILER_H
#define ASSEMBLYCOMPILER_H


#include<iostream>
#include<map>
#include<fstream>
#include<string>
#include<cstring>
#include<vector>

class AssemblyCompiler{
public:
	AssemblyCompiler(std::string);
	//create binary file and initialize its content with binCode
	void binFileInit();
	
private:
	//Opcodes for operations
	static const int opADD=0;
	static const int opSUB=1;
	static const int opAND=2;
	static const int opOR=3;
	static const int opNOT=4;
	static const int opXOR=5;
	static const int opJE=32;
	static const int opJNE=33;
	static const int opJL=34;
	static const int opJLE=35;
	static const int opJG=36;
	static const int opJGE=37;

	//Registers
	static const int R0=0;
	static const int R1=1;
	static const int R2=2;
	static const int R3=3;
	static const int R4=4;
	static const int R5=5;
	static const int Counter=6;
	static const int In=7;
	static const int Out=7;

    //keep binary file after parsing
	std::vector<uint8_t> binCode;
	std::set<std::string> Operand{
		"ADD",
		"SUB",
		"AND",
		"OR",
		"NOT",
		"XOR",
		"JE",
		"JNE",
		"JL",
		"JLE",
		"JG",
		"JGE",
		"MOV"
	}

	//map for constant definitions
	std::map<std::string, int> Constant;
	std::map<std::string, int> Label;

    //Counter for instructions;
	int Count=0;

	//parse assembly file
	void parseAssemblyLine(std::&string);
	//parse for specific types of lines
	void parseArithmeticInstruction(std::&string);
	void parseConditionalInstruction(std::&string);
	void parseConstant(std::&string);
	void parseLabel(std::&string);
	void parseMovInstruction(std::&string);


	std::string readOperand(std::&string)

	//check the type of instruction
	bool isLabel(std::&string);
	bool isConstant(std::&string);
	bool isArithmetic(std::&string);
	bool isConditional(std::string);
	bool isMov(std::&string)


	std::map<std::string, int> Register{
		{"R0",      R0},
		{"R1",      R1},
		{"R2",      R2},
		{"R3",      R3},
		{"R4",      R4},
		{"R5",      R5},
		{"Counter", Counter},
		{"In",      In},
		{"Out",     Out}
	}

	std::map<std::string, int> Operands{
		{"ADD",  opADD},
		{"SUB",  opSUB},
		{"AND",  opAND},
		{"OR",   opOR},
		{"NOT",  opNOT},
		{"XOR",  opXOR},
		{"JE",   opJE},
		{"JNE",  opJNE},
		{"JL",   opJL}, 
		{"JLE",  opJLE},
		{"JG",   opJG}, 
		{"JGE",  opJGE}
	}
}

#endif
