#include "lib/sandpile.h"
#include "lib/parser.h"

int main(int argc, char* argv[]) {
    Parameters output_parameters;
    uint64_t iteration_count = 0;
    uint64_t save_count = 1;

    output_parameters = Parser(argc, argv);
    
    Sandpile sandpile = GetStartValue(output_parameters.file_with_start_value_name);

    RunningArguments running_arguments{iteration_count, output_parameters.iter, output_parameters.freq, save_count, output_parameters.output_directory};

    SandpileIterations(sandpile, running_arguments);

}