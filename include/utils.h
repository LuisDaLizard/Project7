#ifndef LOG_H
#define LOG_H

#include <string>

#define MAX(a, b) ((a > b) ? a : b)
#define MIN(a, b) ((a < b) ? a : b)
#define CLAMP(min, a, max) (MAX(min, MIN(max, a)))
#define DEG2RAD (M_PI / 180.0f)
#define DEG2RADF ((float)(M_PI / 180.0f))

namespace Utils
{
    void Info(const char *message);
    void Warning(const char *message);
    void Error(int code, const char *message);
    char *ReadFile(const char *fileName);
}

#endif  // LOG_H