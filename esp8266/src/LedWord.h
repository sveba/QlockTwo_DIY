#ifndef LEDWORD_H
#define LEDWORD_H

/**
 * LedWord representing on word (only horizontal) on the matrix, defined by first pixel and word length.
 */
class LedWord {
public:
    LedWord();
    LedWord(int firstPixelX, int firstPixelY, int length);
    int getFirstPixelX() const;
    void setFirstPixelX(int firstPixelX);
    int getFirstPixelY() const;
    void setFirstPixelY(int firstPixelX);
    int getLength() const;
    void setLength(int length);
private:
    int firstPixelX;
    int firstPixelY;
    int length;
};

#endif //LEDWORD_H
