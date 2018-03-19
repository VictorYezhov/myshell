//
// Created by Victor on 15.03.2018.
//

#include "../headers/Mpwd.h"

void myshell::Mpwd::help() {
    std::cout<<"Help from Mpwd"<<std::endl;
    std::cout<<"mpwd command to display current directory\n";
    std::cout<<"Syntax: mpwd [-h|--help]"<<std::endl;
}

myshell::Mpwd::Mpwd() {
    token = "mpwd";
    char buf[100];
    current_dir = getcwd(buf, 100);
}

bool myshell::Mpwd::printCurrentDir() {
    char buf[100];
    auto dir = getcwd(buf, 100);
    if(errno == ERANGE){
        pErrorInfo.error_code = ERANGE;
        pErrorInfo.error_info = "Error while getting current directory";
        std::cout<<"Exeption!"<<std::endl;
        return false;
    }
    std::cout<<dir<<std::endl;
    pErrorInfo.error_code = 0;
    return true;
}


const std::string &myshell::Mpwd::getCurrent_dir() const {
    return current_dir;
}

void myshell::Mpwd::setCurrent_dir(const std::string &current_dir) {
    Mpwd::current_dir = current_dir;
}


