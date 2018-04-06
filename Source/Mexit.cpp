//
// Created by Victor on 15.03.2018.
//

#include "../headers/Mexit.h"

void myshell::Mexit::help() {
    std::cout<<"Help from Mexit"<<std::endl;
    std::cout<<"mexit - command to exit myshell with specified code\nIf no code specified exits with code 0\n";
    std::cout<<"Syntax: mexit [exit code] [-h|--help]"<<std::endl;
}

myshell::Mexit::Mexit():token("mexit") {
}

int myshell::Mexit::mexit(int code) {
    return code;
}

int myshell::Mexit::mexit() {
    return 0;
}
