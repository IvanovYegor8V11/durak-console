#ifndef CARD_H
#define CARD_H

#include <iostream>

class card {
    uint8_t rank;
    char suit;

public:
    uint8_t get_rank();
    char get_suit();
};

#endif