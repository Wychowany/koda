//
// Created by Aleksander Nuszel on 2020-12-13.
//
#include <iostream>

#ifndef KODA_CODER_H
#define KODA_CODER_H

class BAC {
public:
    static std::pair<char *, int> encode(const char *bytesToEncode, unsigned int n_zero, unsigned int n_one);

    static std::pair<int, int> calculate_statistics(const char *data, long numberOfBytes);

    static std::pair<char *, int> decode(char *bytes_to_decode, int number_of_bits, int n_zero, int n_one);

private:
    static unsigned short int bottom;
    static long numberOfEncodedBits;
    static long insufficiency;
    static long numberOfDecodedBits;

    static void initialize();

    static unsigned int getBit(unsigned int r, int position);

    static void normalizeEncoder(char *result);

    static void normalizeDecoder(const char *result);
};


#endif //KODA_CODER_H
