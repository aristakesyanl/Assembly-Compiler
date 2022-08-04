#include<algorithm>
#include<regex>
#include<cctype>
#include<utility>
#include<iostream>
#include<fstream>
#include<cstring>
#include"compiler.h"
using namespace std;

AssemblyCompiler::AssemblyCompiler(std::string& fileName){
    std::ifstream in(fileName);
    std::string line;
    std::vector<std::string> lines;

    if (in.is_open()){
        while (getline(in, line)){
            if (!(line.empty() || line.find_first_not_of(' ')==std::string::npos)){
                size_t lineBegin=line.find_first_not_of(' ');
                size_t lineEnd=line.find_last_not_of(' ');
                size_t lineRange=lineEnd-lineBegin+1;
                std::string str=line.substr(lineBegin,lineRange);
                removeExtraWhitespaces(str);
                if(isLabel(str)){
                    parseLabel(str);
                }
                else if(isConstant(str)){

                }
                else{
                    //check if the first word is label
                    std::string s=getFirstWord(str);
                    if(isLabel(s)){
                        lines.push_back(str.substr(s.size()+1));
                        parseLabel(s);
                        Count++;
                    }
                    else{
                        lines.push_back(str);
                        Count++;
                    }
                }
            }
        }
    }
    else{
        throw std::runtime_error("Failed To Open Input File");
    }

    in.close();


    for(size_t i=0; i<lines.size();i++){
        parseAssemblyLine(lines[i]);
    }

}


void AssemblyCompiler::parseAssemblyLine(std::string& str){
    //check if string contains a whitespace
    //If it doesn't it should be a label or invalid instruction
   
    /*Check first the label declarations
    There are two possible case of label declarations
    1.Line entirely consists of label declaration
    2.Line consists of label declaration and following instruction
    For the first case check that the line has format "LABEL:"
    For the second case line has a format "LABEL: INSTRUCTION"
    Parse label first, then parse Instruction.
    Note that the new line can be only Instruction, not a label or constant declaration
    */

    //If the declaration doesn't contain whitespaces it should be a label
    if(isArithmetic(str)){
        parseArithmeticInstruction(str);
    }
    else if(isMov(str)){
        parseMovInstruction(str);
    }
    else if(isConditional(str)){
       parseConditionalInstruction(str);
    }
    else{
        throw std::invalid_argument("Wrong Instruction");
    }
}

void AssemblyCompiler::parseLabel(std::string&str){
    size_t len=str.size();
    std::string op=str.substr(0,len-1);
    Label[op]=Count;
}


bool AssemblyCompiler::isLabel(std::string&str){
    //Label definations end with ':'
    size_t len=str.size();
    if(str[len-1]==':' && str.find(' ')==std::string::npos){
        return true;
    }
    return false;
}

//Constant is Supported only for intgers
bool AssemblyCompiler::isConstant(std::string&str){

    size_t pos=0;
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
            return true;
        }
        else{
            throw std::invalid_argument("Invalid Integer Type");
        }
    }
    return false;
}

bool AssemblyCompiler::isArithmetic(std::string&str){
    std::string op=getFirstWord(str);
    std::transform(op.begin(),op.end(),op.begin(),::toupper); //assembly is case-insensitive
    if(ArithmeticOperand.find(op)!=ArithmeticOperand.end()){
        return true;
    }
    else{
        return false;
    }
}

bool AssemblyCompiler::isMov(std::string&str){
    std::string newstr=str.substr(0,3);
    std::transform(newstr.begin(),newstr.end(),newstr.begin(),::toupper); //assembly is case-insensitive

    if(newstr.substr(0,3)!="MOV"){
        return false;
    }
    else{
        return true;
    }
}


bool AssemblyCompiler::isConditional(std::string&str){
    std::string op=getFirstWord(str);
    std::transform(op.begin(),op.end(),op.begin(),::toupper); //assembly is case-insensitive

    if(ConditionalOperand.find(op)!=ConditionalOperand.end()){
        return true;
    }
    else{
        return false;
    }
}


void AssemblyCompiler::parseArithmeticInstruction(std::string&str){

    //For every line get 4 Components of instruction
    //Add them to bincode
    Instruction instr;
    

    //check instruction operand
    size_t pos1=0;
    while(!isspace(str[pos1])){
        pos1++;
    }
    std::string op=str.substr(0,pos1);
    std::transform(op.begin(), op.end(),op.begin(), ::toupper);
    instr.Operand=Operands[op];


    //check Argument1
    size_t pos2=pos1+1;
    int len1=0;
    while(str[pos2+len1]!=','){
        len1++;
    }
    if(str[pos2+len1-1]==' ') len1--;
    std::string arg1=str.substr(pos2,len1);
    if(arg1.empty()){
        throw std::invalid_argument("Invalid Instruction Format: Argument1 is missing");
    }
    checkArgument1(instr,arg1);
   

    //check Argument2
    size_t pos3=pos2+len1;
    while(str[pos3]==',' ||str[pos3]==' '){
        pos3++;
    }
    int len2=0;
    while(str[pos3+len2]!=','){
        len2++;
    }
    if(str[pos3+len2]==' ') len2--;
    std::string arg2=str.substr(pos3,len2);
    if(arg2.empty()){
        throw std::invalid_argument("Invalid Instruction Format: Argument2 is missing");
    }
    checkArgument2(instr,arg2);
    

    //check Destination
    size_t pos4=pos3+len2;
    while(str[pos4]==',' ||str[pos4]==' '){
        pos4++;
    }
    
    std::string dest=str.substr(pos4);
    if(dest.empty()){
        throw std::invalid_argument("Invalid Instruction Format: Destination is missing");
    }
    checkDestination(instr,dest);

    //add binary code to bincode vector
    addBinaryCode(instr);
    
}


//Machine language does not explicitly contain MOV instructions.
//To translate MOV instructions compiler replaces them with XOR
//instructions.
//mov R0, R1 -> xor R1, 0, R0
//mov R0, 5  -> xor 5, 0, R0
//Note that Argument1 should be a register
//In MOV instructions first argument matches the destination

void AssemblyCompiler::parseMovInstruction(std::string&str){
    Instruction instr;
    size_t pos1=0;
    while(str[pos1]!=' '){
        pos1++;
    }
    instr.Operand=Operands["XOR"];
    //second argument is immediate
    instr.Operand|=0x40;

    //check destination
    size_t pos2=pos1+1;
    int len1=0;
    while(str[pos2+len1]!=','){
        len1++;
    }
    if(str[pos2+len1-1]==' ') len1--;
    std::string dest=str.substr(pos2,len1);
    if(dest.empty()){
        throw std::invalid_argument("Invalid Instruction Format: Argument1 is missing");
    }
    checkDestination(instr,dest);
 
    //check Argument
    size_t pos3=pos2+len1;
    while(str[pos3]==',' ||str[pos3]==' '){
        pos3++;
    }
    int len2=0;
    while(str[pos3+len2]!=','){
        len2++;
    }
    if(str[pos3+len2]==' ') len2--;
    std::string arg=str.substr(pos3,len2);
    if(arg.empty()){
        throw std::invalid_argument("Invalid Instruction Format: Argument2 is missing");
    }
    checkArgument1(instr,arg);

    //Second argument in XOR will be 0
    instr.Argument2=0x00;

    //add binary code to bincode vector
    addBinaryCode(instr);

}


void AssemblyCompiler::parseConditionalInstruction(std::string&str){

    Instruction instr;

    //check instruction operand
    size_t pos1=0;
    while(str[pos1]!=' '){
        pos1++;
    }
    std::string op=str.substr(0,pos1);
    std::transform(op.begin(), op.end(),op.begin(), ::toupper);
    instr.Operand=Operands[op];
   
    //check Argument1
    size_t pos2=pos1+1;
    int len1=0;
    while(str[pos2+len1]!=','){
        len1++;
    }
    if(str[pos2+len1-1]==' ') len1--;
    std::string arg1=str.substr(pos2,len1);
    if(arg1.empty()){
        throw std::invalid_argument("Invalid Instruction Format: Argument1 is missing");
    }
    checkArgument1(instr,arg1);

    //check Argument2
    size_t pos3=pos2+len1;
    while(str[pos3]==',' ||str[pos3]==' '){
        pos3++;
    }
    int len2=0;
    while(str[pos3+len2]!=','){
        len2++;
    }
    if(str[pos3+len2]==' ') len2--;
    std::string arg2=str.substr(pos3,len2);
    if(arg2.empty()){
        throw std::invalid_argument("Invalid Instruction Format: Argument2 is missing");
    }
    checkArgument2(instr,arg2);


    //check Jumpaddress
    size_t pos4=pos3+len2;
    while(str[pos4]==',' ||str[pos4]==' '){
        pos4++;
    }
    
    std::string dest=str.substr(pos4);
    if(dest.empty()){
        throw std::invalid_argument("Invalid Instruction Format: Destination is missing");
    }
    else{
        if(isInt(dest)){
            int jumpaddress=stoi(dest);
            if(jumpaddress>255){
                throw std::invalid_argument("Invalid Jumpaddress: Out of range");
            }
            else{
                instr.Result=jumpaddress;
            }
        }
        else{
            if(Label.count(dest)){
                int jumpaddress=Label[dest];
                if(jumpaddress>255){
                    throw std::invalid_argument("Invalid Jumpaddress: Out of range");
                }
                else{
                    instr.Result=jumpaddress;
                }
            }
            else{
                throw std::invalid_argument("Invalid Label address");
            } 
        }
    }

    addBinaryCode(instr);
}




//Get the first word in the line
std::string AssemblyCompiler::getFirstWord(std::string&str){
    std::string op;
    size_t pos=0;
    while(!isspace(str[pos])){
        pos++;
    }
    op=str.substr(0,pos);
    return op;
}

//First Argument can be register, immediate value or constant
void AssemblyCompiler::checkArgument1(Instruction& instr, std::string& arg1){
    if(arg1[0]=='R' || arg1[0]=='r'){
        std::transform(arg1.begin(),arg1.end(),arg1.begin(),::toupper); //assembly is case-insensitive
        if((int)arg1.size()<4){
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
            if(Constant.count(arg1)){
                if(Constant[arg1]>=256){
                    throw std::invalid_argument("Constant is too big");
                }
                instr.Argument1=Constant[arg1];
                instr.Operand|=0x80;
            }
            else{
                throw std::invalid_argument("Invalid Immediate value");
            }
        }
    }
}

void AssemblyCompiler::checkArgument2(Instruction& instr, std::string& arg2){
    if(arg2[0]=='R' || arg2[0]=='r'){
        std::transform(arg2.begin(),arg2.end(),arg2.begin(),::toupper); //assembly is case-insensitive
        if((int)arg2.size()<4){
            if(arg2[1]<'0' || arg2[1]>'5'){
                throw std::invalid_argument("Invalid Register");
            }
            else if((int)arg2.size()==3 && arg2[2]!=' '){
                throw std::invalid_argument("Invalid Register");
            }
            else{
                instr.Argument2=arg2[1]-'0';
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
            if(Constant.count(arg2)){
                if(Constant[arg2]>=256){
                    throw std::invalid_argument("Constant is too big");
                }
                instr.Argument2=Constant[arg2];
                instr.Operand|=0x40;
            }
            else{
                throw std::invalid_argument("Invalid Immediate value");
            }
        }
    }
}


//destination can also be Out
void AssemblyCompiler::checkDestination(Instruction& instr, std::string& dest){
    if(dest[0]!='R' && dest[0]!='r'){
        std::transform(dest.begin(),dest.end(),dest.begin(),::toupper); //assembly is case-insensitive
        if(dest!="OUT"){
            throw std::invalid_argument("Invalid Destination");
        }
        else{
            instr.Result=Out;
        }
    }
    else{
        std::transform(dest.begin(),dest.end(),dest.begin(),::toupper); //assembly is case-insensitive
        if((int)dest.size()<4){
            if(dest[1]<'0' || dest[1]>'5'){
                throw std::invalid_argument("Invalid Register");
            }
            else if((int)dest.size()==3 && dest[2]!=' '){
                throw std::invalid_argument("Invalid Register");
            }
            else{
                instr.Result=dest[1]-'0';
            }
        }
        else{
            throw std::invalid_argument("Invalid Register");
        }

    }
}


void AssemblyCompiler::removeExtraWhitespaces(std::string&str){
    std::string output;
    std::unique_copy(str.begin(),str.end(),std::back_insert_iterator<std::string>(output),
                [](char a, char b){ return isspace(a) && isspace(b);});
    str=output;
}

void AssemblyCompiler::addBinaryCode(Instruction& instr){
    bincode.push_back(instr.Operand);
    bincode.push_back(instr.Argument1);
    bincode.push_back(instr.Argument2);
    bincode.push_back(instr.Result);
}

//check if string is number with regex
//does not accept leading zeros
bool AssemblyCompiler::isInt(std::string&str){

    constexpr auto max_digits=std::numeric_limits<int>::digits10 ;
    const std::string ub=std::to_string(max_digits-1) ;
    const std::regex int_re("^\\s*([+-]?[1-9]\\d{0,"+ub+"}|0)\\s*$");

    return std::regex_match(str,int_re) ;
}
