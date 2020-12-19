//
// Created by Aleksander Nuszel on 2020-12-13.
//
#include <iostream>

#ifndef KODA_CODER_H
#define KODA_CODER_H

class BAC {
public:
    static std::pair<char*, int> encode(const char *bytesToEncode, int n_zero, int n_one);
    static std::pair<int, int> calculate_statistics(const char *data,long numberOfBytes);
    static std::pair<char *, int>decode(const char *bytes_to_decode, int number_of_bits, int n_zero, int n_one, long originalNumberOfBytes);

private:
    static unsigned short int bottom;
    static unsigned short int top;
    static long encodedIndex;;
    static long insufficiency;
    static void initialize();
    static bool getBit(unsigned short r, int position);
};


#endif //KODA_CODER_H
