#include <card.h>
#include <vector>

/*
♠ Spades:      \u2660
♥ Hearts:      \u2665
♦ Diamonds:    \u2666
♣ Clubs:       \u2663
*/

const uint8_t CARDS_IN_ROW = 6;
const uint8_t RANKS[] = {6, 7, 8, 9, 10, 11, 12, 13, 14};
const wchar_t SUITS[] = {L'\u2660', L'\u2665', L'\u2666', L'\u2663'};

std::vector<card> deck;
std::vector<card> bot_hand;
std::vector<card> user_hand;

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

void show_hand(std::vector<card>& hand_to_show) {
    uint8_t row_count = (hand_to_show.size() + 5) / CARDS_IN_ROW;

    for (uint8_t i = 0; i < row_count; i++) {
        for (uint8_t j = i * CARDS_IN_ROW; j < hand_to_show.size(); j++) {
            if (j % CARDS_IN_ROW == 0 && j != i * CARDS_IN_ROW) {
                break;
            }
            std::wcout << L"/¯¯¯¯¯¯¯\\";
        }
        std::wcout << std::endl;

        for (uint8_t j = i * CARDS_IN_ROW; j < hand_to_show.size(); j++) {
            if (j % CARDS_IN_ROW == 0 && j != i * CARDS_IN_ROW) {
                break;
            }
            std::wcout << L"|       |";
        }
        std::wcout << std::endl;

        for (uint8_t j = i * CARDS_IN_ROW; j < hand_to_show.size(); j++) {
            if (j % CARDS_IN_ROW == 0 && j != i * CARDS_IN_ROW) {
                break;
            }

            std::string rank_on_card = hand_to_show[j].get_symbols();

            std::wcout << L"|  " << hand_to_show[j].get_suit() << rank_on_card[0] << rank_on_card[1] << L"  |";
        }
        std::wcout << std::endl;

        for (uint8_t j = i * CARDS_IN_ROW; j < hand_to_show.size(); j++) {
            if (j % CARDS_IN_ROW == 0 && j != i * CARDS_IN_ROW) {
                break;
            }
            std::wcout << L"|       |";
        }
        std::wcout << std::endl;

        for (uint8_t j = i * CARDS_IN_ROW; j < hand_to_show.size(); j++) {
            if (j % CARDS_IN_ROW == 0 && j != i * CARDS_IN_ROW) {
                break;
            }
            std::wcout << L"\\_______/";
        }
        std::wcout << std::endl;

        for (uint8_t j = i * CARDS_IN_ROW; j < hand_to_show.size(); j++) {
            if (j % CARDS_IN_ROW == 0 && j != i * CARDS_IN_ROW) {
                break;
            }

            std::wcout << L"   |" << j << ((j > 9) ? L"|  " : L"|   ");
            
        }
        std::wcout << std::endl;
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

    for (uint8_t i = 0; i < 6; i++) {
        bot_hand.push_back(deck.back());
        deck.pop_back();
        user_hand.push_back(deck.back());
        deck.pop_back();
        // std::wcout << user_hand[i].get_rank() << user_hand[i].get_suit() << std::endl;
    }

    // show_hand(bot_hand);
    show_hand(user_hand);

    // while (!deck.empty()) {
    //     deck.back().show_card();
    //     deck.pop_back();
    // }

    return 0;
}