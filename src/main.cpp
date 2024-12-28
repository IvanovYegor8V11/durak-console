#include <card.h>

/*
♠ Spades: Unicode \u2660
♥ Hearts: Unicode \u2665
♦ Diamonds: Unicode \u2666
♣ Clubs: Unicode \u2663
*/

const uint8_t RANKS[9] = {6, 7, 8, 9, 10, 11, 12, 13, 14};
const wchar_t SUITS[4] = {L'\u2660', L'\u2665', L'\u2666', L'\u2663'};

int main() {
    srand(time(nullptr));
    std::locale::global(std::locale("en_US.UTF-8"));
    std::wcout.imbue(std::locale());

    card new_card(RANKS[rand() % 9], SUITS[rand() % 4]);

    new_card.show_card();

    return 0;
}