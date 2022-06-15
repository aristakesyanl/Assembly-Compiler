#include<algorithm>
#include<regex>
#include<cctype>
#include<utility>
#include"AssemblyCompiler.h"

AssemblyCompiler::AssemblyCompiler(std::string& fileName){
	std::ifstream in(fileName);
    std::string line;
    std::vector<std::string> lines;

    if (in.is_open()){
        while (getline(in, line)){
            if (!(line.empty() || line.find_first_not_of(' ')==std::string::npos)){
                const auto lineBegin=line.find_first_not_of(' ');
                const auto lineEnd=line.find_last_not_of(' ');
    			const auto lineRange=lineEnd-lineBegin+1;
                lines.push_back(line.substr(lineBegin,lineRange));
            }
        }
    }
    else{
        throw std::runtime_error("Failed To Open Input File");
    }

    in.close();

    for(auto i=0; i<lines.size(); i++){
    	std::transform(lines[i].begin(),lines[i].end(),lines[i].begin(),::toupper); //assembly is case-insensitive
    	parseAssemblyLine(lines[i]);
    }
}

void AssemblyCompiler::parseAssemblyLine(std::string& str){
	//check if string contains a whitespace
	//If it doesn't it should be a label or invalid instruction
	removeExtraWhitespaces(str);
	if(str.find(' ')!=std::string::npos){
		if(!isLabel(str)){
			throw std::invalid_argument("Invalid Label Format");
		}
		else{
			parseLabel(str);
		}
	}
	std::string op=getFirstWord(str);
	if(isLabel(op)){
		parseLabel(op);
		decltype(op.size()) len=op.size();
		std::string newstr=str.substr(len+1);
		parseAssemblyLine(newstr);
	}
	else if(isConstant(str)){

	}
	else if(isArithmetic(str)){
		parseArithmeticInstruction(str);
		Count++;
	}
	else if(isConditional(str)){
		parseConditionalInstruction(str);
		Count++;
	}
	else if(isMov(str)){
		parseMovInstruction(str);
		Count++;
	}
	else{
		throw std::invalid_argument("Invalid Instruction");
	}

}

std::string AssemblyCompiler::getFirstWord(std::string&str){
	std::string op;
	decltype(op.size()) pos=0;
	while(str[pos]!=' '){
		pos++;
	}
	op=str.substr(0,pos);
	if(Operands.find(op)==Operands.end()){
		throw std::invalid_argument("Invalid Instruction");
	}
	else{
		return op;
	}
}

void AssemblyCompiler::parseArithmeticInstruction(std::string&str){

	//For every line get 4 Components of instruction
	//Add them to bincode
	Instruction instr;
    

    //check instruction operand
	decltype(str.size()) pos1=0;
	while(!isspace(str[pos1])){
		pos1++;
	}
	instr.Operand=Operands[str.substr(0,pos1)];


	//check Argument1
	decltype(str.size()) pos2=pos1+1;
	while(!isspace(str[pos2])){
		pos2++;
	}
	std::string arg1=str.substr(pos2,pos2-1-pos1);
	if(arg1.empty()){
		throw std::invalid_argument("Invalid Instruction Format: Argument1 is missing");
	}
	checkArgument1(instr,arg1);


    //check Argument2
	decltype(str.size()) pos3=pos2+1;
	while(!isspace(str[pos3])){
		pos3++;
	}
	if(arg2.empty()){
		throw std::invalid_argument("Invalid Instruction Format: Argument2 is missing");
	}
	std::string arg2=str.substr(pos3,pos3-1-pos2);
	checkArgument2(instr,arg2);


	//check Destination
	decltype(str.size()) pos4=pos3+1;
	while(!isspace(str[pos4])){
		pos4++;
	}
	std::string dest=str.substr(pos4,pos4-1-pos3);
	checkDestination(instr,dest);

}

void AssemblyCompiler::parseConditionalInstruction(std::string&str){
	
}


void AssemblyCompiler::parseLabel(std::string&str){
	decltype(str.size()) len=str.size();
	std::string op=str.substr(0,len-1);
	Label[op]=Count;
}

void AssemblyCompiler::parseMovInstruction(std::string&str){
	
}

bool AssemblyCompiler::isLabel(std::string&str){
	decltype(str.size()) len=str.size();
	if(str[len-1]==':'){
		return true;
	}
	else{
		return false;
	}
}

//Constant is Supported only for intgers
bool AssemblyCompiler::isConstant(std::string&str){

	decltype(str.size()) pos=0;
	while(!isspace(str[pos])){
		pos++;
	}
	if(str.substr(pos+1,3)!="equ"){
		return false;
	}
	else{
		std::string num=str.substr(pos+4);
		if(isInt(num)){
			int num=stoi(str.substr(pos+4));
			Constant[str.substr(0,pos)]=num;
		}
		else{
			throw std::invalid_argument("Invalid Integer Type");
		}
	}
	return false;
}

bool AssemblyCompiler::isArithmetic(std::string&str){
	decltype(str.size()) pos=0;
	while(!isspace(str[pos])){
		pos++;
	}

	if(ArithmeticOperand.find(str.substr(0,pos))!=ArithmeticOperand.end()){
		return true;
	}
	else{
		return false;
	}
	
}

bool AssemblyCompiler::isConditional(std::string&str){
	decltype(str.size()) pos=0;
	while(!isspace(str[pos])){
		pos++;
	}

	if(ConditionalOperand.find(str.substr(0,pos))!=ConditionalOperand.end()){
		return true;
	}

	else{
		return false;
	}
	
}

bool AssemblyCompiler::isMov(std::string&str){
	decltype(str.size()) pos=0;
	while(!isspace(str[pos])){
		pos++;
	}

	if(str.substr(0,pos)!="MOV"){
		return false;
	}
	else{
		return true;
	}
	
}

void AssemblyCompiler::removeExtraWhitespaces(std::string&str){
	std::string output;
	std::unique_copy(str.begin(),str.end(),std::back_insert_iterator<std::string>(output),
	            [](char a, char b){ return isspace(a) && isspace(b);});
	str=output;
}

//check if string is number with regex
//does not accept leading zeros
bool isInt(std::string&str){

    constexpr auto max_digits=std::numeric_limits<int>::digits10 ;
    const std::string ub=std::to_string(max_digits-1) ;
    const std::regex int_re("^\\s*([+-]?[1-9]\\d{0,"+ub+"}|0)\\s*$");

    return std::regex_match(str,int_re) ;
}

void AssemblyCompiler::checkArgument1(Instruction& instr, std::string& arg1){
	if(arg1[0]=='R'){
		if((int)arg1.size()>3){
			throw std::invalid_argument("Invalid Register");
			if(arg1[1]<'0' || arg1[1]>'5'){
				throw std::invalid_argument("Invalid Register");
			}
			else if((int)arg1.size()==3 && arg1[2]!=' '){
				throw std::invalid_argument("Invalid Register");
			}
			else{
				instr.Argument1=arg1[1]-'0';
			}
		}
		else{
			throw std::invalid_argument("Invalid Register");
		}
	}
	else{
		if(isInt(arg1) && stoi(arg1)<256 && stoi(arg1)>=0){
			instr.Argument1=stoi(arg1);
			instr.Operand|=0x80;
		}
		else{
			throw std::invalid_argument("Invalid Immediate value");
		}

	}
}

void AssemblyCompiler::checkArgument2(Instruction& instr, std::string& arg2){
	if(arg2[0]=='R'){
		if((int)arg2.size()>3){
			throw std::invalid_argument("Invalid Register");
			if(arg2[1]<'0' || arg2[1]>'5'){
				throw std::invalid_argument("Invalid Register");
			}
			else if((int)arg2.size()==3 && !isspace(arg2[2])){
				throw std::invalid_argument("Invalid Register");
			}
			else{
				instr.Argument1=arg2[1]-'0';
			}
		}
		else{
			throw std::invalid_argument("Invalid Register");
		}
	}
	else{
		if(isInt(arg2) && stoi(arg2)<256 && stoi(arg2)>=0){
			instr.Argument2=stoi(arg2);
			instr.Operand|=0x40;
		}
		else{
			throw std::invalid_argument("Invalid Immediate value");
		}
	}
}

std::pair<int,int> AssemblyCompiler::checkDestination(Instruction& instr, std::string& dest){
	
}
