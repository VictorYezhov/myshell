//
// Created by Victor on 31.05.2018.
//

#include "../headers/Desider.h"

int Desider::deside(std::string input, myshell::Mpwd *pwd) {

    if(input.empty()){
    }else
    if(boost::contains(input,"mpwd")){
        mpwdAction(input, pwd);
        input.clear();
    } else if(boost::contains(input,"mexit")){
        delete pwd;
        exit(mexitAction(input));
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

void Desider::mpwdAction(std::string input, myshell::Mpwd *pwd){

    if(boost::contains(input,"-h")||boost::contains(input,"-help")){
        pwd->help();
    }
    std::cout<<pwd->printCurrentDir()<<std::endl;
    input.clear();
}
void Desider::mcdAction(std::string input){
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
void Desider::merrnoAction(const std::string input){
    auto *merrno = new myshell::Merrno();
    if(boost::contains(input,"-h")||boost::contains(input,"-help")){
        merrno->help();
    }
    merrno->printLastError();
    delete(merrno);
}
int Desider::mexitAction(std::string input){
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

void Desider::commandAction(std::string input, myshell::Mpwd* mpwd){
    using namespace std;
    auto *com = new myshell::Command();

    if(boost::algorithm::ends_with(input, ".msh")){
        vector<std::string> args = parsePath(input);
        return;
    }
    if(boost::contains(input,"=")){
        unsigned long  pos = input.find('=');
        variables[input.substr(0, pos)] = input.substr(pos+1, input.size());
        std::cout<<"variable: "<<input.substr(0, pos)<<" saved with value: "<< variables[input.substr(0, pos)]<<std::endl;
        return;
    }

    input = input+" ";

    auto start_pos = input.find(" ");


    std::string path = input.substr(start_pos+1, input.size());

    std::string program_name = input.substr(0,start_pos);

    std::string pathToMyEXE;
    if(boost::equals(program_name, "mycat"))
        pathToMyEXE = mpwd->getWorking_dir() +"/mycat";
    else
        pathToMyEXE = program_name;


    vector<std::string> args = parsePath(path);

    if(!args.empty())
        args.erase(args.end()-1);

    com->exec_prog(pathToMyEXE, args);
    delete(com);

}

std::vector<std::string> Desider::parsePath(std::string path){




    std::vector<std::string> pathes;
    std::vector<std::string> other;
    boost::split(pathes,path,boost::is_any_of(" "), boost::token_compress_on);


    if(place_in_out(pathes)){
        read_args_from_file(pathes);
        return pathes;
    }


    if(path.empty())
        return pathes;
    for(int i=0;i<pathes.size();i++){
        if(boost::algorithm::ends_with(pathes[i], ".msh")){
            execute(pathes[i]);
            return pathes;
        }
        if(boost::algorithm::starts_with(pathes[i], "$")){
            other.push_back(variables[pathes[i].substr(1, pathes[i].size())]);
            pathes.erase(pathes.begin()+i);
            if(i!=0)
                i--;
        }

        if(!is_file_or_dir(pathes[i]) && !pathes[i].empty()) {
            other.push_back(pathes[i]);
            pathes.erase(pathes.begin()+i);
            i--;
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
                        if (exists_test(dirs.at(i) + "/" + ent->d_name))
                            pathes.push_back(dirs.at(i) + "/" + ent->d_name);
                        else {
                            std::cout << "File " << dirs.at(i) + "/" + ent->d_name << " does not exist"
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
                return pathes;
            }
        } else {
            if (exists_test(dirs.at(i) + "/" + s))
                pathes.push_back(dirs.at(i) + "/" + s);
            else {
                std::cout << "File " << dirs.at(i) + "/" + s << " does not exist" << std::endl;
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
    return pathes;

}


bool Desider::match(char const *wildcart, char const *target) {
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

bool Desider::is_file_or_dir(std::string path) {


#ifdef _WIN32
    GetFileAttributes(path.c_str());
    if(INVALID_FILE_ATTRIBUTES == GetFileAttributes(path.c_str()) && GetLastError()==ERROR_FILE_NOT_FOUND)
    {
        return false;
    } else{
        return true;
    }
#else
    struct stat sb;
    return stat(path.c_str(), &sb) != -1;
#endif
}


bool Desider::place_in_out(std::vector<std::string> &input){




    for(int i =0; i< input.size(); i++){
        if(input[i]==">" && input[i+2]=="2>&1"){
            iofiles.ferr = input[i+1];
            iofiles.fout = input[i+1];
            input.erase(input.begin()+i);
            input.erase(input.begin()+i);
            input.erase(input.begin()+i);
            return false;
        }
        if(input[i]==">"){
            iofiles.fout  = input[i+1];
            input.erase(input.begin()+i);
            input.erase(input.begin()+i);
            return false;

        }
        if(input[i]=="2>"){
            iofiles.ferr = input[i+1];
            input.erase(input.begin()+i);
            input.erase(input.begin()+i);
            return false;
        }

        if(input[i]=="<"){
            iofiles.fin  = input[i+1];
            input.erase(input.begin()+i);
            input.erase(input.begin()+i);
            return true;
        }

    }
}
void Desider::read_args_from_file(std::vector<std::string> &args) {

    std::ifstream fin(iofiles.fin);
    std::string str;

    while (fin >> str) // Will read up to eof() and stop at every
    {
        if(str != "'':")
            args.push_back(str);
    }
}

void Desider::execute(std::string script) {

    std::ifstream infile(script);

    std::string str;
    while (std::getline(infile, str)) {

        if (boost::starts_with(str, "#")){

        } else
        if (boost::starts_with(str, echo)){
            std::cout<< str.substr(5, str.size()-1)<<std::endl;
        } else{

            deside(str, mpwd);
        }
    }
}

Desider::Desider(myshell::Mpwd *mpwd) : mpwd(mpwd) {}
