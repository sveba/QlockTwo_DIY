
#ifndef LedMapping_H
#define LedMapping_H

#include "LedWord.h"

const LedWord PREFIX_IT = LedWord(0, 0, 2); // ES
const LedWord PREFIX_IS = LedWord(3, 0, 3); // IST

const LedWord INFIX_AFTER = LedWord(0, 3, 3); // VOR
const LedWord INFIX_BEFORE = LedWord(3, 3, 4); // NACH

const LedWord SUFFIX_OCLOCK = LedWord(8, 9, 3); // UHR

const LedWord MINUTE_FIVE = LedWord(7, 0, 4); // FÜNF
const LedWord MINUTE_TEN = LedWord(0, 1, 4); // ZEHN
const LedWord MINUTE_QUARTER = LedWord(3, 2, 7); // VIERTEL
const LedWord MINUTE_TWENTY = LedWord(4, 1, 7); // ZWANZIG
const LedWord MINUTE_HALF = LedWord(0, 4, 4); // HALB
const LedWord MINUTE_THREEQUARTER = LedWord(0, 2, 11); // THREEVIERTEL

const LedWord HOURS[] = {   LedWord(0, 5, 4), // EINS
                            LedWord(7, 5, 4), // ZWEI
                            LedWord(0, 6, 4), // DREI
                            LedWord(7, 6, 4), // VIER
                            LedWord(7, 4, 4), // FÜNF
                            LedWord(0, 7, 5), // SECHS
                            LedWord(0, 8, 6), // SIEBEN
                            LedWord(7, 7, 4), // ACHT
                            LedWord(3, 9, 4), // NEUN
                            LedWord(0, 9, 4), // ZEHN
                            LedWord(5, 4, 3), // ELF
                            LedWord(6, 8, 5) // ZWÖLF
                        };

#endif /* LedMapping_H */