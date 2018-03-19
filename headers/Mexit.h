//
// Created by Victor on 15.03.2018.
//

#ifndef MYSHELL_MEXIT_H
#define MYSHELL_MEXIT_H
#include <iostream>

namespace  myshell {
    class Mexit  {
    private:
        std::string token;
    public:
        Mexit();
        void help();
        int mexit(int code);
        int mexit();
    };

}
#endif //MYSHELL_MEXIT_H
