#define CUTF_IMPLEMENTATION 1
#include "cutf.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t* readfile(const char* path, size_t* size)
{
    FILE* file = fopen(path, "rb");
    uint8_t* result = 0;
    if(file) {
        size_t file_size = 0;
        fseek(file, 0L, SEEK_END);
        file_size = (size_t)ftell(file);
        fseek(file, 0L, SEEK_SET);
        result = (uint8_t*)calloc(file_size, sizeof(uint8_t));
        *size = fread(result, sizeof(uint8_t), file_size, file);
        fclose(file);
    }
    return result;
}

int check(const char* name, uint8_t* original, uint8_t* copy, size_t size)
{
    bool bIsOk = memcmp(original, copy, size) == 0;

    if(!bIsOk)
        printf("\n  %s: %s\n", name, "fail");

    return (bIsOk) ? 0 : 1;
}

int roundtrip_16(uint8_t* original, uint8_t* copy, size_t size, size_t count)
{
    size_t written;
    uint16_t* temp = (uint16_t*)calloc(count*2, sizeof(uint16_t));
    written = cutf_8to16(original, original+size, temp);
    written = cutf_16to8(temp, temp+written, copy, size * 2);
    free(temp);
    return check("roundtrip_16", original, copy, size);
}

int roundtrip_32(uint8_t* original, uint8_t* copy, size_t size, size_t count)
{
    size_t written;
    uint32_t* temp = (uint32_t*)calloc(count, sizeof(uint32_t));
    written = cutf_8to32(original, original+size, temp);
    written = cutf_32to8(temp, temp+written, copy, size * 2);
    free(temp);
    return check("roundtrip_32", original, copy, size);
}

int testfile(const char* path, bool expectInvalidChars = false)
{
    size_t size;
    uint8_t* original = readfile(path, &size);
    printf("%-16.16s: ", path);
    if(original) {
        int r = 0;
        uint8_t* copy = (uint8_t*)calloc(size*2, sizeof(uint8_t));
        size_t count;

        if(!cutf_is_valid(original, original+size)) {
            cutf_default_replace_invalid(original, original+size, copy, size);
            memcpy(original, copy, size);
            if (!expectInvalidChars)
            {
                printf("  file contains invalid utf-8 characters\n");
                r |= 1;
            }
        }

        count = cutf_distance(original, original+size);
        if(count) {
            r |= roundtrip_16(original, copy, size, count);
            printf(".");
            
            r |= roundtrip_32(original, copy, size, count);
            printf(".");
        }

        free(copy);
        free(original);
        if(!r)
            printf(" ok.");

        printf("\n");
        return r;
    } else {
        printf("  file read failed");
        return 1;
    }
}

int simpleStringTest();

int main(int argc, char** argv)
{
    int r = 0;
    #ifdef _WIN32
        printf("[windows] sizeof(wchar_t)=%zd\n", sizeof(wchar_t));
    #else
        printf("[linux] sizeof(wchar_t)=%zd\n", sizeof(wchar_t));
    #endif

    r |= simpleStringTest();
    r |= testfile("utf8_invalid.txt", true);
    r |= testfile("quickbrown.txt");
    r |= testfile("UTF-8-demo.txt");
    r |= testfile("big.txt");

    return r;
}
