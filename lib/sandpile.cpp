#include "BMP.h"
#include "parser.h"

void SavePicture(Sandpile& sandpile, uint64_t save_count, char* output_path) {
    Image image{sandpile.width, sandpile.height};
    uint64_t byte_count = 0;

    const uint8_t kSandpileLenght = 8;
    const uint8_t kPathLenght = std::strlen(output_path);
    
    char save_count_string[128];
    char name[9] = "sandpile";
    uint16_t save_count_string_length = kPathLenght + kSandpileLenght;
    uint64_t buff_save_count = save_count;

    std::strcpy(save_count_string, output_path);

    for (int i = kPathLenght; i < kPathLenght + kSandpileLenght; i++) {
        save_count_string[i] = name[i - kPathLenght];
    }

    while (save_count > 0) {
        save_count_string_length++;
        save_count /= 10;
    }
    for (int i = save_count_string_length - 1; i >= kPathLenght + kSandpileLenght; i--) {
        save_count_string[i] = buff_save_count % 10 + '0';
        buff_save_count /= 10;
    }

    save_count_string[save_count_string_length++] = '.';
    save_count_string[save_count_string_length++] = 'b';
    save_count_string[save_count_string_length++] = 'm';
    save_count_string[save_count_string_length++] = 'p';
    save_count_string[save_count_string_length++] = '\0';

    image.Export(save_count_string, sandpile);
}

Sandpile GetStartValue(char* file_name) {

    std::ifstream fin(file_name);

    if (!fin.is_open()) {
        std::cerr << "ERROR.\nInvalid file path";
        std::exit(EXIT_FAILURE);
    }

    int16_t x;
    int16_t y;
    uint64_t cnt_of_grains;
    int16_t min_x = INT16_MAX;
    int16_t min_y = INT16_MAX;
    int16_t max_x = INT16_MIN;
    int16_t max_y = INT16_MIN;

    while (!fin.eof())  { 
        fin >> x;
        fin >> y;
        fin >> cnt_of_grains;
        max_x = max_x > x ? max_x :  x;
        min_x = min_x < x ? min_x : x;
        max_y = max_y > y ? max_y :  y;
        min_y = min_y < y ? min_y : y;
    }

    uint16_t width = abs(max_x - min_x) + 1;
    uint16_t height = abs(max_y - min_y) + 1 ;

    uint64_t** cells = new uint64_t* [height];
    for (uint16_t i = 0; i < height; ++i) {
        cells[i] = new uint64_t [width] {0};
    }

    fin.clear();
    fin.seekg(0, std::ios_base::beg);

    while (!fin.eof()) {
        fin >> x;
        fin >> y;
        fin >> cnt_of_grains;
        cells[y - min_y][x - min_x] = cnt_of_grains;
    }

    Sandpile sandpile{width, height, cells};

    return sandpile;
}

bool SandpileTopple(Sandpile& sandpile) {
    Direction direction;
    bool was_expands = false;
    if (sandpile.width < INT16_MAX) {
        for (int i = 0; (i < sandpile.width) && !(direction.up || direction.down); i++) {
            if (sandpile.cells[0][i] >= 4) {
                direction.up = true;
            }
            if (sandpile.cells[sandpile.height - 1][i] >= 4) {
                direction.down = true;
            }
        }
    }

    if (sandpile.height < INT16_MAX) {
        for (int i = 0; i < sandpile.height && !(direction.left || direction.right); i++) {
            if (sandpile.cells[i][0] >= 4) {
                direction.left = true;
            }
            if (sandpile.cells[i][sandpile.width - 1] >= 4) {
                direction.right = true;
            }
        }
    }

    uint16_t new_height = sandpile.height + direction.up + direction.down;
    uint16_t new_width = sandpile.width + direction.left + direction.right;
    uint64_t** new_cells = new uint64_t* [new_height];

    new_cells[0] = new uint64_t [new_width] {0};
    new_cells[1] = new uint64_t [new_width] {0};

    for (int i = 0; i < sandpile.height; i++) {
        if (i + 2 < new_height) {
            new_cells[i + 2] = new uint64_t[new_width] {0};
        }
        for (int j = 0; j < sandpile.width; j++) {
            if (sandpile.cells[i][j] >= 4) {
                new_cells[i + direction.up][j + direction.left] += sandpile.cells[i][j] - 4;
                new_cells[i + direction.up - 1][j + direction.left] += 1;
                new_cells[i + direction.up + 1][j + direction.left] += 1;
                new_cells[i + direction.up][j + direction.left - 1] += 1;
                new_cells[i + direction.up][j + direction.left + 1] += 1;
                was_expands = true;
            } else {
                new_cells[i + direction.up][j + direction.left] += sandpile.cells[i][j];
            }
        }
        delete[] sandpile.cells[i];
    }

    delete[] sandpile.cells;

    sandpile.cells = new_cells;
    sandpile.width = new_width;
    sandpile.height = new_height;

    return was_expands;
}

void SandpileIterations(Sandpile& sandpile, RunningArguments& running_arguments) {

    while (running_arguments.iteration_count < running_arguments.max_iteration) {
        
        bool was_expands;
        was_expands = SandpileTopple(sandpile);

        ++running_arguments.iteration_count;

        if (was_expands) {
            if (running_arguments.iteration_count % running_arguments.frequency_of_save == 0) {
                SavePicture(sandpile, running_arguments.save_count++, running_arguments.output_path);
            }
        } else {
            SavePicture(sandpile, running_arguments.save_count++, running_arguments.output_path);
            return;
        }
    }
}

Sandpile::~Sandpile() {
    for (int i = 0; i < this -> height; i++) {
        delete[] this -> cells[i];
    }
    delete[] cells;
}