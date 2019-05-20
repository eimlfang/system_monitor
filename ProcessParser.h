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

class ProcessParser {
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
        static string print_cpu_stats(vector<string> values1, vector<string>values2);
};

string ProcessParser::get_vm_size(string pid) {
    string line;
    string name = "VmData";
    string value;
    float result;
    ifstream stream = Util::getStream((Path::basePath() + pid + Path::statusPath()));
    while (std::getline(stream, line))
    {
        cout << "scan: " << line << "\n";
        if (line.compare(0, name.size(), name) == 0) {
            
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);
            result = (stof(values[1])/float(1024));
            break;
        }
    }
    return to_string(result);
}

string ProcessParser::get_cpu_percent(string pid) {

    string line;
    ifstream stream = Util::getStream((Path::basePath() + pid + Path::statPath()));

    vector<string> pars = {};
    while (std::getline(stream, line, ' '))
    {
        pars.push_back(line);
    }
    
    float utime = 0.0;//stof(ProcessParser::get_proc_up_time(pid));

    float stime = stof(pars[14]);
    float cutime = stof(pars[15]);
    float cstime = stof(pars[16]);
    float startTime = stof(pars[21]);

    cout << "stime = " << stime << endl;
    cout << "cutime = " << cutime << endl;
    cout << "cstime = " << cstime << endl;
    cout << "startaTime = " << startTime << endl;

    float uptime = 0.0;//ProcessParser::get_sys_up_time();

    float freq = sysconf(_SC_CLK_TCK);

    cout << "freq = " << freq << endl;

    float total_time = utime + stime + cutime + cstime;

    cout << "total_time = " << total_time << endl;

    float seconds = uptime - (startTime/freq);
    float result = 100.0*((total_time/freq)/seconds);

    return to_string(result);
}
