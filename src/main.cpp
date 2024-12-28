#include <card.h>
#include <queue>

/*
♠ Spades:      \u2660
♥ Hearts:      \u2665
♦ Diamonds:    \u2666
♣ Clubs:       \u2663
*/

const uint8_t RANKS[] = {6, 7, 8, 9, 10, 11, 12, 13, 14};
const wchar_t SUITS[] = {L'\u2660', L'\u2665', L'\u2666', L'\u2663'};

std::queue<card> deck;

void make_deck() {
    for (wchar_t suit : SUITS) {
        for (uint8_t rank : RANKS) {
            card new_card(rank, suit);
            deck.push(new_card);
        }
    }
}

int main() {
    srand(time(nullptr));
    std::locale::global(std::locale("en_US.UTF-8"));
    std::wcout.imbue(std::locale());

    card new_card(RANKS[rand() % 9], SUITS[rand() % 4]);
    new_card.show_card();

    make_deck();

    wchar_t command;
    card top_card;
    while (!deck.empty()) {
        std::wcout << L"Do you want to see the card? (y/n): ";
        std::wcin >> command;

        switch (command) {
            case L'y':
                deck.front().show_card();
                deck.pop();
                break;
            case L'n':
                std::wcout << L"Bye-bye.\n";
                return 0;
            default:
                std::wcout << L"Wrong command!!!\n";
                break;
        }
    }

    return 0;
}