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
    char lpszComLine[1024];  // для командной строки

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    std::stringstream ss;
    ss<<pathToMyEXE<<" ";
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
        std::cout<<("Failed to execute "+pathToMyEXE+"\n");
        pErrorInfo.error_code = errno;
        pErrorInfo.error_info = "Error while executing " + pathToMyEXE +"\n";
    }
    // закрываем дескрипторы нового процесса
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
}

#endif



