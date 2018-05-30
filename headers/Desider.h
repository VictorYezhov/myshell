//
// Created by Victor on 31.05.2018.
//

#ifndef MYSHELL_DESIDER_H
#define MYSHELL_DESIDER_H

#include <string>

#include <iostream>
#include "Mpwd.h"
#include "Mexit.h"
#include "Merrno.h"
#include "Mcd.h"
#include "Command.h"
#include <sstream>
#include <iterator>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <fstream>
#include "IOStreams.h"
#include <map>

#ifdef _WIN32

inline bool exists_test(const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}
#else
#include "dirent.h"

#endif

class Desider {

private:

    std::string echo{"echo"};
    myshell::Mpwd *mpwd;
    void execute(std::string script);
    std::map<std::string, std::string> variables;
public:
    int deside(std::string input, myshell::Mpwd *mpwd);
    void mpwdAction(std::string input,myshell::Mpwd *pwd);
    bool is_file_or_dir(std::string path);
    void mcdAction(std::string input);
    void merrnoAction(std::string input);
    int mexitAction(std::string input);
    void commandAction(std::string input,myshell::Mpwd *pwd);
    std::vector<std::string> parsePath(std::string path);
    bool match(char const *wildcart, char const *target);
    bool place_in_out(std::vector<std::string> &input);
    void read_args_from_file(std::vector<std::string> &args);

    inline bool exists_test (const std::string& name) {
        struct stat buffer;
        return (stat (name.c_str(), &buffer) == 0);
    }

    Desider(myshell::Mpwd *mpwd);
};


#endif //MYSHELL_DESIDER_H
