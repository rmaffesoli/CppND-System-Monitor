#include <string>
#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) {
    string hour_s, minute_s, second_s;
    hour_s = std::to_string(seconds / 3600);
    hour_s.insert(hour_s.begin(), 2-hour_s.size(), '0');
    seconds = seconds % 3600;
    minute_s = std::to_string(seconds / 60);
    minute_s.insert(minute_s.begin(), 2-minute_s.size(), '0');
    second_s = std::to_string(seconds % 60);
    string final = hour_s + ":" + minute_s + ":" + second_s;
    return final;
};