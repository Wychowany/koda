//
// Created by Aleksander Nuszel on 15/12/2020.
//

#include <fstream>
#include "ImageReader.h"

std::pair<char *, int> ImageReader::readImg(char *path) {
    std::fstream image;
    image.open(path);

    image.seekg(0, std::ios::end);
    int n = image.tellg();
    image.seekg(0, std::ios::beg);

    char *res = new char[n];
    image.read(res, n);
    return std::make_pair(res, n);
}

void ImageReader::write(char *bytes, long size) {
    std::ofstream image;
    image.open("a.png");
    image.write(bytes, size);
    image.close();
}
