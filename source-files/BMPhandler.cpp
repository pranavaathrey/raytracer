#include "BMP.hpp"
#include <fstream>
#include <filesystem> 

#pragma pack(push, 1)
struct BMPFileHeader {
    uint16_t fileType{0x4D42}; // 'BM'
    uint32_t fileSize{0};
    uint16_t reserved1{0};
    uint16_t reserved2{0};
    uint32_t offsetData{54};   // File header + info header
};

struct BMPInfoHeader {
    uint32_t size{40};
    int32_t width;
    int32_t height;
    uint16_t planes{1};
    uint16_t bitCount{24};     // RGB (no alpha)
    uint32_t compression{0};
    uint32_t sizeImage{0};     // Can be 0 for BI_RGB
    int32_t xPixelsPerMeter{0};
    int32_t yPixelsPerMeter{0};
    uint32_t colorsUsed{0};
    uint32_t colorsImportant{0};
};
#pragma pack(pop)

void saveAsBMP(const std::vector<Colour>& pixels, int width, int height, const std::string& filename) {
    // writes to "renders" directory in folder
    std::filesystem::create_directories("renders");
    std::string fullPath = "renders/" + filename;

    // metadata definitions
    const int rowStride = width * 3;
    const int paddingAmount = (4 - (rowStride % 4)) % 4;
    const int paddedRowSize = rowStride + paddingAmount;
    const int pixelDataSize = paddedRowSize * height;

    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    fileHeader.fileSize = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + pixelDataSize;
    infoHeader.width = width;
    infoHeader.height = height;
    infoHeader.sizeImage = pixelDataSize;

    // writing to output file
    std::ofstream out(fullPath, std::ios::binary);
    if (!out) return;

    out.write(reinterpret_cast<const char*>(&fileHeader), sizeof(fileHeader));
    out.write(reinterpret_cast<const char*>(&infoHeader), sizeof(infoHeader));

    std::vector<uint8_t> padding(paddingAmount, 0);

    // writing colour data to bmp file (in reverse becuz spec)
    for (int y = height - 1; y >= 0; --y) { 
        for (int x = 0; x < width; ++x) {
            const Colour& c = pixels[y * width + x];
            uint8_t bgr[3] = { c.blue, c.green, c.red }; // alpha channel unsupported
            out.write(reinterpret_cast<const char*>(bgr), 3);
        }
        out.write(reinterpret_cast<const char*>(padding.data()), padding.size());
    }
    out.close();
}


std::vector<Colour> loadFromBMP(const std::string& filename, int width, int height) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        throw std::runtime_error("Failed to open BMP file.");
    }

    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    // read headers
    in.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    in.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

    // validate BMP format (basic checks)
    if (fileHeader.fileType != 0x4D42) { // 'BM' in little-endian
        throw std::runtime_error("Not a valid BMP file.");
    }

    width = infoHeader.width;
    height = infoHeader.height;
    const int rowStride = width * 3;
    const int paddingAmount = (4 - (rowStride % 4)) % 4;
    const int paddedRowSize = rowStride + paddingAmount;

    std::vector<Colour> pixels(width * height);

    // seek to pixel array offset
    in.seekg(fileHeader.offsetData, std::ios::beg);

    for (int y = height - 1; y >= 0; --y) {
        for (int x = 0; x < width; ++x) {
            uint8_t bgr[3];
            in.read(reinterpret_cast<char*>(bgr), 3);
            Colour& c = pixels[y * width + x];
            c.blue = bgr[0];
            c.green = bgr[1];
            c.red = bgr[2];
        }
        // skip row padding
        in.ignore(paddingAmount);
    }

    in.close();
    return pixels;
}
