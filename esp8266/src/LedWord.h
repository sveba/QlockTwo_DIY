#ifndef LEDWORD_H
#define LEDWORD_H


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
