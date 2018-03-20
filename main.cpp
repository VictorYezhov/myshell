
#ifdef _WIN32
#include "windows.h"
#include <string.h>
#endif
#include <stdio.h>
#include <string.h>

#if defined(linux) || defined(__unix__) || defined(__linux__) || defined(__CYGWIN__) || defined(__APPLE__)
#include <unistd.h>
#include <sys/stat.h>
#endif
#if defined(__cplusplus) || defined(_cplusplus)
#include <exception>  // terminate()
#else
#include <cstdlib>    // abort()
#endif
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
void mcdAction(std::string input);
void merrnoAction(std::string input);
int mexitAction(std::string input);
void myCatAction(std::string input,myshell::Mpwd *pwd);
std::vector<std::string> parsePath(std::string path);
bool match(char const *wildcart, char const *target);
static int exec_prog(std::string pathToMyCat, std::vector<std::string> files);
#ifdef _WIN32
static int exec_prog_win_32(std::string pathToMyCat, std::vector<std::string> files);
#endif
int main(int argc, char *argv[]) {


    auto  *pwd = new myshell::Mpwd();
    std::string input;
   // bool  working = true;
    while(true){
        std::getline(std::cin, input);

        boost::trim(input);

        if(boost::contains(input,"mpwd")){
            mpwdAction(input, pwd);
            input.clear();
        } else if(boost::contains(input,"mexit")){
            input.clear();
            return mexitAction(input);
        } else if(boost::contains(input,"mcd")){
            mcdAction(input);
            input.clear();
        } else if(boost::contains(input,"merrno")){
            merrnoAction(input);
            input.clear();
        } else if(boost::contains(input,"mycat")){
            myCatAction(input, pwd);
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
   // delete pwd;k
}
void mcdAction(std::string input){
    auto start_pos = input.find(" ");
    char path[input.size()-start_pos];
    input.copy(path,input.size(),start_pos+1);
    auto *mcd = new myshell::Mcd();
    if(boost::contains(input,"-h")||boost::contains(input,"-help")){
        mcd->help();
    }
    mcd->changeDir(path);
}
void merrnoAction(std::string input){
    auto *merrno = new myshell::Merrno();
    if(boost::contains(input,"-h")||boost::contains(input,"-help")){
        merrno->help();
    }
    merrno->printLastError();
}
int mexitAction(std::string input){
    auto *mexit = new myshell::Mexit();
    if(boost::contains(input,"-h")||boost::contains(input,"-help")){
        mexit->help();
    }
    delete mexit;
    return  mexit->mexit();
}
void myCatAction(std::string input, myshell::Mpwd* mpwd){
    using namespace std;
    auto start_pos = input.find(" ");
   // char name[] = "mycat.exe";
  //  int name_size = std::string(name).size();
    std::string path = input.substr(start_pos+1);
    std::string pathToMyCat = mpwd->getCurrent_dir() +"\\mycat";
    vector<std::string> args = parsePath(path);
    if(args.empty()){
        return;
    }
    // additional information
#ifdef _WIN32
        exec_prog_win_32(pathToMyCat,args);
#else
        exec_prog(pathToMyCat,args);
#endif
}

std::vector<std::string> parsePath(std::string path){
    std::vector<std::string> pathes;
    boost::split(pathes,path,boost::is_any_of(" "), boost::token_compress_on);

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
    int i=0;
    for(auto s : names){
        if(boost::contains(s,"-h") ||boost::contains(s,"-help") ) {
            pathes.push_back("-h");
            i++;
            continue;
        }
        //boost::replace_all(s,"X", " ");
        if(boost::contains(s,"*")|| boost::contains(s, "?")){
            DIR *dir;
            struct dirent *ent;
            auto * writable = new char[dirs.at(i).size()];
            std::copy(dirs.at(i).begin(), dirs.at(i).end(), writable);
           // writable[s.size()] = '\0';
            if ((dir = opendir (writable)) != NULL) {
                /* print all the files and directories within directory */
                while ((ent = readdir (dir)) != NULL) {
                    if(match(s.c_str(),ent->d_name)) {
                        if(exists_test(dirs.at(i)+"\\"+ ent->d_name))
                            pathes.push_back(dirs.at(i) + "\\" + ent->d_name);
                        else{
                            std::cout<<"File " << dirs.at(i) + "\\" + ent->d_name<<" does not exist"<<std::endl;
                            pErrorInfo.error_code = ENOENT;
                            pErrorInfo.error_info = "No such file or directory\n";
                            pathes.clear();
                            return  pathes;
                        }
                    }
                }
                closedir (dir);
                delete[] writable;
            } else {
                /* could not open directory */
                std::cout<<"Could not open directory "<< writable <<" to read files"<<std::endl;
                pErrorInfo.error_code = EACCES;
                pErrorInfo.error_info = "Permission denied\n";
                pathes.clear();
                return  pathes;//TODO EXIT
            }
        } else{
            if(exists_test(dirs.at(i)+"\\"+s))
                pathes.push_back(dirs.at(i)+"\\"+s);
            else{
                std::cout<<"File " << dirs.at(i) + "\\" + s<<" does not exist"<<std::endl;
                pErrorInfo.error_code = ENOENT;
                pErrorInfo.error_info = "No such file or directory\n";
                pathes.clear();
                return  pathes;
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



static int exec_prog(std::string pathToMyCat, std::vector<std::string> files)
{
    using namespace std;

    char *args[files.size()+1];
    char *writable;
    writable = new char[files.at(0).size() + 2];
    std::copy(files.at(0).begin(), files.at(0).end(), writable);
    writable[files.at(0).size()] = '\0';
    args[0] = writable;


    for(int i=0; i< files.size(); i++){
        writable = new char[files.at(i).size() + 1];
        std::copy(files.at(i).begin(), files.at(i).end(), writable);
        writable[files.at(i).size()] = '\0';
        args[i+1] = writable;
    }
    args[files.size()+1] = nullptr;

        pid_t pid;

    if ((pid = fork()) == -1)
        perror("fork error");
    else if (pid == 0) {
        execv(pathToMyCat.c_str(), args);
        printf("Return not expected. Must be an execv error.n");
        std::cout<<"Errno = " << errno;
    }


}
#ifdef _WIN32
static int exec_prog_win_32(std::string pathToMyCat, std::vector<std::string> files){
    char lpszComLine[1024];  // для командной строки

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    SECURITY_ATTRIBUTES sa;

    HANDLE  hThread;
    DWORD  IDThread;


    std::stringstream ss;
    ss<<"mycat.exe"<<" ";
    for(auto file : files){
        ss << file << " ";
    }
    ss<< '\0';

    // устанавливаем атрибуты нового процесса
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb=sizeof(STARTUPINFO);
    // формируем командную строку
    wsprintf(lpszComLine, ss.str().c_str());
    std::cout<<lpszComLine<<"\n";
    // запускаем новый консольный процесс
    if (!CreateProcess(
            nullptr,    // имя процесса
            lpszComLine,  // адрес командной строки
           nullptr,    // атрибуты защиты процесса по умолчанию
           nullptr,    // атрибуты защиты первичного потока по умолчанию
            TRUE,    // наследуемые дескрипторы текущего процесса
            // наследуются новым процессом
           FALSE,  // новая консоль
           nullptr,    // используем среду окружения процесса предка
           nullptr,    // текущий диск и каталог, как и в процессе предке
            &si,     // вид главного окна - по умолчанию
            &pi      // здесь будут дескрипторы и идентификаторы
            // нового процесса и его первичного потока
    )
            ){
        std::cout<<("Failed to execute mycat.\n");
    }
    // закрываем дескрипторы нового процесса
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}
#endif

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