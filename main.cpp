#include <iostream>
#include "headers/Mpwd.h"
#include "headers/Mexit.h"
#include "headers/Merrno.h"
#include "headers/Mcd.h"
#include "headers/Command.h"
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include "windows.h"

#include <libgen.h>




extern char **environ;


void mpwdAction(std::string input,myshell::Mpwd *pwd);
void mcdAction(std::string input);
void merrnoAction(std::string input);
int mexitAction(std::string input);
void myCatAction(std::string input,myshell::Mpwd *pwd);
std::vector<std::string> parsePath(std::string path);
std::string trim(const std::string& str);


int main(int argc, char *argv[]) {

    auto  *pwd = new myshell::Mpwd();
    std::string input;
   // bool  working = true;
    while(true){
        std::getline(std::cin, input);

        input = trim(input);

        if(input.find("mpwd") == 0){
            mpwdAction(input, pwd);
            input.clear();
        } else if(input.find("mexit")==0){
            input.clear();
            return mexitAction(input);
        } else if(input.find("mcd")==0){
            mcdAction(input);
            input.clear();
        } else if(input.find("merrno")==0){
            merrnoAction(input);
            input.clear();
        } else if(input.find("mycat")==0){
            myCatAction(input, pwd);
            input.clear();
        }
    }
}

void mpwdAction(std::string input, myshell::Mpwd *pwd){

    if(input.find("-h")!=input.npos||input.find("-help")!=input.npos){
        pwd->help();
    }
    pwd->printCurrentDir();
    input.clear();
   // delete pwd;
}
void mcdAction(std::string input){
    auto start_pos = input.find(" ");
    char path[input.size()-start_pos];
    input.copy(path,input.size(),start_pos+1);
    auto *mcd = new myshell::Mcd();
    if(input.find("-h")!=input.npos||input.find("-help")!=input.npos){
        mcd->help();
    }
    mcd->changeDir(path);
}
void merrnoAction(std::string input){
    auto *merrno = new myshell::Merrno();
    if(input.find("-h")!=input.npos||input.find("-help")!=input.npos){
        merrno->help();
    }
    merrno->printLastError();
}
int mexitAction(std::string input){
    auto *mexit = new myshell::Mexit();
    if(input.find("-h")!=input.npos||input.find("-help")!=input.npos){
        mexit->help();
    }
    delete mexit;
    return  mexit->mexit();
}
void myCatAction(std::string input, myshell::Mpwd* mpwd){
    using namespace std;
    auto start_pos = input.find(" ");
    char name[] = "mycat.exe";
    int name_size = std::string(name).size();
    std::string path = input.substr(start_pos+1);
    std::string pathToMyCat = mpwd->getCurrent_dir() +"\\mycat.exe";
    // additional information
    STARTUPINFO si;
    PROCESS_INFORMATION pi;


    // set the size of the structures
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    // start the program up
    CreateProcess( pathToMyCat.c_str(),   // the path
                   "mycat.exe D:\\UCU\\Arcitechture\\myshell\\file.txt",        // Command line
                   NULL,           // Process handle not inheritable
                   NULL,           // Thread handle not inheritable
                   FALSE,          // Set handle inheritance to FALSE
                   0,              // No creation flags
                   NULL,           // Use parent's environment block
                   NULL,           // Use parent's starting directory
                   &si,            // Pointer to STARTUPINFO structure
                   &pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
    );
    // Close process and thread handles.
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );

}
std::string trim(const std::string& str)
{
    std::string whitespace =" ";
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

std::vector<std::string> parsePath(std::string path){

}



