#include <iostream>
#include "BAC.h"
#include "ImageReader.h"

using namespace std;

int main() {

    pair<char *, int> imageInfo = ImageReader::readImg("sample.png");
    char *imagePointer = imageInfo.first;
    long numberOfBytes = imageInfo.second;
    pair<int, int> statistics = BAC::calculate_statistics(imagePointer, numberOfBytes);
    int number_of_zeros = statistics.first;
    int number_of_ones = statistics.second;
    pair<char *, int> encoded = BAC::encode(imagePointer, number_of_zeros, number_of_ones);
    pair<char *, int> decoded = BAC::decode(encoded.first, encoded.second, number_of_zeros, number_of_ones,numberOfBytes);
    ImageReader::write(decoded.first, decoded.second/8);
}

