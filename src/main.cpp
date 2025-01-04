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
std::vector<card> attacking_cards(6);
std::vector<card> defending_cards(6);
card* trump_card;
card empty_card;
uint8_t bot_hand_trump_edge = 0;
uint8_t user_hand_trump_edge = 0;
bool defending = true; /*if true - user defends, else - bot defends*/

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

void get_card_from_deck(std::vector<card>& hand_to_get_card_in) {
    hand_to_get_card_in.push_back(deck.back());
    deck.pop_back();
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

            wchar_t cur_suit = hand_to_show[j].get_suit();
            std::string rank_on_card = hand_to_show[j].get_symbols();
            std::string color_code = set_color_mode(cur_suit);

            std::wcout << L"|  " << color_code[0] << color_code[1] << color_code[2] << color_code[3] << color_code[4] 
                << cur_suit << L"\033[0m" << rank_on_card[0] << rank_on_card[1] << L"  |";
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

void show_deck() {
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

void show_game_field_row(uint8_t start, uint8_t end) {
    std::wcout << L"                  ";
    for (uint8_t i = start; i < end; i++) {
        std::wcout << L"   |" << i << L"|     ";            
    }
    std::wcout << std::endl;
    
    std::wcout << L"                  ";
    for (uint8_t i = start; i < end; i++) {
        if (attacking_cards[i].get_rank() == 0) {
            std::wcout << L"|¯¯¯¯¯¯¯|  ";
        }
        else if (defending_cards[i].get_rank() != 0){
            wchar_t cur_suit = attacking_cards[i].get_suit();
            std::string rank_on_card = attacking_cards[i].get_symbols();
            std::string color_code = set_color_mode(cur_suit);
            std::wcout << L"/¯¯" << color_code[0] << color_code[1] << color_code[2] << color_code[3] << color_code[4] 
                << cur_suit << L"\033[0m" << rank_on_card[0] << rank_on_card[1] << L"¯¯\\  ";
        }
        else {
            std::wcout << L"/¯¯¯¯¯¯¯\\  ";
        }
    }
    std::wcout << std::endl;

    std::wcout << L"                  ";
    for (uint8_t i = start; i < end; i++) {
        if (attacking_cards[i].get_rank() == 0) {
            std::wcout << L"|       |  ";
        }
        else if (defending_cards[i].get_rank() != 0){
            std::wcout << L"| /¯¯¯¯¯¯¯\\";
        }
        else {
            std::wcout << L"|       |  ";
        }
    }
    std::wcout << std::endl;

    std::wcout << L"                  ";
    for (uint8_t i = start; i < end; i++) {
        if (attacking_cards[i].get_rank() == 0) {
            std::wcout << L"|   +   |  ";
        }
        else if (defending_cards[i].get_rank() != 0){
            std::wcout << L"| |       |";
        }
        else {
            wchar_t cur_suit = attacking_cards[i].get_suit();
            std::string rank_on_card = attacking_cards[i].get_symbols();
            std::string color_code = set_color_mode(cur_suit);
            std::wcout << L"|  " << color_code[0] << color_code[1] << color_code[2] << color_code[3] << color_code[4] 
                << cur_suit << L"\033[0m" << rank_on_card[0] << rank_on_card[1] << L"  |  ";
        }
    }
    std::wcout << std::endl;

    std::wcout << L"                  ";
    for (uint8_t i = start; i < end; i++) {
        if (attacking_cards[i].get_rank() != 0 && defending_cards[i].get_rank() != 0){
            wchar_t cur_suit = defending_cards[i].get_suit();
            std::string rank_on_card = defending_cards[i].get_symbols();
            std::string color_code = set_color_mode(cur_suit);
            std::wcout << L"| |  " << color_code[0] << color_code[1] << color_code[2] << color_code[3] << color_code[4] 
                << cur_suit << L"\033[0m" << rank_on_card[0] << rank_on_card[1] << L"  |";
        }
        else {
            std::wcout << L"|       |  ";
        }
    }
    std::wcout << std::endl;

    std::wcout << L"                  ";
    for (uint8_t i = start; i < end; i++) {
        if (attacking_cards[i].get_rank() == 0) {
            std::wcout << L"|_______|  ";
        }
        else if (defending_cards[i].get_rank() != 0){
            std::wcout << L"\\_|       |";
        }
        else {
            std::wcout << L"\\_______/  ";
        }
    }
    std::wcout << std::endl;

    std::wcout << L"                  ";
    for (uint8_t i = start; i < end; i++) {
        if (attacking_cards[i].get_rank() != 0 && defending_cards[i].get_rank() != 0){
            std::wcout << L"  \\_______/";
        }
        else {
            std::wcout << L"           ";
        }
    }
    std::wcout << std::endl;
    std::wcout << std::endl;
}

void show_game_field() {
    show_game_field_row(0, BASE_HAND_SIZE / 2);
    show_game_field_row(BASE_HAND_SIZE / 2, BASE_HAND_SIZE);
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

bool five_suits_checker(std::vector<card>& hand_to_check) {
    wchar_t suit_to_check;
    uint8_t suit_counter;
    for (uint8_t i = 0; i < 2; i++) {
        suit_to_check = hand_to_check[i].get_suit();
        suit_counter = 1;
        for (uint8_t j = i + 1; j < BASE_HAND_SIZE; j++) {
            if (suit_to_check == hand_to_check[j].get_suit()) {
                suit_counter++;
            }
        }

        if (suit_counter >= 5) {
            return false;
        }
    }

    return true;
}

bool can_bot_defend(card* card_to_beat, uint8_t card_ind) {
    uint8_t rank = card_to_beat->get_rank();
    wchar_t suit = card_to_beat->get_suit();
    bool is_card_trump = (suit == trump_card->get_suit());
    int8_t i = is_card_trump ? bot_hand_trump_edge - 1 : bot_hand.size() - 1;
    int8_t bot_hand_ind = -1;
    for (; i >= 0; i--) {
        if (i < bot_hand_trump_edge && (!is_card_trump || bot_hand[i].get_rank() > rank)) {
            bot_hand_ind = i;
            break;
        }
        else if (bot_hand[i].get_suit() == suit && bot_hand[i].get_rank() > rank) {
            bot_hand_ind = i;
            break;
        }
    }

    if (bot_hand_ind != -1) {
        defending_cards[card_ind] = bot_hand[bot_hand_ind];
        bot_hand.erase(bot_hand.begin() + bot_hand_ind);
        bot_hand_trump_edge = find_trump_edge(bot_hand);
        return true;
    }

    return false;
}

int main() {
    srand(time(nullptr));
    std::locale::global(std::locale("en_US.UTF-8"));
    std::wcout.imbue(std::locale());
    make_deck();

    /*card new_attacking_card(RANKS[rand() % 9], SUITS[rand() % 4]);
    attacking_cards[0] = new_attacking_card;

    card new_defending_card(RANKS[rand() % 9], SUITS[rand() % 4]);
    defending_cards[0] = new_defending_card;

    card another_new_defending_card(RANKS[rand() % 9], SUITS[rand() % 4]);
    defending_cards[1] = another_new_defending_card;

    card another_new_attacking_card(RANKS[rand() % 9], SUITS[rand() % 4]);
    attacking_cards[1] = another_new_attacking_card;*/

    bool card_distribution_checker;
    do {        
        shuffle_deck();
        for (uint8_t i = 0; i < BASE_HAND_SIZE; i++) {
            get_card_from_deck(bot_hand);
            get_card_from_deck(user_hand);
        }
        card_distribution_checker = five_suits_checker(user_hand) && five_suits_checker(bot_hand);

        if (!card_distribution_checker) {
            deck.insert(deck.end(), std::make_move_iterator(user_hand.begin()), std::make_move_iterator(user_hand.end()));
            user_hand.clear();
            deck.insert(deck.end(), std::make_move_iterator(bot_hand.begin()), std::make_move_iterator(bot_hand.end()));
            bot_hand.clear();            
        }
            
    }
    while (!card_distribution_checker);

    deck.insert(deck.begin(), deck.back());
    deck.pop_back();
    trump_card = &deck[0];

    show_deck();
    sort_hand(user_hand);
    sort_hand(bot_hand);
    bot_hand_trump_edge = find_trump_edge(bot_hand);
    user_hand_trump_edge = find_trump_edge(user_hand);

    show_hand(bot_hand);
    std::wcout << find_trump_edge(bot_hand) << std::endl;
    show_game_field();
    show_hand(user_hand);
    std::wcout << find_trump_edge(user_hand) << std::endl;

    if (first_player_definer()) {
        defending = false;
        std::wcout << L"User plays first\n";
    }
    else {
        defending = true;
        std::wcout <<  L"Bot plays first\n";
    }

    uint8_t game_ind = 0;
    while (!defending) {
        if (user_hand.size() == 0) {
            break;
        }

        uint16_t number_of_card;
        do {
            std::wcout << L"Choose the card from your hand to play: ";
            std::wcin >> number_of_card;
        }
        while (number_of_card < 0 || number_of_card >= user_hand.size());

        attacking_cards[game_ind] = user_hand[number_of_card];
        user_hand.erase(user_hand.begin() + number_of_card);
        user_hand_trump_edge = find_trump_edge(user_hand);

        if (can_bot_defend(&attacking_cards[game_ind], game_ind)) {
            std::wcout << L"Bot beated the card\n";

            show_deck();
            show_hand(bot_hand);
            std::wcout << find_trump_edge(bot_hand) << std::endl;
            show_game_field();
            show_hand(user_hand);
            std::wcout << find_trump_edge(user_hand) << std::endl;

            game_ind++;
            if (game_ind == 6) {
                game_ind = 0;

                for (uint8_t i = 0; i < BASE_HAND_SIZE; i++) {
                    if (attacking_cards[i].get_rank() != 0) {
                        attacking_cards[i] = empty_card;
                    }
                    
                    if (defending_cards[i].get_rank() != 0) {
                        defending_cards[i] = empty_card;
                    }
                }
            }
        }
        else {
            std::wcout << L"Bot took the card(s)\n";

            for (uint8_t i = 0; i < BASE_HAND_SIZE; i++) {
                if (attacking_cards[i].get_rank() != 0) {
                    bot_hand.push_back(attacking_cards[i]);
                    attacking_cards[i] = empty_card;
                }
                
                if (defending_cards[i].get_rank() != 0) {
                    bot_hand.push_back(defending_cards[i]);
                    defending_cards[i] = empty_card;
                }
            }            
            sort_hand(bot_hand);

            show_deck();
            show_hand(bot_hand);
            std::wcout << find_trump_edge(bot_hand) << std::endl;
            show_game_field();
            show_hand(user_hand);
            std::wcout << find_trump_edge(user_hand) << std::endl;

            game_ind = 0;
        }
    }

    return 0;
}
