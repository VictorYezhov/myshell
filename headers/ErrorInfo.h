//
// Created by Victor on 15.03.2018.
//

#ifndef MYSHELL_ERRORINFO_H
#define MYSHELL_ERRORINFO_H

#include <string>

struct Error_Info{
    int error_code;
    std::string error_info;
};

extern Error_Info  pErrorInfo;


#endif //MYSHELL_ERRORINFO_H
