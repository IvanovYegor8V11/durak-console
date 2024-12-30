#include <card.h>
#include <vector>

/*
♠ Spades:      \u2660
♥ Hearts:      \u2665
♦ Diamonds:    \u2666
♣ Clubs:       \u2663
*/

const uint8_t BASE_HAND_SIZE = 6;
const uint8_t RANKS[] = {6, 7, 8, 9, 10, 11, 12, 13, 14};
const wchar_t SUITS[] = {L'\u2660', L'\u2665', L'\u2666', L'\u2663'};

std::vector<card> deck;
std::vector<card> bot_hand;
std::vector<card> user_hand;
card trump_card;

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
    uint8_t row_count = (hand_to_show.size() + BASE_HAND_SIZE - 1) / BASE_HAND_SIZE;

    for (uint8_t i = 0; i < row_count; i++) {
        for (uint8_t j = i * BASE_HAND_SIZE; j < hand_to_show.size(); j++) {
            if (j % BASE_HAND_SIZE == 0 && j != i * BASE_HAND_SIZE) {
                break;
            }
            std::wcout << L"/¯¯¯¯¯¯¯\\";
        }
        std::wcout << std::endl;

        for (uint8_t j = i * BASE_HAND_SIZE; j < hand_to_show.size(); j++) {
            if (j % BASE_HAND_SIZE == 0 && j != i * BASE_HAND_SIZE) {
                break;
            }
            std::wcout << L"|       |";
        }
        std::wcout << std::endl;

        for (uint8_t j = i * BASE_HAND_SIZE; j < hand_to_show.size(); j++) {
            if (j % BASE_HAND_SIZE == 0 && j != i * BASE_HAND_SIZE) {
                break;
            }

            std::string rank_on_card = hand_to_show[j].get_symbols();

            std::wcout << L"|  " << hand_to_show[j].get_suit() << rank_on_card[0] << rank_on_card[1] << L"  |";
        }
        std::wcout << std::endl;

        for (uint8_t j = i * BASE_HAND_SIZE; j < hand_to_show.size(); j++) {
            if (j % BASE_HAND_SIZE == 0 && j != i * BASE_HAND_SIZE) {
                break;
            }
            std::wcout << L"|       |";
        }
        std::wcout << std::endl;

        for (uint8_t j = i * BASE_HAND_SIZE; j < hand_to_show.size(); j++) {
            if (j % BASE_HAND_SIZE == 0 && j != i * BASE_HAND_SIZE) {
                break;
            }
            std::wcout << L"\\_______/";
        }
        std::wcout << std::endl;

        for (uint8_t j = i * BASE_HAND_SIZE; j < hand_to_show.size(); j++) {
            if (j % BASE_HAND_SIZE == 0 && j != i * BASE_HAND_SIZE) {
                break;
            }

            std::wcout << L"   |" << j << ((j > 9) ? L"|  " : L"|   ");
            
        }
        std::wcout << std::endl;
    }
}

void sort_hand(std::vector<card>& hand_to_sort) {
    uint8_t trump_edge = 0;
    for (uint8_t i = 0; i < hand_to_sort.size(); i++) {
        if (hand_to_sort[i].get_suit() != trump_card.get_suit()) {
            continue;
        }

        std::swap(hand_to_sort[i], hand_to_sort[trump_edge]);
        trump_edge++;
    }

    for (uint8_t i = 0; i < trump_edge; i++) {
        uint8_t max_ind = i;
        for (uint8_t j = i + 1; j < trump_edge; j++) {
            if (hand_to_sort[max_ind].get_rank() < hand_to_sort[j].get_rank()) {
                max_ind = j;
            }
        }

        std::swap(hand_to_sort[i], hand_to_sort[max_ind]);
    }

    for (uint8_t i = trump_edge; i < hand_to_sort.size(); i++) {
        uint8_t max_ind = i;
        for (uint8_t j = i + 1; j < hand_to_sort.size(); j++) {
            if (hand_to_sort[max_ind].get_rank() < hand_to_sort[j].get_rank()) {
                max_ind = j;
            }
        }

        std::swap(hand_to_sort[i], hand_to_sort[max_ind]);
    }
}

void show_game_field() {
    uint8_t deck_size = deck.size() + 1;
    std::string trump_card_symbols = trump_card.get_symbols();

    std::wcout << L"/¯¯¯¯¯¯¯\\\n";
    std::wcout << L"|       |¯¯¯¯\\\n";
    std::wcout << L"|   " << deck_size << ((deck_size > 9) ? L"  | " : L"   | ") 
        << trump_card.get_suit() << trump_card_symbols[0] << trump_card_symbols[1] << L"|   "
        << L"bot hand size: " << bot_hand.size() << std::endl;
    std::wcout << L"|       |____/\n";
    std::wcout << L"\\_______/\n";
}

int main() {
    srand(time(nullptr));
    std::locale::global(std::locale("en_US.UTF-8"));
    std::wcout.imbue(std::locale());

    // card new_card(RANKS[rand() % 9], SUITS[rand() % 4]);
    // new_card.show_card();

    make_deck();
    shuffle_deck();

    for (uint8_t i = 0; i < BASE_HAND_SIZE; i++) {
        bot_hand.push_back(deck.back());
        deck.pop_back();
        user_hand.push_back(deck.back());
        deck.pop_back();
        // std::wcout << user_hand[i].get_rank() << user_hand[i].get_suit() << std::endl;
    }

    trump_card = deck.back();
    deck.pop_back();

    show_game_field();
    sort_hand(user_hand);

    // show_hand(bot_hand);
    show_hand(user_hand);

    // while (!deck.empty()) {
    //     deck.back().show_card();
    //     deck.pop_back();
    // }

    return 0;
}