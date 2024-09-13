#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct BMPHeader {
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;
    unsigned int biSize;
    int biWidth;
    int biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
};

bool readBMP(const string& filename, vector<unsigned char>& pixels, BMPHeader& header) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return false;
    }

    file.read(reinterpret_cast<char*>(&header), sizeof(BMPHeader));

    if (header.bfType != 0x4D42) {
        cerr << "Error: Not a BMP file." << endl;
        return false;
    }

    pixels.resize(header.biWidth * header.biHeight * 3);
    file.seekg(header.bfOffBits, ios::beg);
    file.read(reinterpret_cast<char*>(pixels.data()), pixels.size());

    return true;
}

void dumpAsRaw(const vector<unsigned char>& pixels, const BMPHeader& header) {
    string rawFilename = "raw_image.bin";
    ofstream rawFile(rawFilename, ios::binary);
    if (!rawFile.is_open()) {
        cerr << "Error creating raw file: " << rawFilename << endl;
        return;
    }

    rawFile.write(reinterpret_cast<const char*>(pixels.data()), pixels.size());
    cout << "Raw image dumped to: " << rawFilename << endl;
}

int main() {
    string filename = "your_image.bmp";
    vector<unsigned char> pixels;
    BMPHeader header;

    if (readBMP(filename, pixels, header)) {
        cout << "Image loaded successfully!" << endl;
        cout << "Width: " << header.biWidth << endl;
        cout << "Height: " << header.biHeight << endl;
        cout << "Bits per pixel: " << header.biBitCount << endl;
        // ... other header information as needed ...
        dumpAsRaw(pixels, header);
    } else {
        cerr << "Error loading image." << endl;
    }

    return 0;
}
