#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "format.h"
#include "linux_parser.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { 
    return cpu_; 
}

vector<Process>& System::Processes() {
    vector<int> pids{LinuxParser::Pids()};

    set<int> current_pids;
    for (Process const& proc : processes_) {
        current_pids.insert(proc.Pid());
    }

    for (int pid : pids) {
        if (current_pids.find(pid) == current_pids.end()) {
            processes_.emplace_back(pid);
        }
    }

    for (auto& proc : processes_) {
        proc.CpuUtilization(LinuxParser::ActiveJiffies(proc.Pid()), 
                            LinuxParser::Jiffies());
    }

    std::sort(processes_.begin(), processes_.end(), std::greater<Process>());

    return processes_; 
}

std::string System::Kernel() {
    return LinuxParser::Kernel();
}

float System::MemoryUtilization() { 
    return LinuxParser::MemoryUtilization();
 }

std::string System::OperatingSystem() {
    return LinuxParser::OperatingSystem();
}

int System::RunningProcesses() {
    return LinuxParser::RunningProcesses();   
}

int System::TotalProcesses() {
    return LinuxParser::TotalProcesses();
}

long int System::UpTime() {
    return LinuxParser::UpTime();
}