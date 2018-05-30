
#include "headers/Desider.h"

int main(int argc, char *argv[]) {

    auto  *pwd = new myshell::Mpwd();
    std::string input;
    Desider desider(pwd);
    // bool  working = true;
    while(true){
        std::cout<< pwd->printCurrentDir()+"myshell $  ";
        std::getline(std::cin, input);
        boost::trim(input);
        desider.deside(input, pwd);
    }
    delete (pwd);
}

