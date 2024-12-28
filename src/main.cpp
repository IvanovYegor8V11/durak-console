#include <card.h>
#include <vector>

/*
♠ Spades:      \u2660
♥ Hearts:      \u2665
♦ Diamonds:    \u2666
♣ Clubs:       \u2663
*/

const uint8_t RANKS[] = {6, 7, 8, 9, 10, 11, 12, 13, 14};
const wchar_t SUITS[] = {L'\u2660', L'\u2665', L'\u2666', L'\u2663'};

std::vector<card> deck;

void make_deck() {
    for (const wchar_t& suit : SUITS) {
        for (const uint8_t& rank : RANKS) {
            card new_card(rank, suit);
            deck.push_back(new_card);
        }
    }
}

void shuffle_deck() {
    for (uint8_t i = 0; i < deck.size(); i++) {
        uint8_t ind = rand() % deck.size();
        std::swap(deck[i], deck[ind]);
    }
}

int main() {
    srand(time(nullptr));
    std::locale::global(std::locale("en_US.UTF-8"));
    std::wcout.imbue(std::locale());

    // card new_card(RANKS[rand() % 9], SUITS[rand() % 4]);
    // new_card.show_card();

    make_deck();
    shuffle_deck();

    while (!deck.empty()) {
        deck.back().show_card();
        deck.pop_back();
    }

    return 0;
}