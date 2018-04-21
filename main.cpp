

#include <stdio.h>
#include <string.h>

#include <iostream>
#include "headers/Mpwd.h"
#include "headers/Mexit.h"
#include "headers/Merrno.h"
#include "headers/Mcd.h"
#include "headers/Command.h"
#include <sstream>
#include <string>
#include <iterator>
#include <vector>
#include <boost/algorithm/string.hpp>
#include "dirent.h"
#include <libgen.h>

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
inline bool exists_test (const std::string& name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}
#endif

void mpwdAction(std::string input,myshell::Mpwd *pwd);
bool is_file_or_dir(std::string path);
void mcdAction(std::string input);
void merrnoAction(std::string input);
int mexitAction(std::string input);
void commandAction(std::string input,myshell::Mpwd *pwd);
std::vector<std::string> parsePath(std::string path);
bool match(char const *wildcart, char const *target);


int main(int argc, char *argv[]) {

    auto  *pwd = new myshell::Mpwd();
    std::string input;
   // bool  working = true;
    while(true){
        std::cout<< pwd->getWorking_dir()+" $ myshell ";
        std::getline(std::cin, input);
        boost::trim(input);
        if(input.empty()){

        }else
        if(boost::contains(input,"mpwd")){
            mpwdAction(input, pwd);
            input.clear();
        } else if(boost::contains(input,"mexit")){
            delete pwd;
            return mexitAction(input);
        } else if(boost::contains(input,"mcd")){
            mcdAction(input);
            input.clear();
        } else if(boost::contains(input,"merrno")){
            merrnoAction(input);
            input.clear();
        } else{
            commandAction(input, pwd);
            input.clear();
        }
    }
}

void mpwdAction(std::string input, myshell::Mpwd *pwd){

    if(boost::contains(input,"-h")||boost::contains(input,"-help")){
        pwd->help();
    }
    pwd->printCurrentDir();
    input.clear();
}
void mcdAction(std::string input){
    auto start_pos = input.find(" ");
    char path[input.size()-start_pos];
    input.copy(path,input.size(),start_pos+1);
    auto *mcd = new myshell::Mcd();
    if(boost::contains(input,"-h")||boost::contains(input,"-help")){
        mcd->help();
    } else {
        mcd->changeDir(path);
    }
    delete(mcd);
}
void merrnoAction(const std::string input){
    auto *merrno = new myshell::Merrno();
    if(boost::contains(input,"-h")||boost::contains(input,"-help")){
        merrno->help();
    }
    merrno->printLastError();
    delete(merrno);
}
int mexitAction(std::string input){
    std::vector<std::string> data;
    boost::split(data,input,boost::is_any_of(" "), boost::token_compress_on);
    auto *mexit = new myshell::Mexit();
    if(boost::contains(input,"-h")||boost::contains(input,"-help")){
        mexit->help();
    }
    int code;

        code = mexit->mexit(std::atoi(data.at(1).c_str()));
    delete mexit;
    return  code;
}

void commandAction(std::string input, myshell::Mpwd* mpwd){
    using namespace std;

    input = input+" ";
    auto start_pos = input.find(" ");
    auto *com = new myshell::Command();

    std::string path = input.substr(start_pos+1, input.size());

    std::string program_name = input.substr(0,start_pos);

    std::string pathToMyEXE;
    if(boost::equals(program_name, "mycat"))
        pathToMyEXE = mpwd->getWorking_dir() +"/mycat";
    else
        pathToMyEXE = program_name;

    vector<std::string> args = parsePath(path);

    args.erase(args.end());


    com->exec_prog(pathToMyEXE, args);
    delete(com);

}

std::vector<std::string> parsePath(std::string path){
    std::vector<std::string> pathes;
    std::vector<std::string> other;
    boost::split(pathes,path,boost::is_any_of(" "), boost::token_compress_on);
    for(int i=0;i<pathes.size();i++){
        if(!is_file_or_dir(pathes[i]) && !pathes[i].empty()) {
            other.push_back(pathes[i]);
            pathes.erase(pathes.begin()+i);
        }
    }
    std::vector<std::string> dirs;
    std::vector<std::string> names;
    for(auto s : pathes){
        auto * writable = new char[s.size() + 1];
        std::copy(s.begin(), s.end(), writable);
        writable[s.size()] = '\0';
        names.emplace_back(basename(writable));
        dirs.emplace_back(dirname(writable));
        delete[] writable;
    }
    pathes.clear();
    for(auto s: other){
        pathes.push_back(s);
    }
    int i=0;
    for(auto s : names){

            if (boost::contains(s, "*") || boost::contains(s, "?")) {
                DIR *dir;
                struct dirent *ent;
                auto *writable = new char[dirs.at(i).size()];
                std::copy(dirs.at(i).begin(), dirs.at(i).end(), writable);
                // writable[s.size()] = '\0';
                if ((dir = opendir(writable)) != NULL) {
                    /* print all the files and directories within directory */
                    while ((ent = readdir(dir)) != NULL) {
                        if (match(s.c_str(), ent->d_name)) {
                            if (exists_test(dirs.at(i) + "\\" + ent->d_name))
                                pathes.push_back(dirs.at(i) + "\\" + ent->d_name);
                            else {
                                std::cout << "File " << dirs.at(i) + "\\" + ent->d_name << " does not exist"
                                          << std::endl;
                                pErrorInfo.error_code = ENOENT;
                                pErrorInfo.error_info = "No such file or directory\n";
                                pathes.clear();
                                return pathes;
                            }
                        }
                    }
                    closedir(dir);
                    delete[] writable;
                } else {
                    std::cout << "Could not open directory " << writable << " to read files" << std::endl;
                    pErrorInfo.error_code = EACCES;
                    pErrorInfo.error_info = "Permission denied\n";
                    pathes.clear();
                    return pathes;//TODO EXIT
                }
            } else {
                if (exists_test(dirs.at(i) + "\\" + s))
                    pathes.push_back(dirs.at(i) + "\\" + s);
                else {
                    std::cout << "File " << dirs.at(i) + "\\" + s << " does not exist" << std::endl;
                    pErrorInfo.error_code = ENOENT;
                    pErrorInfo.error_info = "No such file or directory\n";
                    pathes.clear();
                    return pathes;
                }
            }
            i++;
        }

    if(pathes.empty()){
        pErrorInfo.error_code = ENOENT;
        pErrorInfo.error_info = "No such file or directory\n";
        return  pathes;
    }

}


bool match(char const *wildcart, char const *target) {
    for (; *wildcart != '\0'; ++wildcart) {
        switch (*wildcart) {
            case '?':
                if (*target == '\0')
                    return false;
                ++target;
                break;
            case '*': {
                if (wildcart[1] == '\0')
                    return true;
                size_t max = strlen(target);
                for (size_t i = 0; i < max; i++)
                    if (match(wildcart + 1, target + i))
                        return true;
                return false;
            }
            case '[':{
                while(*wildcart!=']'||*wildcart=='\0'){
                    if(*target == *wildcart)
                        return true;
                    wildcart++;
                }
                return false;
            }
            default:
                if (*target != *wildcart)
                    return false;
                ++target;
        }
    }
    return *target == '\0';
}

bool is_file_or_dir(std::string path) {
    struct stat sb;
    return stat(path.c_str(), &sb) != -1;
}