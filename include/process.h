#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid() const;
  std::string User() const;                      // TODO: See src/process.cpp
  std::string Command() const;                   // TODO: See src/process.cpp
  float CpuUtilization() const;                  // TODO: See src/process.cpp
  void CpuUtilization(long active_jifs, long system_jifs);
  std::string Ram() const;                       // TODO: See src/process.cpp
  long UpTime() const;                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  bool operator>(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
 int pid_;
 float cpu_{0};
 long prev_active_jifs_{0};
 long prev_system_jifs_{0};
};

#endif