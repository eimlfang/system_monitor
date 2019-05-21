#include <algorithm>
#include <iostream>
#include <math.h>
#include <thread>
#include <chrono>
#include <iterator>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <dirent.h>
#include <time.h>
#include <unistd.h>
#include "util.h"
#include "constants.h"

using namespace std;

class ProcessParser
{
public:
    static string get_cmd(string pid);
    static vector<string> get_pid_list();
    static string get_vm_size(string pid);
    static string get_cpu_percent(string pid);
    static long int get_sys_up_time();
    static string get_proc_up_time(string pid);
    static string get_proc_user(string pid);
    static vector<string> get_sys_cpu_percent(string coreNumber = "");
    static float get_sys_ram_percent();
    static string get_sys_kernel_version();
    static int get_total_threads();
    static int get_total_number_of_processes();
    static int get_number_of_running_processes();
    static string get_os_name();
    static string print_cpu_stats(vector<string> values1, vector<string> values2);
    static int get_number_of_cores();
};

string ProcessParser::get_vm_size(string pid)
{
    string line;
    string name = "VmData";
    string value;
    float result;
    ifstream stream = Util::getStream((Path::basePath() + pid + Path::statusPath()));
    while (std::getline(stream, line))
    {
        cout << "scan: " << line << "\n";
        if (line.compare(0, name.size(), name) == 0)
        {

            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);
            result = (stof(values[1]) / float(1024));
            break;
        }
    }
    return to_string(result);
}

string ProcessParser::get_cpu_percent(string pid)
{

    string line;
    string value;
    float result;
    ifstream stream = Util::getStream((Path::basePath() + pid + "/" + Path::statPath()));
    getline(stream, line);
    string str = line;
    istringstream buf(str);
    istream_iterator<string> beg(buf), end;
    vector<string> values(beg, end);
    float utime = stof(ProcessParser::get_proc_up_time(pid));

    float stime = stof(values[14]);
    float cutime = stof(values[15]);
    float cstime = stof(values[16]);
    float startTime = stof(values[21]);

    float uptime = ProcessParser::get_sys_up_time();

    float freq = sysconf(_SC_CLK_TCK);

    float total_time = utime + stime + cutime + cstime;

    float seconds = uptime - (startTime / freq);
    result = 100.0 * ((total_time / freq) / seconds);

    return to_string(result);
}

string ProcessParser::get_proc_up_time(string pid)
{
    string line;
    string value;
    float result;
    ifstream stream = Util::getStream((Path::basePath() + pid + "/" + Path::statPath()));
    getline(stream, line);
    string str = line;
    istringstream buf(str);
    istream_iterator<string> beg(buf), end;
    vector<string> values(beg, end);
    return to_string(float(stof(values[13]) / sysconf(_SC_CLK_TCK)));
}

long int ProcessParser::get_sys_up_time()
{
    string line;
    string value;
    float result;
    ifstream stream = Util::getStream((Path::basePath() + Path::upTimePath()));
    getline(stream, line);
    string str = line;
    istringstream buf(str);
    istream_iterator<string> beg(buf), end;
    vector<string> values(beg, end);
    return stoi(values[0]);
}

string ProcessParser::get_proc_user(string pid)
{
    string line;
    string name = "Uid:";
    string result = "";
    ifstream stream = Util::getStream((Path::basePath() + pid + Path::statusPath()));
    while (getline(stream, line))
    {
        if (line.compare(0, name.size(), name) == 0)
        {
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);
            result = values[1];
            break;
        }
    }
    stream = Util::getStream("/etc/passwd");
    name = ("x:" + result);
    while (getline(stream, line))
    {
        if (line.find(name) != string::npos)
        {
            result = line.substr(0, line.find(":"));
            return result;
        }
    }
    return "";
}

vector<string> ProcessParser::get_pid_list()
{
    DIR *dir;

    vector<string> container;
    if (!(dir = opendir("/proc")))
        throw std::runtime_error(std::strerror(errno));

    while (dirent *dirp = readdir(dir))
    {
        if (dirp->d_type != DT_DIR)
            continue;
        if (all_of(dirp->d_name, dirp->d_name + std::strlen(dirp->d_name), [](char c) { return std::isdigit(c); }))
        {
            container.push_back(dirp->d_name);
        }
    }

    if (closedir(dir))
        throw std::runtime_error(std::strerror(errno));
    return container;
}

string ProcessParser::get_cmd(string pid)
{
    string line;
    ifstream stream = Util::getStream((Path::basePath() + pid + Path::cmdPath()));
    getline(stream, line);
    return line;
}

int ProcessParser::get_number_of_cores()
{
    string line;
    string name = "cpu cores";
    ifstream stream = Util::getStream((Path::basePath() + "cpuinfo"));
    while (getline(stream, line))
    {
        if (line.compare(0, name.size(), name) == 0) {
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);
            return stoi(values[3]);
        }
    }
    return 0;
}