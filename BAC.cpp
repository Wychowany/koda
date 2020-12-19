//
// Created by Aleksander Nuszel on 2020-12-13.
//

#include "BAC.h"
#include <bitset>
#include <iostream>

using namespace std;

unsigned short int BAC::bottom;
unsigned long long int R;
long BAC::insufficiency;
long BAC::numberOfEncodedBits;
long BAC::numberOfDecodedBits;
unsigned short int code;
long numberOfReadBits;


#define HALF 1 << 15
#define QUARTER 1 << 14

void BAC::initialize() {
    bottom = 0x0000;
    insufficiency = 0;
    R = 0xffff + 1;
    numberOfEncodedBits = 0;
    numberOfDecodedBits = 0;
    code = 0;
    numberOfReadBits = 0;
}


std::pair<char *, int> BAC::encode(const char *bytesToEncode, unsigned int n_zero, unsigned int n_one) {
    initialize();
    unsigned int numberOfBits = (n_zero + n_one);
    unsigned int numberOfBytes = numberOfBits / 8;
    char *result = new char[numberOfBytes];
    for (int i = 0; i < numberOfBytes; i++) {

        const char currentByte = *(bytesToEncode + i);

        for (int j = 0; j < 8; j++) {
            unsigned long long int R1 = R * n_zero / (n_zero + n_one);
            unsigned long long int R2 = R - R1;
            if (getBit(currentByte, j) == 0) {
                R = R1;
                bottom = bottom + R2;
            } else {
                R = R2;
            }
            normalizeEncoder(result);
        }
    }
    cout <<"Original number of bits: "<< numberOfBytes * 8 << endl;
    cout << "Encoded number of bits: " << numberOfEncodedBits << endl;
    return std::make_pair(result, numberOfEncodedBits);
}

void BAC::normalizeEncoder(char *result) {
    while (R <= QUARTER) {
        if (bottom >= HALF) {
            *(result + numberOfEncodedBits / 8) |= 1UL << (numberOfEncodedBits % 8);
            numberOfEncodedBits++;
            while (insufficiency > 0) {
                *(result + numberOfEncodedBits / 8) &= ~(1UL << (numberOfEncodedBits % 8));
                numberOfEncodedBits++;
                insufficiency--;
            }
            bottom -= HALF;
        } else if (bottom + R <= HALF) {
            *(result + numberOfEncodedBits / 8) &= ~(1UL << (numberOfEncodedBits % 8));
            numberOfEncodedBits++;
            while (insufficiency > 0) {
                *(result + numberOfEncodedBits / 8) |= 1UL << (numberOfEncodedBits % 8);
                numberOfEncodedBits++;
                insufficiency--;
            }
        } else {
            insufficiency++;
            bottom -= QUARTER;
        }
        bottom <<= 1;
        R <<= 1;
//        cout<<"R:" <<R<<endl;
//        cout<<"bottm:" <<bottom<<endl;
    }
}

// returns 0 or 1, where 0 position is least significant
int BAC::getBit(unsigned short r, int position) {
    return (r >> position) & 1;
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
BAC::decode(char *bytes_to_decode, int number_of_bits, int n_zero, int n_one, long originalNumberOfBytes) {
    initialize();
    char *result = new char[originalNumberOfBytes];
    long originalNumberOfBits = originalNumberOfBytes * 8;
    for (int i = 0; i < 16; i++) {
        code <<= 1;
        if (getBit(*(bytes_to_decode + i / 8), i % 8)) {
            code |= 1UL << i;
        }
    }
    numberOfReadBits = 15;

    while (numberOfDecodedBits < originalNumberOfBits) {
        unsigned int R1 = R * n_zero / (n_zero + n_one);
        unsigned int R2 = R - R1;
        if (code - bottom >= R2) {
            R = R1;
            bottom = bottom + R2;
            *(result + numberOfDecodedBits / 8) &= ~(1UL << (numberOfDecodedBits % 8));
        } else {
            R = R2;
            *(result + numberOfDecodedBits / 8) |= 1UL << (numberOfDecodedBits % 8);
        }
        numberOfDecodedBits++;
        normalizeDecoder(bytes_to_decode);
    }
    return std::make_pair(result, originalNumberOfBits);
}

void BAC::normalizeDecoder(char *bytes_to_decode) {
    while (R <= QUARTER) {
        if (bottom + R <= HALF) {

        } else if (bottom >= HALF) {
            bottom -= HALF;
            code -= HALF;
        } else {
            bottom -= QUARTER;
            code -= QUARTER;
        }
        bottom <<= 1;
        R <<= 1;
        code <<= 1;
        if (getBit(*(bytes_to_decode + numberOfReadBits / 8), numberOfReadBits % 8)) {
            code = code + 1;
        }
        numberOfReadBits++;
    }
}


