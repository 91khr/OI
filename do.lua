-- Lua file to begin doing a problem
require('io')
template = [[
#include <cstdio>
#ifdef DEBUG
#include "../debug.hpp"
#else
template<typename First, typename ...Rest>
void VarLog(First, Rest...) {}
template<typename First, typename ...Rest>
void FormatLog(First, Rest...) {}
#endif

int main(void)
{
    return 0;
}

]]

f = io.open(arg[1], "w")
f:write(template)
f:close()

