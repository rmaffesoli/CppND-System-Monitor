#include "processor.h"
#include "linux_parser.h"
#include <unistd.h>

float Processor::Utilization() {
    
    long total_jiffies_start = LinuxParser::Jiffies();
    long active_jiffies_start = LinuxParser::ActiveJiffies();
    
    usleep(100000);

    long total_jiffies_end = LinuxParser::Jiffies();
    long active_jiffies_end = LinuxParser::ActiveJiffies();

    float total_diff = float(total_jiffies_end - total_jiffies_start);
    float active_diff = float(active_jiffies_end - active_jiffies_start);
    return active_diff/total_diff;
}