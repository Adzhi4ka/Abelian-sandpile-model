#include "BMP.h"

Image::Image(int width, int height) {
    this -> m_width = width;
    this -> m_height = height;
}

void Image::Export(char* path, Sandpile& sandpile) {
    std::ofstream fout;
    fout.open(path, std::ios::out | std::ios::binary);

    if(!fout.is_open()) {
        std::cerr << "ERROR\n. Uncorrect output path\n";
        std::exit(EXIT_FAILURE);
    }

    unsigned char bmpPad[1] = {0};
    const int kPaddingAmount = (4 - ((m_width + 1) / 2) % 4) % 4;

    const int kFileHeaderSize = 14;
    const int kInformationHeaderSize = 40;
    const int kPalleteSize = 16 * 4;
    const int kFileSize = kFileHeaderSize + kInformationHeaderSize + kPalleteSize + (m_width + 1) * m_height / 2 + kPaddingAmount * m_height;
    const int kSizeBeforePixels = kFileHeaderSize + kInformationHeaderSize + kPalleteSize;

    unsigned char fileHeader[kFileHeaderSize];

    fileHeader[0] = 'B';
    fileHeader[1] = 'M';

    fileHeader[2] = kFileSize;
    fileHeader[3] = kFileSize >> 8;
    fileHeader[4] = kFileSize >> 16;
    fileHeader[5] = kFileSize >> 24;

    fileHeader[6] = 0;
    fileHeader[7] = 0;
    fileHeader[8] = 0;
    fileHeader[9] = 0;
    
    fileHeader[10] = kSizeBeforePixels;
    fileHeader[11] = kSizeBeforePixels >> 8;
    fileHeader[12] = kSizeBeforePixels >> 16;
    fileHeader[13] = kSizeBeforePixels >> 24;

    unsigned char informationHeader[kInformationHeaderSize];

    informationHeader[0] = kInformationHeaderSize;

    informationHeader[1] = 0;
    informationHeader[2] = 0;
    informationHeader[3] = 0;

    informationHeader[4] = m_width;
    informationHeader[5] = m_width >> 8;
    informationHeader[6] = m_width >> 16;
    informationHeader[7] = m_width >> 24;

    informationHeader[8] = m_height;
    informationHeader[9] = m_height >> 8;
    informationHeader[10] = m_height >> 16;
    informationHeader[11] = m_height >> 24;

    informationHeader[12] = 1;
    informationHeader[13] = 0;

    informationHeader[14] = 4;
    informationHeader[15] = 0;
    
    //НЕ ИСПОЛЬЗУЕМЫЕ КРОМЕ ПАЛИТРЫ
    for (int i = 16; i < 40; i++) {
        informationHeader[i] = 0;
    }

    informationHeader[32] = kPalleteSize / 4;

    fout.write(reinterpret_cast<char*>(fileHeader), kFileHeaderSize);
    fout.write(reinterpret_cast<char*>(informationHeader), kInformationHeaderSize);
    
    unsigned char white[4] = {255, 255, 255, 0};
    fout.write(reinterpret_cast<char*>(white), 4);

    unsigned char green[4]  = {0, 255, 0, 0};
    fout.write(reinterpret_cast<char*>(green), 4);

    unsigned char yellow[4] = {0, 255, 255, 0};
    fout.write(reinterpret_cast<char*>(yellow), 4);

    unsigned char purple[4] = {255, 0, 255, 0};
    fout.write(reinterpret_cast<char*>(purple), 4);

    unsigned char black[4] = {0, 0, 0, 0};
    for (int i = 0; i < 12; i++) {
        fout.write(reinterpret_cast<char*>(black), 4);
    }

    for (int y = 0; y < sandpile.height; y++) {
        for (int x = 0; x < sandpile.width + sandpile.width % 2; x += 2) {
            unsigned char color_type1 = sandpile.cells[y][x] > 3 ? 4 : sandpile.cells[y][x];
            unsigned char color_type2 = sandpile.cells[y][x + 1] > 3 ? 4 : sandpile.cells[y][x + 1];
            if (x + 1 >= sandpile.width - 1 + sandpile.width % 2) {
                color_type2 = 0;
            }

            unsigned char for_write[1];
            for_write[0] = color_type1 * 16 + color_type2;

            fout.write(reinterpret_cast<char*>(for_write), 1);
        }
        fout.write(reinterpret_cast<char*>(bmpPad), kPaddingAmount);
    }

    fout.close();
}