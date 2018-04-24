//
// Created by Victor on 15.03.2018.
//

#include "../headers/Mpwd.h"

void myshell::Mpwd::help() {
    std::cout<<"Help from Mpwd"<<std::endl;
    std::cout<<"mpwd command to display current directory\n";
    std::cout<<"Syntax: mpwd [-h|--help]"<<std::endl;
}

myshell::Mpwd::Mpwd():token("mpwd") {

    char buf[100];
#ifdef __CYGWIN__
    GetModuleFileName( NULL, buf, 100 );
    std::string::size_type pos = std::string( buf ).find_last_of( "\\/" );
    working_dir = std::string( buf ).substr( 0, pos);
#else
    working_dir = getcwd(buf, 100);
     std::cout<<working_dir<<"\n";
#endif
}

std::string myshell::Mpwd::printCurrentDir() {
    char buf[100];
    auto dir = getcwd(buf, 100);
    if(errno == ERANGE){
        pErrorInfo.error_code = ERANGE;
        pErrorInfo.error_info = "Error while getting current directory";
        std::cout<<"Exeption!"<<std::endl;
        return nullptr;
    }
    return dir;
}


const std::string &myshell::Mpwd::getWorking_dir() const {
    return working_dir;
}

void myshell::Mpwd::setCurrent_dir(const std::string &current_dir) {
    Mpwd::working_dir = current_dir;
}


