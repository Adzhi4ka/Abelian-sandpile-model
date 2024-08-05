#include "parser.h"

const char kForCheckIsStringValueVoid[Parameters::kDirectoryMaxLenght] = "";
const int64_t kForCheckIsNumberValueVoid = INT64_MIN;
const char kNameOfInputArg[9] = "--input=";
const int kSizeInputArg = 8;
const char kNameOfOutputArg[10] = "--output=";
const int kSizeOutputArg = 9;
const char kNameOfIterArg[12] = "--max-iter=";
const int kSizeIterArg = 11;
const char kNameOfFreqArg[8] = "--freq=";
const int kSizeFreqArg = 7;

void ErrorOutput(ErrorType err) {
    switch (err)
    {
        case ErrorType::kUncorrectArgument:
            std::cerr << "UNCORRECT INPUT.\nUnknown argument.\n";
            break;
        case ErrorType::kUncorrectValue: 
            std::cerr << "UNCORRECT INPUT.\n-m and --max-iter=, -f and --freq  argument is number.\n"; 
            break;
        case ErrorType::kRepeatParameters: 
            std::cerr << "UNCORRECT INPUT.\nThe value of parametres is set several times\n"; 
            break;
    }
}

void Number(const char* argument, const int length_argument_type, std::int64_t& value) {
    if (value == kForCheckIsNumberValueVoid) {
        const char* first = argument + length_argument_type;
        const char* last = argument + std::strlen(argument);
        std::from_chars_result res = std::from_chars(first, last, value);

        if (res.ec == std::errc{} && res.ptr == last && value >= -1) {
            return;
        }

        ErrorOutput(ErrorType::kUncorrectValue);
        std::exit(EXIT_FAILURE);
    }

    ErrorOutput(ErrorType::kRepeatParameters);
    exit(EXIT_FAILURE); 
}


void String(char* argument, const int length_argument_type, char* value) {
    if (!std::strcmp(value, kForCheckIsStringValueVoid)) {
        int argument_length = std::strlen(argument);
        for (int i = 0; i < (argument_length - length_argument_type); i++) {
            value[i] = argument[i + length_argument_type];
        }
        return;
    }

    ErrorOutput(ErrorType::kRepeatParameters);
    std::exit(EXIT_FAILURE);
}

Parameters Parser(int argc, char* argv[]) {
    Parameters output_parameters;

    for (int i = 1; i < argc; ++i) {
        size_t argv_lenght = std::strlen(argv[i]);

        if (argv[i][0] == '-'  && argv_lenght == 2) {

            if (i + 1 == argc) {
                ErrorOutput(ErrorType::kUncorrectValue);
                std::exit(EXIT_FAILURE);
            }

            switch(argv[i][1])
                {
                case 'm':
                    Number(argv[++i], 0, output_parameters.iter); 
                    continue;
                case 'f':
                    Number(argv[++i], 0,  output_parameters.freq); 
                    continue;
                case 'o':
                    String(argv[++i], 0, output_parameters.output_directory); 
                    continue;
                case 'i':
                    String(argv[++i], 0, output_parameters.file_with_start_value_name);
                    continue;
                }

        } else if (argv[i][1] == '-') {
            if (!strncmp(argv[i], kNameOfInputArg, kSizeInputArg)) {
                String(argv[i], kSizeInputArg, output_parameters.file_with_start_value_name);
            } else if (!strncmp(argv[i], kNameOfOutputArg, kSizeOutputArg)) {
                String(argv[i], kSizeOutputArg, output_parameters.output_directory);
            } else if (!strncmp(argv[i], kNameOfIterArg, kSizeIterArg)) {
                Number(argv[i], kSizeIterArg, output_parameters.iter);
            } else if (!strncmp(argv[i], kNameOfFreqArg, kSizeFreqArg)) {
                Number(argv[i], kSizeFreqArg, output_parameters.freq);
            }
            continue;
        }

        ErrorOutput(ErrorType::kUncorrectArgument);
        std::exit(EXIT_FAILURE);
    }

    return output_parameters;
}
