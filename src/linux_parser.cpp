#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() { 
  string line, key;
  float total = 1, free = 1;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
        if (key == "MemTotal:") {
          linestream >> total;
        } else if (key == "MemAvailable:") {
          linestream >> free;
          break;
        }
      }
    }
  return (total - free)/total; 
}

long LinuxParser::UpTime() {
  string line, uptime;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return std::stol(uptime);
}

long LinuxParser::Jiffies() {
  vector<string> raw_cpu_util = LinuxParser::CpuUtilization();
  long total;
  for (string jif : raw_cpu_util){
    total += std::stol(jif);
  }
  return total;
}

long LinuxParser::ActiveJiffies(int pid) {
  string line, value;
  long total;
  int i;  
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()){
    total = 0;
    i = 0;
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while(linestream >> value){
      if ((i > 12) && (i < 17)) {
        total += stol(value);
      }
      if (i == 16) {
        break;
      }
      i++;
    }
  }
  return total;
}

long LinuxParser::ActiveJiffies() {
  return LinuxParser::Jiffies() - LinuxParser::IdleJiffies();
}

long LinuxParser::IdleJiffies() {
  vector<string> raw_cpu_util = LinuxParser::CpuUtilization();
  long output = stol(raw_cpu_util[3]) + stol(raw_cpu_util[4]);
  return output;
}

vector<string> LinuxParser::CpuUtilization() {
  string line, key, value;
  vector<string> output;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while(linestream >> key){
        if (key == "cpu") {
          while(linestream >> value) {
            output.push_back(value);
          }
          break;
        }
      }
    }
  }
  return output; 
}

int LinuxParser::TotalProcesses() {
  string line, key;
  int total_processes = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
        if (key == "processes") {
          linestream >> total_processes;
          break;
        }
      }
    }
  return total_processes; 
}

int LinuxParser::RunningProcesses() {
  string line, key;
  int running_processes = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
        if (key == "procs_running") {
          linestream >> running_processes;
          break;
        }
      }
    }
  return running_processes; 
}

string LinuxParser::Command(int pid) {
  string line = "";
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
  }
  return line; 
}

string LinuxParser::Ram(int pid) {
  string token;
  int value = 0;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (filestream >> token){
      if (token == "VmSize:"){
        if (filestream >> token) {
          value = std::stoi(token)/1024;
          return to_string(value);
        }
      }
    }
  }
  return to_string(value);
}

string LinuxParser::Uid(int pid) {
  string token, value="";
  std::ifstream filestream(kProcDirectory+to_string(pid)+kStatusFilename);
  if (filestream.is_open()) {
    while (filestream >> token){
      if (token == "Uid:"){
        if (filestream >> token) {
          return token;
        }
      }
    }
  }
  return value;
}

string LinuxParser::User(int pid) {
  string line, token, user;
  string uid = Uid(pid);
  int i;

  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      i = 0;
      std::istringstream linestream(line);
      while(std::getline(linestream, token, ':')) {
        if (i == 0) {
          user = token;
        } else if (token == uid) {
          return user;
        } else if (i == 2) {
            break;
        }
        i++;
      }
    }
  }
  return "None";
}

long LinuxParser::UpTime(int pid) {
  string line, token;
  int i;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)){
      i = 0;
      std::istringstream linestream(line);
      while(linestream >> token){
        if (i == 21) {
          return (stol(token)/ sysconf(_SC_CLK_TCK));
        }
        i++;
      }
    }
  }
  return 0;
}