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

int main() {
    string filename = "COE499.bmp";
    vector<unsigned char> pixels;
    BMPHeader header;

    if (readBMP(filename, pixels, header)) {
        cout << "Image loaded successfully!" << endl;
        cout << "Width: " << header.biWidth << endl;
        cout << "Height: " << header.biHeight << endl;

        // Now you have the pixels in the 'pixels' vector
        // You can process or manipulate the pixels as needed
    } else {
        cerr << "Error loading image." << endl;
    }

    return 0;
}
