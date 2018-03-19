//
// Created by Victor on 15.03.2018.
//

#ifndef MYSHELL_MERRNO_H
#define MYSHELL_MERRNO_H
#include <iostream>
#include "ErrorInfo.h"



namespace  myshell {
    class Merrno {
    private:
        std::string token;
    public:
        Merrno();
        void help();
        void printLastError();


    };


}
#endif //MYSHELL_MERRNO_H
