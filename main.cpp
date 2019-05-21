#include <iostream>
#include "ProcessParser.h"

int main()
{
    std::cout << "memory monitor" << std::endl;
    ProcessParser p;
    auto ss = ProcessParser::get_sys_cpu_percent(to_string(ProcessParser::get_number_of_cores()));
    for ( auto a : ss ) 
    {
        cout << a << " ";
    }

    cout << endl;
    return 0;
}