//
// Created by Victor on 15.03.2018.
//

#ifndef MYSHELL_MCD_H
#define MYSHELL_MCD_H
#include <iostream>
#include <unistd.h>

namespace  myshell {

    class Mcd  {
    private:
        std::string token;
    public:
        Mcd();
        void help();
        int changeDir(std::string path);
    };


}
#endif //MYSHELL_MCD_H
