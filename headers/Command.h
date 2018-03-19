//
// Created by Victor on 15.03.2018.
//

#ifndef MYSHELL_MYSHELL_H
#define MYSHELL_MYSHELL_H
#include <iostream>
#include <string>
#include <unistd.h>
#include "ErrorInfo.h"

namespace  myshell {

   class Command {
   private:
       std::string token;
    public:
       virtual void help();
       Command();
   };
}
#endif //MYSHELL_MYSHELL_H
