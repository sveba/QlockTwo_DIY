#include "LedWord.h"

LedWord::LedWord(int firstPixelX, int firstPixelY, int length)  {
  this->firstPixelX =  firstPixelX;
  this->firstPixelY = firstPixelY;
  this->length = length;
}

int LedWord::getFirstPixelX() {
        return firstPixelX;
};

void LedWord::setFirstPixelX(int firstPixelX) {
        this->firstPixelX = firstPixelX;
};

int LedWord::getFirstPixelY() {
        return firstPixelY;
};

void LedWord::setFirstPixelY(int firstPixelY) {
        this->firstPixelY = firstPixelY;
};

int LedWord::getLength() {
        return length;
};

void LedWord::setLength(int length) {
        this->length = length;
};
