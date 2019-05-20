#include <iostream>
#include "ProcessParser.h"


int main() {
    std::cout << "memory monitor" << std::endl;
    ProcessParser p;
    string ss = ProcessParser::get_cpu_percent("2090");
    cout << ss << endl;
    return 0;
}