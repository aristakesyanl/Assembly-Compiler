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

	AssemblyCompiler(std::string&);
	void binFileInit(); //create binary file and initialize its content with binCode
	
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


	//Operand Values
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
	};


	//Register Values
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
	};


	//Set of all Arithmetic Instructions
	std::set<std::string> ArithmeticOperand{
		"ADD",
		"SUB",
		"AND",
		"OR",
		"NOT",
		"XOR"
	};


	//Set of all Conditional Instructions
	std::set<std::string> ConditionalOperand{
		"JE",
		"JNE",
		"JL",
		"JLE",
		"JG",
		"JGE"
	};


	std::vector<uint8_t> binCode; //keep binary file after parsing
	std::map<std::string, int> Constant; //map for constant definitions
	std::map<std::string, int> Label; //map for labels
	unsigned int Count=0; //Counter for instructions;



	//Functions to parse Assembly file
	void parseAssemblyLine(std::string&); //parse assembly file
	void parseArithmeticInstruction(std::string&); //parse arithmetic instructions
	void parseConditionalInstruction(std::string&); //parse conditional instructions
	void parseLabel(std::string&); //parse labels
	void parseMovInstruction(std::string&); //parse MOV instructions


	//check the type of instruction
	bool isLabel(std::string&);
	bool isConstant(std::string&);
	bool isArithmetic(std::string&);
	bool isConditional(std::string&);
	bool isMov(std::string&);
	bool is_int(std::string&);


    //Functions that help parsing Instruction components
        std::string getFirstWord(std::string&); //get the first word of instruction
	void checkArgument1(Instruction&, std::string&); //parse the first argument
	void checkArgument2(Instruction&, std::string&); //parse the second argument
	void checkDestination(Instruction&, std::string&); //parse destination
	

    //Helper function
	void removeExtraWhitespaces(std::string&); //removes extra whitespaces
};

#endif
