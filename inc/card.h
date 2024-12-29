#ifndef CARD_H
#define CARD_H

#include <iostream>
#include <locale>
#include <codecvt>

class card {
    uint8_t rank;
    wchar_t suit;

public:
    card(uint8_t rank = 0, wchar_t suit = '\0');
    uint8_t get_rank();
    wchar_t get_suit();
    void show_card();
    std::string get_symbols();
};

#endif /*CARD_H*/