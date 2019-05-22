#pragma once

#include "Process.h"
#include <string>
#include <vector>

using namespace std;

class ProcessContainer
{
public:
    ProcessContainer()
    {
        this->refresh_list();
    }
    void refresh_list();
    string print_list();
    vector<string> get_list();

private:
    vector<Process> _list;
};

void ProcessContainer::refresh_list()
{
    vector<string> pids = ProcessParser::get_pid_list();
    this->_list.clear();
    for (auto pid : pids)
    {
        Process proc(pid);
        this->_list.push_back(proc);
    }
}

string ProcessContainer::print_list()
{
    std::string result = "";
    for (auto i : this->_list)
    {
        result += i.get_process();
    }
    return result;
}

vector<string> ProcessContainer::get_list()
{
    vector<string> values;
    for (int i = (this->_list.size() - 10); i < this->_list.size(); i++)
    {
        values.push_back(this->_list[i].get_process());
    }
    return values;
}