#ifndef CARD_H
#define CARD_H

#include <iostream>
#include <locale>
#include <codecvt>

class card {
    uint8_t rank;
    wchar_t suit;

public:
    card();
    card(uint8_t rank, wchar_t suit);
    uint8_t get_rank();
    wchar_t get_suit();
    void show_card();
    std::string get_symbols();
};

#endif /*CARD_H*/