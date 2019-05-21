#include <iostream>
#include "ProcessParser.h"


int main() {
    std::cout << "memory monitor" << std::endl;
    ProcessParser p;
    vector<string> ss = ProcessParser::get_pid_list();
    for (string a : ss)
    {
        cout << a << endl;
    }
    
    return 0;
}