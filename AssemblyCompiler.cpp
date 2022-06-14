#include<algorithm>
#include<regex>
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
                lines.push_back(line.substr(lineBegin,lineRange););
            }
        }
    }
    else{
        throw std::runtime_error("Failed To Open Input Files");
    }

    in.close();

    int count=(int)lines.size();
    for(int i=0; i<count; i++){
    	std::transform(lines[i].begin(),lines[i].end(),lines[i].begin(),::toupper);
    	parseAssemblyLine(lines[i]);
    }
}

void AssemblyCompiler::parseAssemblyLine(std::string&str){
	//check if the string contains a whitespace
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
		parseAssemblyLine(str.substr(len+1));
	}
	else if(isConstant(str)){

	}
	else if(isArithmetic(str)){
		parseArithmeticInstruction();
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
	int pos=0;
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
	Instruction instr;

	int pos1=0;
	while(str[pos1]!=whitespace){
		pos1++;
	}

	instr.Operand=ArithmeticOperand[str.substr(0,pos1)];


	//check Argument1
	int pos2=pos1+1;
	while(str[i]!=','){
		pos2++;
	}
	std::string arg1=str.substr(pos2,pos2-1-pos1);
	checkArgument1(instr,arg1);


    //check Argument2
	int pos3=pos2+1;
	while(str[i]!=','){
		pos3++;
	}
	std::string arg2=str.substr(pos3,pos3-1-pos2);
	checkArgument1(instr,arg2);

	//check Destination

	
}

void AssemblyCompiler::parseConditionalInstruction(std::string&str){
	
}


void AssemblyCompiler::parseLabel(std::string&str){
	int len=(int)str.size();
	std::string op=str.substr(0,len-1);
	Label[op]=Count;
}

void AssemblyCompiler::parseMovInstruction(std::string&str){
	
}

bool AssemblyCompiler::isLabel(std::string&str){
	int len=(int)str.size();
	if(str[len-1]==":"){
		return true;
	}
	else{
		return false;
	}
}

//Constant is Supported only for intgers
bool AssemblyCompiler::isConstant(std::string&str){

	int pos=0;
	while(str[pos]!=whitespace){
		pos++;
	}
	if(str.substr(pos+1,3)!="equ"){
		return false;
	}
	else{
		if(is_int(str.substr(pos+4))){
			int num=stoi(str.substr(pos+4));
			Constant[str.substr(0,pos)]=num;
		}
		else{
			throw::invalid_argument("Invalid Integer Type");
		}
	}
}

bool AssemblyCompiler::isArithmetic(std::string&str){
	int pos=0;
	while(str[i]!=whitespace){
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
	int pos=0;
	while(str[i]!=whitespace){
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
	int pos=0;
	while(str[i]!=whitespace){
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
	std::unique_copy(str.begin(),str.end(),back_insert_iterator<string>(output),
	            [](char a, char b){ return isspace(a) && isspace(b)});
	str=output;
}

//check if string is number with regex
//does not accept leading zeros
bool is_int(std::string&str){

    static constexpr auto max_digits=std::numeric_limits<int>::digits10 ;
    static const std::string ub=std::to_string(max_digits-1) ;
    static const std::regex int_re("^\\s*([+-]?[1-9]\\d{0,"+ub+"}|0)\\s*$");

    return std::regex_match(str,int_re) ;
}

void AssemblyCompiler::checkArgument1(Instruction& instr, const std::string arg1){
	if(arg1[0]=='R'){
		if((int)arg1.size()>3){
			throw std::invalid_argument("Invalid Register");
			if(arg1[1]<'0' || arg1[1]>'5'){
				throw std::invalid_argument("Invalid Register");
			}
			else if((int)arg1.size()==3 && arg1[2]!=whitespace){
				throw std::invalid_argument("Invalid Register");
			}
			else{
				instr.Argument1=arg1[1]-'0';
			}
		}
		else{
			throw insvalid_argument("Invalid Register");
		}
	}
	else{
		if(is_int(arg1) && stoi(arg1)<256 && stoi(arg1)>=0){
			instr.Argument1=stoi(arg1);
			instr.Operand|=0x80;
		}
		else{
			throw std::invalid_argumen("Invalid Immediate value");
		}

	}
}

void AssemblyCompiler::checkArgument2(Instruction& instr, const std::string arg2){
	if(arg2[0]=='R'){
		if((int)arg2.size()>3){
			throw std::invalid_argument("Invalid Register");
			if(arg2[1]<'0' || arg2[1]>'5'){
				throw std::invalid_argument("Invalid Register");
			}
			else if((int)arg2.size()==3 && arg2[2]!=whitespace){
				throw std::invalid_argument("Invalid Register");
			}
			else{
				instr.Argument1=arg2[1]-'0';
			}
		}
		else{
			throw insvalid_argument("Invalid Register");
		}
	}
	else{
		if(is_int(arg2) && stoi(arg2)<256 && stoi(arg2)>=0){
			instr.Argument2=stoi(arg2);
			instr.Operand|=0x40;
		}
		else{
			throw std::invalid_argumen("Invalid Immediate value");
		}

	}
}

