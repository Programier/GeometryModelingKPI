#include <cstdarg>
#include <cstdio>

void info_log(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    fprintf(stdout, "[ INFO] ");
    vfprintf(stdout, format, args);
    va_end(args);
}

void error_log(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    fprintf(stdout, "[ERROR] ");
    vfprintf(stderr, format, args);
    va_end(args);
}
