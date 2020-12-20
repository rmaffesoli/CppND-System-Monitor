#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {}

int Process::Pid() const {return pid_;}

float Process::CpuUtilization() const { 
    return cpu_;
}

void Process::CpuUtilization(long active_jifs, long system_jifs) {
    long active_duration{active_jifs - prev_active_jifs_};
    long system_duration{system_jifs - prev_system_jifs_};
    cpu_ = static_cast<float> (active_duration)/ system_duration;
    prev_active_jifs_ = active_jifs;
    prev_system_jifs_= system_jifs;
}

string Process::Command() const { 
    return LinuxParser::Command(Pid());
}

string Process::Ram() const { 
    return LinuxParser::Ram(Pid());
}

string Process::User() const { 
    return LinuxParser::User(Pid());
}

long int Process::UpTime() const {
    return LinuxParser::UpTime(Pid());
}

bool Process::operator<(const Process& a) const {
  return CpuUtilization() < a.CpuUtilization();
}

bool Process::operator>(const Process& a) const {
  return CpuUtilization() > a.CpuUtilization();
}