#include <iostream>
#include "ProcessParser.h"

int main()
{
    std::cout << "memory monitor" << std::endl;
    ProcessParser p;
    auto ss = ProcessParser::get_number_of_cores();
    cout << ss << endl;

    return 0;
}