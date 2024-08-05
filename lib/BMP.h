#include "sandpile.h"

#include <iostream>
#include <fstream>
#include <cinttypes>

struct Image{
    uint16_t m_width;
    uint16_t m_height;

    Image(int width, int height);

    void Export(char* path, Sandpile& sandpile);
};