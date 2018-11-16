#include <iostream>
#include <cstdio>

void FormatLog(const char *fmt)
{
    std::clog << fmt;
}

template<typename First, typename ...Rest>
void FormatLog(const char *fmt, First car, Rest... cdr)
{
    for (; *fmt != '\0'; ++fmt)
    {
        if (*fmt == '%')
        {
            if (*++fmt == '%')
            {
                std::clog << '%';
            }
            else
            {
                break;
            }
        }
        else
        {
            std::clog << *fmt;
        }
    }
    std::clog << car;
    if (*fmt != '\0')
    {
        FormatLog(fmt, cdr...);
    }
}

void VarLog_Help(void)
{
    std::clog << std::endl;
}

template<typename First, typename ...Rest>
void VarLog_Help(First car, Rest... cdr)
{
    std::clog << car << " ";
    VarLog_Help(cdr...);
}

#define VarLog(...) VarLog_Help(#__VA_ARGS__ ":", __VA_ARGS__);

