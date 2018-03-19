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
myshell::Mcd::Mcd() {
    token = "mcd";
}

int myshell::Mcd::changeDir(std::string path){
    int res = chdir(path.c_str());
    if(res == -1){
        pErrorInfo.error_code = res;
        pErrorInfo.error_info = "Error happened while changing directoty ";
        return res;
    }
}
