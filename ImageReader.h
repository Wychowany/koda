//
// Created by Aleksander Nuszel on 15/12/2020.
//

#ifndef KODA_IMAGEREADER_H
#define KODA_IMAGEREADER_H

using namespace std;

class ImageReader {
public:
    static std::pair<char*, int> readImg(char *path);
    static void write(char* bytes, long size);
};


#endif //KODA_IMAGEREADER_H
