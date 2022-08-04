#include"AssemblyCompiler.h"
#include<iostream>
int main(int argc, char* argv[])
{
    if(argc<2)
    {
        std::cerr<<"Not enough arguments are provided"<<std::endl;
        return 1;
    }
    else{
        std::string str(argv[1]);
        AssemblyCompiler compiler(str);
        int state=compiler.binFileInit();
        if(state==0){
            std::cout<<"Success!!"<<std::endl;
        }
        else{
            std::cerr<<"Failed to create binary file"<<std::endl;
        }
    }
}
