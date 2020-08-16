#ifndef ERROR_HPP
#define ERROR_HPP

#include <cstdio>

inline bool error(const char* file_name, const char* message)
{
    printf("%s: %s\n", file_name, message);
    return false;
}

inline bool error(const char* file_name, unsigned int line_number, const char* message)
{
    printf("%s:%u: %s\n", file_name, line_number, message);
    return false;
}

#endif // ERROR_HPP