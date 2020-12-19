//
// Created by Aleksander Nuszel on 2020-12-13.
//

#include "BAC.h"

using namespace std;

unsigned short int BAC::bottom;
unsigned short int BAC::top;
long BAC::insufficiency;
long BAC::encodedIndex;

std::pair<char *, int> BAC::encode(const char *bytesToEncode, int n_zero, int n_one) {
    BAC::initialize();
    int number_of_bytes = (n_zero + n_one) / 8;
    char *res = new char[number_of_bytes];
    long R;
    for (int i = 0; i < number_of_bytes; i++) {
        char byte = *(bytesToEncode + i);
        for (int j = 0; j < 8; j++) {
            R = top - bottom + 1;
            unsigned short int R1 = R * n_zero / (n_zero + n_one);
            if (!getBit(byte, j)) {
                top = bottom + R1;
            } else {
                bottom = bottom + R1;
            }

            for (;;) {
                if ((getBit(top, 15)) == (getBit(bottom, 15))) {
                    encodedIndex++;
                    if (getBit(top, 15)) {
                        *(res + (encodedIndex / 8)) |= 1UL << (encodedIndex % 8);
                    } else {
                        *(res + (encodedIndex / 8)) &= ~(1UL << (encodedIndex % 8));
                    }

                    while (insufficiency > 0) {
                        encodedIndex++;
                        if (!getBit(top, 15)) {
                            *(res + (encodedIndex / 8)) |= 1UL << (encodedIndex % 8);
                        } else {
                            *(res + (encodedIndex / 8)) &= ~(1UL << (encodedIndex % 8));
                        }
                        insufficiency--;
                    }
                } else if ((getBit(bottom, 14)) && !(getBit(top, 14))) {
                    insufficiency++;
                    bottom &= 0x3fff;
                    top |= 0x4000;
                } else {
                    break;
                }
                bottom <<= 1;
                top <<= 1;
                top |= 1;
            }
        }
    }
    cout << encodedIndex << endl;
    cout << number_of_bytes * 8 << endl;
    return std::make_pair(res, encodedIndex + 1);
}

void BAC::initialize() {
    bottom = 0x0000;
    top = 0xffff;
    insufficiency = 0;
    encodedIndex = -1;
}

bool BAC::getBit(unsigned short r, int position) {
    return static_cast<bool>((r >> position) & 0x1);
}

std::pair<int, int> BAC::calculate_statistics(const char *data, long numberOfBytes) {
    int zero = 0;
    int one = 0;
    for (int i = 0; i < numberOfBytes; i++) {
        char byte = *(data + i);
        for (int j = 0; j < 8; j++) {
            if (getBit(byte, j)) {
                one++;
            } else {
                zero++;
            }
        }
    }
    return std::make_pair(zero, one);
}

std::pair<char *, int>
BAC::decode(const char *bytes_to_decode, int number_of_bits, int n_zero, int n_one, long originalNumberOfBytes) {
    char *original = new char[originalNumberOfBytes];
    long readIndex;
    long decodedIndex = 0;
    BAC::initialize();
    unsigned int code = 0x0000;
    for (int i = 0; i < 16; i++) {
        if (getBit(*(bytes_to_decode + i / 8), i % 8)) {
            code |= 1UL << i;
        }
    }
    readIndex = 16;
    bool zero;
    while (readIndex < number_of_bits - 1) {
        long R = top - bottom + 1;
        unsigned short int R1 = R * n_zero / (n_zero + n_one);
        if (code - bottom < R1) {
            zero = true;
            top = bottom + R1;
        } else {
            zero = false;
            bottom = bottom + R1;
        }
        for (;;) {
            if ((getBit(top, 15)) == (getBit(bottom, 15))) {

            } else if ((((bottom) & 0x4000) == 0x4000) && ((top & 0x4000) == 0)) {
                code ^= 0x4000;
                bottom &= 0x3fff;
                top |= 0x4000;
            } else {
                if (zero) {
                    *(original + (decodedIndex / 8)) &= ~(1UL << (decodedIndex % 8));
                } else {
                    *(original + (decodedIndex / 8)) |= 1UL << (decodedIndex % 8);
                }
                decodedIndex++;
                break;
            }
            bottom <<= 1;
            top <<= 1;
            top |= 1;
            code <<= 1;
            if (getBit(*(bytes_to_decode + readIndex / 8), readIndex % 8)) {
                code |= 1UL << 0;
            }
            readIndex++;
        }
    }
    cout << decodedIndex<<endl;
    return std::make_pair(original, decodedIndex);
}


