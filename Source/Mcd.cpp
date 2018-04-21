//
// Created by Victor on 15.03.2018.
//

#include "../headers/Mcd.h"
#include "../headers/ErrorInfo.h"

void myshell::Mcd::help() {
    std::cout<<"Help from Mcd"<<std::endl;
    std::cout<<"mcd - command to change current directory\n";
    std::cout<<"Syntax: mcd <path> [-h|--help]"<<std::endl;
}
myshell::Mcd::Mcd():token("mcd") {
}

int myshell::Mcd::changeDir(std::string path){
    int res;
    if(path =="..\u0001"){
        char writable[100];
        res =  chdir(dirname(getcwd(writable, 100)));
    } else {
        res = chdir(path.c_str());
    }
    if(res == -1){
        std::cerr<<"Error happened while changing directoty \n";
        pErrorInfo.error_code = res;
        pErrorInfo.error_info = "Error happened while changing directoty ";
        return res;
    }
}
