//
// Created by Victor on 15.03.2018.
//

#include "../headers/Command.h"

void myshell::Command::help() {
    std::cout<<"Help from myshell"<<std::endl;
}

myshell::Command::Command() {
     token = "myshell";

}


