//
// Created by Victor on 15.03.2018.
//

#include "../headers/Merrno.h"

void myshell::Merrno::help() {
    std::cout<<"Help from Merrno"<<std::endl;
    std::cout<<"merrno - command to display last error\n";
    std::cout<<"Syntax - merrno [-h|--help] "<<std::endl;
}

myshell::Merrno::Merrno() {
    token = "merrno";
}

void myshell::Merrno::printLastError() {

    std::cout<<"Error code: "<<pErrorInfo.error_code<<std::endl;
    if(pErrorInfo.error_code!=0) {
        std::cout << "Error description: " << pErrorInfo.error_info << std::endl;
    }
}
