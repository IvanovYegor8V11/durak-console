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
card* trump_card;
uint8_t bot_hand_trump_edge = 0;
uint8_t user_hand_trump_edge = 0;

std::string set_color_mode(wchar_t suit) {
    std::string color_code;

    switch (suit) {
        case L'♠':
        case L'♣':
            color_code = "\033[30m"; // Black
            break;
        case L'♥':
        case L'♦':
            color_code = "\033[31m"; // Red
            break;
        default:
            color_code = "\033[0m";  // Default
            break;
    }

    return color_code;
}

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

void start_card_distribution() {
    for (uint8_t i = 0; i < BASE_HAND_SIZE; i++) {
        bot_hand.push_back(deck.back());
        deck.pop_back();
        user_hand.push_back(deck.back());
        deck.pop_back();
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
            std::string color_code = set_color_mode(hand_to_show[j].get_suit());

            std::wcout << L"|  " << color_code[0] << color_code[1] << color_code[2] << color_code[3] << color_code[4] 
                << hand_to_show[j].get_suit() << L"\033[0m" << rank_on_card[0] << rank_on_card[1] << L"  |";
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
        if (hand_to_sort[i].get_suit() != trump_card->get_suit()) {
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

void show_game_stat() {
    uint8_t deck_size = deck.size();
    std::string trump_card_symbols = trump_card->get_symbols();
    std::string color_code = set_color_mode(trump_card->get_suit());

    std::wcout << L"/¯¯¯¯¯¯¯\\\n";
    std::wcout << L"|       |¯¯¯¯\\\n";
    std::wcout << L"|   " << deck_size << ((deck_size > 9) ? L"  | " : L"   | ") 
        << color_code[0] << color_code[1] << color_code[2] << color_code[3] << color_code[4]
        << trump_card->get_suit() << L"\033[0m" << trump_card_symbols[0] << trump_card_symbols[1] << L"|   "
        << L"bot hand size: " << bot_hand.size() << std::endl;
    std::wcout << L"|       |____/\n";
    std::wcout << L"\\_______/\n";
}

uint8_t find_trump_edge(std::vector<card>& hand_to_find_in) {
    uint8_t trump_edge = 0;
    wchar_t trump_card_suit = trump_card->get_suit();
    for (uint8_t i = 0; i < hand_to_find_in.size(); i++) {
        if (hand_to_find_in[i].get_suit() != trump_card_suit) {
            break;
        }

        trump_edge++;
    }

    return trump_edge;
}

bool min_rank_definer(uint8_t user_start_ind, uint8_t bot_start_ind, uint8_t user_end_ind = 0, uint8_t bot_end_ind = 0) {
    uint8_t cur_user_min_ind = user_start_ind;
    uint8_t cur_user_min_rank = user_hand[cur_user_min_ind].get_rank();
    uint8_t cur_bot_min_ind = bot_start_ind;
    uint8_t cur_bot_min_rank = bot_hand[cur_bot_min_ind].get_rank();

    while (cur_user_min_ind >= user_end_ind || cur_bot_min_ind >= bot_end_ind) {
        if (cur_bot_min_rank > cur_user_min_rank) {
            return true;
        }
        else if (cur_bot_min_rank < cur_user_min_rank) {
            return false;
        }

        cur_user_min_ind--;
        cur_bot_min_ind--;

        uint8_t next_user_min_rank;
        do {
            next_user_min_rank = user_hand[cur_user_min_ind].get_rank();
        }
        while (next_user_min_rank == cur_user_min_rank);
        cur_user_min_rank = next_user_min_rank;

        uint8_t next_bot_min_rank;
        do {
            next_bot_min_rank = bot_hand[cur_bot_min_ind].get_rank();
        }
        while (next_bot_min_rank == cur_bot_min_rank);
        cur_bot_min_rank = next_bot_min_rank;        
    }

    return false;    
}

bool first_player_definer() {
    if (bot_hand_trump_edge == 0 && user_hand_trump_edge != 0) {
        return true;
    }
    else if (bot_hand_trump_edge != 0 && user_hand_trump_edge == 0) {
        return false;
    }
    else if (bot_hand_trump_edge == 0 && user_hand_trump_edge == 0) {
        return min_rank_definer(BASE_HAND_SIZE - 1, BASE_HAND_SIZE - 1, user_hand_trump_edge, bot_hand_trump_edge);
    }
    else {
        return min_rank_definer(user_hand_trump_edge - 1, bot_hand_trump_edge - 1);
    }
}

int main() {
    srand(time(nullptr));
    std::locale::global(std::locale("en_US.UTF-8"));
    std::wcout.imbue(std::locale());

    make_deck();
    shuffle_deck();

    start_card_distribution();

    deck.insert(deck.begin(), deck.back());
    deck.pop_back();
    trump_card = &deck[0];

    show_game_stat();
    sort_hand(user_hand);
    sort_hand(bot_hand);
    bot_hand_trump_edge = find_trump_edge(bot_hand);
    user_hand_trump_edge = find_trump_edge(user_hand);

    show_hand(bot_hand);
    std::wcout << find_trump_edge(bot_hand) << std::endl;
    show_hand(user_hand);
    std::wcout << find_trump_edge(user_hand) << std::endl;

    if (first_player_definer()) {
        std::wcout << L"User plays first\n";
    }
    else {
        std::wcout <<  L"Bot plays first\n";
    }    

    return 0;
}