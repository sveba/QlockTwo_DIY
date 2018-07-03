#include "LedWord.h"

LedWord::LedWord(){};

LedWord::LedWord(int _firstPixelX, int _firstPixelY, int _length) : firstPixelX(_firstPixelX), firstPixelY(_firstPixelY), length(_length) {
}

int LedWord::getFirstPixelX() const{
        return firstPixelX;
};

void LedWord::setFirstPixelX(int firstPixelX) {
        this->firstPixelX = firstPixelX;
};

int LedWord::getFirstPixelY() const{
        return firstPixelY;
};

void LedWord::setFirstPixelY(int firstPixelY) {
        this->firstPixelY = firstPixelY;
};

int LedWord::getLength() const {
        return length;
};

void LedWord::setLength(int length) {
        this->length = length;
};
