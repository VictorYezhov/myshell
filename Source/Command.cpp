//
// Created by Victor on 15.03.2018.
//

#include <vector>
#include "../headers/Command.h"

void myshell::Command::help() {
    std::cout<<"Help from myshell"<<std::endl;
}

myshell::Command::Command() {
     token = "myshell";

}


#ifdef _WIN32
int myshell::Command::exec_prog(std::string pathToMyEXE, std::vector<std::string> files){

    GetFileAttributes(pathToMyEXE.c_str()); // from winbase.h
    if(INVALID_FILE_ATTRIBUTES == GetFileAttributes(pathToMyEXE) && GetLastError()==ERROR_FILE_NOT_FOUND)
    {
        std::cerr<<"No such command found " + pathToMyEXE+"\n";
        pErrorInfo.error_code = -1;
        pErrorInfo.error_info = "Error while executing " + pathToMyEXE +"\n";
        return  -1;
    }

    char lpszComLine[1024];

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    std::stringstream ss;
    ss<<pathToMyEXE<<" ";
    for(auto file : files){
        ss << file << " ";
    }
    ss<< '\0';


    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb=sizeof(STARTUPINFO);

    wsprintf(lpszComLine, ss.str().c_str());
    std::cout<<lpszComLine<<"\n";

    if (!CreateProcess(
            nullptr,    //  procces name
            lpszComLine,  // arguments
           nullptr,    // defence attr
           nullptr,    // thread deffence args
            TRUE,    // inherit descriptors of current process

           FALSE,  // open in new console
           nullptr,    // env
           nullptr,    // disk
            &si,     //
            &pi      //
            ){
        std::cout<<("Failed to execute "+pathToMyEXE+"\n");
        pErrorInfo.error_code = errno;
        pErrorInfo.error_info = "Error while executing " + pathToMyEXE +"\n";
    }
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}
#else
int myshell::Command::exec_prog(std::string pathToMyEXE, std::vector<std::string> files) {
    using namespace std;


    char *args[files.size()+1];
    char *writable;
    writable = new char[pathToMyEXE.size() + 2];
    std::copy(pathToMyEXE.begin(), pathToMyEXE.end(), writable);
    writable[pathToMyEXE.size()] = '\0';
    args[0] = writable;
    struct stat sb;
    if(stat(pathToMyEXE.c_str(), &sb) == -1){
        std::cerr<<"No such command found " + pathToMyEXE+"\n";
        pErrorInfo.error_code = -1;
        pErrorInfo.error_info = "Error while executing " + pathToMyEXE +"\n";
        return  -1;
    }


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
        execv(pathToMyEXE.c_str(), args);
        printf("Return not expected. Must be an execv error.n");
        std::cout<<"Errno = " << errno;
        pErrorInfo.error_code = errno;
        pErrorInfo.error_info = "Error while executing " + pathToMyEXE +"\n";
    }
    return 0;
}

#endif



