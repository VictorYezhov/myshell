//
// Created by Victor on 15.03.2018.
//

#ifndef MYSHELL_MPWD_H
#define MYSHELL_MPWD_H
#include <iostream>
#include <unistd.h>
#include "ErrorInfo.h"

namespace  myshell {
    class Mpwd  {
    private:
        std::string current_dir;
        std::string token;
    public:
        Mpwd();
        void help();
        bool printCurrentDir();
        const std::string &getCurrent_dir() const;
        void setCurrent_dir(const std::string &current_dir);
    };
}

#endif //MYSHELL_MPWD_H
