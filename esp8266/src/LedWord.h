#ifndef LEDWORD_H
#define LEDWORD_H


class LedWord {
public:
    LedWord(int firstPixelX, int firstPixelY, int length);
    int getFirstPixelX();
    void setFirstPixelX(int firstPixelX);
    int getFirstPixelY();
    void setFirstPixelY(int firstPixelX);
    int getLength();
    void setLength(int length);
private:
    int firstPixelX;
    int firstPixelY;
    int length;
};

#endif //LEDWORD_H
