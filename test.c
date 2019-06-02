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
		file_size = ftell(file);
		fseek(file, 0L, SEEK_SET);
		result = (uint8_t*)calloc(file_size, sizeof(uint8_t));
		*size = fread(result, sizeof(uint8_t), file_size, file);
		fclose(file);
	}
	return result;
}

void check(const char* name, uint8_t* original, uint8_t* copy, size_t size)
{
	printf("  %s: %s\n", name, memcmp(original, copy, size) == 0 ? "pass" : "fail");
}

void roundtrip_16(uint8_t* original, uint8_t* copy, size_t size, size_t count)
{
	size_t written;
	uint16_t* temp = calloc(count*2, sizeof(uint16_t));
	written = cutf_8to16(original, original+size, temp);
	written = cutf_16to8(temp, temp+written, copy);
	free(temp);
	check("roundtrip_16", original, copy, size);
}

void roundtrip_32(uint8_t* original, uint8_t* copy, size_t size, size_t count)
{
	size_t written;
	uint32_t* temp = calloc(count, sizeof(uint32_t));
	written = cutf_8to32(original, original+size, temp);
	written = cutf_32to8(temp, temp+written, copy);
	free(temp);
	check("roundtrip_32", original, copy, size);
}

void testfile(const char* path)
{
	size_t size;
	uint8_t* original = readfile(path, &size);
	printf("%s:\n", path);
	if(original) {
		uint8_t* copy = calloc(size*2, sizeof(uint8_t));
		size_t count;

		if(!cutf_is_valid(original, original+size)) {
			cutf_default_replace_invalid(original, original+size, copy, size);
			memcpy(original, copy, size);
			printf("  file contains invalid utf-8 characters\n");
		}

		count = cutf_distance(original, original+size);
		printf("  # of cp: %d\n", count);
		if(count) {
			roundtrip_16(original, copy, size, count);
			roundtrip_32(original, copy, size, count);
		}

		free(copy);
		free(original);
	} else {
		printf("  file read failed");
	}
}

int main(int argc, char** argv)
{
	testfile("utf8_invalid.txt");
	testfile("quickbrown.txt");
	testfile("UTF-8-demo.txt");
	testfile("big.txt");
	return 0;
}
