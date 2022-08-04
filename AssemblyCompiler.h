#ifndef ASSEMBLYCOMPILER_H
#define ASSEMBLYCOMPILER_H


#include<iostream>
#include<map>
#include<fstream>
#include<string>
#include<cstring>
#include<vector>
#include<set>

struct Instruction{
	uint8_t Operand;
	uint8_t Argument1;
	uint8_t Argument2;
	uint8_t Result;
};

class AssemblyCompiler{
public:
	//RAM size is 2MB
	static const int RAM=65536;

	//Opcodes for operations
	const uint8_t opADD=0;
	const uint8_t opSUB=1;
	const uint8_t opAND=2;
	const uint8_t opOR=3;
	const uint8_t opNOT=4;
	const uint8_t opXOR=5;
	const uint8_t opJE=32;
	const uint8_t opJNE=33;
	const uint8_t opJL=34;
	const uint8_t opJLE=35;
	const uint8_t opJG=36;
	const uint8_t opJGE=37;


	//Registers
	const uint8_t R0=0;
	const uint8_t R1=1;
	const uint8_t R2=2;
	const uint8_t R3=3;
	const uint8_t R4=4;
	const uint8_t R5=5;
	const uint8_t Counter=6;
	const uint8_t In=7;
	const uint8_t Out=7;

	AssemblyCompiler(std::string&);
	int binFileInit();

private:
	//Operand Values
	std::map<std::string, uint8_t> Operands={
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
	};
	//Register Values
	std::map<std::string, uint8_t> Register={
		{"R0",      R0},
		{"R1",      R1},
		{"R2",      R2},
		{"R3",      R3},
		{"R4",      R4},
		{"R5",      R5},
		{"Counter", Counter},
		{"In",      In},
		{"Out",     Out}
	};


	//Set of all Arithmetic Instructions
	std::set<std::string> ArithmeticOperand={
		"ADD",
		"SUB",
		"AND",
		"OR",
		"NOT",
		"XOR"
	};

	//Set of all Conditional Instructions
	std::set<std::string> ConditionalOperand={
		"JE",
		"JNE",
		"JL",
		"JLE",
		"JG",
		"JGE"
	};

	std::vector<uint8_t> bincode; //keep binary file after parsing
	std::map<std::string, int> Constant; //map for constant definitions
	std::map<std::string, int> Label; //map for labels
	int Count=0; //Points on the next instruction to be executed

	//Functions to parse Assembly file
	void parseAssemblyLine(std::string&);
	void parseLabel(std::string&); //parse labels
	void parseArithmeticInstruction(std::string&); //parse arithmetic instructions
	void parseMovInstruction(std::string&); //parse MOV instructions
	void parseConditionalInstruction(std::string&); //parse conditional instructions


	//check the type of instruction
	bool isLabel(std::string&);
	bool isConstant(std::string&);
	bool isArithmetic(std::string&);
	bool isMov(std::string&);
	bool isConditional(std::string&);

	//Functions that help parsing Instruction components
    std::string getFirstWord(std::string&); //get the first word of instruction
    void checkArgument1(Instruction&, std::string&); //parse the first argument
	void checkArgument2(Instruction&, std::string&); //parse the second argument
	void checkDestination(Instruction&, std::string&); //parse destination
	void addBinaryCode(Instruction&);


	void removeExtraWhitespaces(std::string&); //removes extra whitespaces
	bool isInt(std::string&);
};

#endif
