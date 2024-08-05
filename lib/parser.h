#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdint>
#include <charconv>

enum class ErrorType {
    kUncorrectArgument,
    kUncorrectValue,
    kRepeatParameters,
};

struct Parameters {
    static const size_t kDirectoryMaxLenght = 256;
    int64_t iter = INT64_MIN;
    int64_t freq = INT64_MIN;
    char output_directory[kDirectoryMaxLenght] = "";
    char file_with_start_value_name[kDirectoryMaxLenght] = "";
};

void ErrorOutput(ErrorType err);

void Number(const char* argument, const int length_argument_type, std::int64_t& value);

void String(char* argument, const int length_argument_type, char* value);

Parameters Parser(int argc, char* argv[]);