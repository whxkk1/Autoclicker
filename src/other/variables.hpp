#pragma once
#include <vector>

struct Input {
    size_t ticks[2];
    size_t repeatTimes;
    bool p1;
};

namespace Autoclicker {
    extern std::vector<Input> g_inputs[2];
    extern bool g_autoclickerEnabled;
}
