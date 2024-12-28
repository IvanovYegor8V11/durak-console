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

void show_user_hand() {
    uint8_t row_count = (user_hand.size() + 5) / CARDS_IN_ROW;

    for (uint8_t i = 0; i < row_count; i++) {
        for (uint8_t j = i * CARDS_IN_ROW; j < user_hand.size(); j++) {
            if (j % CARDS_IN_ROW == 0 && j != i * CARDS_IN_ROW) {
                break;
            }
            std::wcout << L"/¯¯¯¯¯¯¯\\";
        }
        std::wcout << std::endl;

        for (uint8_t j = i * CARDS_IN_ROW; j < user_hand.size(); j++) {
            if (j % CARDS_IN_ROW == 0 && j != i * CARDS_IN_ROW) {
                break;
            }
            std::wcout << L"|       |";
        }
        std::wcout << std::endl;

        for (uint8_t j = i * CARDS_IN_ROW; j < user_hand.size(); j++) {
            if (j % CARDS_IN_ROW == 0 && j != i * CARDS_IN_ROW) {
                break;
            }

            char rank_on_card[2] = {' ', ' '};

            if (user_hand[j].get_rank() < 10) {
                rank_on_card[1] = '0' + user_hand[j].get_rank();
            }
            else if (user_hand[j].get_rank() == 10) {
                rank_on_card[0] = '1';
                rank_on_card[1] = '0';
            }
            else if (user_hand[j].get_rank() > 10) {
                switch (user_hand[j].get_rank()) {
                    case 11:
                        rank_on_card[1] = 'J';
                        break;
                    case 12:
                        rank_on_card[1] = 'Q';
                        break;
                    case 13:
                        rank_on_card[1] = 'K';
                        break;
                    case 14:
                        rank_on_card[1] = 'A';
                        break;
                    default:
                        rank_on_card[0] = 'E';
                        rank_on_card[1] = 'R';
                        break;
                }
            }
            std::wcout << L"|  " << user_hand[j].get_suit() << rank_on_card[0] << rank_on_card[1] << L"  |";
        }
        std::wcout << std::endl;

        for (uint8_t j = i * CARDS_IN_ROW; j < user_hand.size(); j++) {
            if (j % CARDS_IN_ROW == 0 && j != i * CARDS_IN_ROW) {
                break;
            }
            std::wcout << L"|       |";
        }
        std::wcout << std::endl;

        for (uint8_t j = i * CARDS_IN_ROW; j < user_hand.size(); j++) {
            if (j % CARDS_IN_ROW == 0 && j != i * CARDS_IN_ROW) {
                break;
            }
            std::wcout << L"\\_______/";
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

    for (uint8_t i = 0; i < 13; i++) {
        user_hand.push_back(deck.back());
        deck.pop_back();
        std::wcout << user_hand[i].get_rank() << user_hand[i].get_suit() << std::endl;
    }

    show_user_hand();

    // while (!deck.empty()) {
    //     deck.back().show_card();
    //     deck.pop_back();
    // }

    return 0;
}