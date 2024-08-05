#include <cinttypes>
#include <cstring>

struct RunningArguments {

    uint64_t iteration_count {};
    int64_t max_iteration {};
    int64_t frequency_of_save {};
    uint64_t save_count {};
    char* output_path {};

};

struct Sandpile {

    uint16_t width;
    uint16_t height;
    uint64_t** cells;

    ~Sandpile();
};

struct Direction {

    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;

};

void SavePicture(Sandpile& sandpile, uint64_t save_count, char* output_path);

Sandpile GetStartValue(char* file_name);

bool SandpileTopple(Sandpile& sandpile);

void SandpileIterations(Sandpile& sandpile, RunningArguments& running_arguments);