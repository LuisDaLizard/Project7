#include "utils.h"
#include <cstdio>
#include <cstdlib>

void Utils::Info(const char *message)
{
    printf("%s\n", message);
}

void Utils::Warning(const char *message)
{
    printf("WARNING: %s\n", message);
}

void Utils::Error(int code, const char *message)
{
    printf("ERROR: %s\n", message);
    exit(code);
}

char *Utils::ReadFile(const char *fileName)
{
    FILE *file = fopen(fileName, "rb");

    if (!file)
        return nullptr;

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *contents = new char[size + 1];

    fread(contents, 1, size, file);
    contents[size] = 0;

    fclose(file);

    return contents;
}