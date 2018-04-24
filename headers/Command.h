//
// Created by Victor on 15.03.2018.
//

#ifndef MYSHELL_MYSHELL_H
#define MYSHELL_MYSHELL_H
#ifdef _WIN32
#include "windows.h"
#include <string.h>
#include <sstream>
#include<winbase.h>
#endif
#if defined(linux) || defined(__unix__) || defined(__linux__) || defined(__CYGWIN__) || defined(__APPLE__)
#include <unistd.h>
#include <sys/stat.h>
#endif


#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#include "ErrorInfo.h"

namespace  myshell {

   class Command {
   private:
       std::string token;
    public:
       void help();
       Command();
       int exec_prog(std::string pathToMyEXE, std::vector<std::string> files);
   };
}
#endif //MYSHELL_MYSHELL_H
