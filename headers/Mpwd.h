//
// Created by Victor on 15.03.2018.
//

#ifndef MYSHELL_MPWD_H
#define MYSHELL_MPWD_H
#if defined(__CYGWIN__)
#include <windows.h>
#include <unistd.h>
#elif defined(linux) || defined(__unix__) || defined(__linux__) || defined(__CYGWIN__) || defined(__APPLE__)
#include <unistd.h>
#elif defined(_WIN32)
#include <direct.h>
#endif
#include <iostream>
#include "ErrorInfo.h"

namespace  myshell {
    class Mpwd  {
    private:
        std::string working_dir;
        std::string token;
    public:
        Mpwd();
        void help();
        std::string printCurrentDir();
        const std::string &getWorking_dir() const;
        void setCurrent_dir(const std::string &current_dir);
    };
}

#endif //MYSHELL_MPWD_H
