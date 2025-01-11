#include <card.h>
#include <vector>
#include <thread>
#include <chrono>
#include <stdlib.h>

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
uint8_t defending = 0; /*if true - user defends, else - bot defends*/
uint8_t game_ind = 0;

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

bool can_bot_defend(uint8_t& game_ind) {
    uint8_t rank = attacking_cards[game_ind].get_rank();
    wchar_t suit = attacking_cards[game_ind].get_suit();
    bool is_card_trump = (suit == trump_card->get_suit());
    int8_t i = is_card_trump ? bot_hand_trump_edge - 1 : bot_hand.size() - 1;
    int8_t bot_hand_ind = -1;
    for (; i >= 0; i--) {
        if (i < bot_hand_trump_edge && !is_card_trump) {
            bot_hand_ind = i;
            break;
        }
        else if (bot_hand[i].get_suit() == suit && bot_hand[i].get_rank() > rank) {
            bot_hand_ind = i;
            break;
        }
    }

    if (bot_hand_ind != -1) {
        defending_cards[game_ind] = bot_hand[bot_hand_ind];
        bot_hand.erase(bot_hand.begin() + bot_hand_ind);
        bot_hand_trump_edge = find_trump_edge(bot_hand);
        return true;
    }

    return false;
}

void fill_hand(std::vector<card>& hand_to_fill) {
    int8_t difference = BASE_HAND_SIZE - hand_to_fill.size();
    difference = difference < 0 ? 0 : difference;
    int8_t deck_size = deck.size();
    int8_t count_new_cards = deck_size > difference ? difference : deck_size;
    for (uint8_t i = 0; i < count_new_cards; i++) {
        get_card_from_deck(hand_to_fill);
    }
    sort_hand(hand_to_fill);
}

bool can_attack(card& attacking_card, uint8_t& game_ind) {
    if (game_ind == 0) {
        return true;
    }

    uint8_t attacking_rank = attacking_card.get_rank();
    for (uint8_t i = 0; i < game_ind; i++) {
        if (attacking_cards[i].get_rank() == attacking_rank) {
            return true;
        }

        if (defending_cards[i].get_rank() == attacking_rank) {
            return true;
        }
    }

    return false;    
}

void show_all() {
    system("clear");     // #include <stdlib.h> system("cls"); or std::wcout << L"\x1B[2J\x1B[H";
    std::wcout << game_ind << std::endl;
    show_deck();
    // show_hand(bot_hand);
    // std::wcout << find_trump_edge(bot_hand) << std::endl;
    show_game_field();
    show_hand(user_hand);
    std::wcout << find_trump_edge(user_hand) << std::endl;
}

void take_cards(std::vector<card>& hand_to_take_card_in) {
    for (uint8_t i = 0; i < BASE_HAND_SIZE; i++) {
        if (attacking_cards[i].get_rank() != 0) {
            hand_to_take_card_in.push_back(attacking_cards[i]);
            attacking_cards[i] = empty_card;
        }
        
        if (defending_cards[i].get_rank() != 0) {
            hand_to_take_card_in.push_back(defending_cards[i]);
            defending_cards[i] = empty_card;
        }
    }            
    sort_hand(hand_to_take_card_in);
}

void empty_field() {
    for (uint8_t i = 0; i < BASE_HAND_SIZE; i++) {
        if (attacking_cards[i].get_rank() != 0) {
            attacking_cards[i] = empty_card;
        }
        
        if (defending_cards[i].get_rank() != 0) {
            defending_cards[i] = empty_card;
        }
    }
}

bool can_user_defend(card& defending_card, uint8_t& game_ind) {
    uint8_t attacking_rank = attacking_cards[game_ind].get_rank();
    wchar_t attacking_suit = attacking_cards[game_ind].get_suit();
    bool is_attacking_card_trump = (attacking_suit == trump_card->get_suit());

    uint8_t defending_rank = defending_card.get_rank();
    wchar_t defending_suit = defending_card.get_suit();
    bool is_defending_card_trump = (defending_suit == trump_card->get_suit());
    bool is_suits_same = (attacking_suit == defending_suit);
    bool is_defending_rank_greater = (defending_rank > attacking_rank);

    if (is_suits_same && is_defending_rank_greater) {
        return true;
    }
    else if (!is_attacking_card_trump && is_defending_card_trump) {
        return true;
    }
    else {
        return false;
    }
}

int main() {
    srand(time(nullptr));
    std::locale::global(std::locale("en_US.UTF-8"));
    std::wcout.imbue(std::locale());
    make_deck();

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

    
    sort_hand(user_hand);
    sort_hand(bot_hand);
    bot_hand_trump_edge = find_trump_edge(bot_hand);
    user_hand_trump_edge = find_trump_edge(user_hand);

    show_all();

    if (first_player_definer()) {
        defending = 0;
        std::wcout << L"User plays first\n";
    }
    else {
        defending = 1;
        std::wcout <<  L"Bot plays first\n";
    }
    
    bool game = true;
    while (game) {
        switch (defending) {
            case 0: {
                if (game_ind == 6) {
                    defending = 1;
                    break;
                }

                if (user_hand.size() == 0) {
                    game = false;
                    break;
                }

                bool can_user_attack = false;
                for (uint8_t i = 0; i < user_hand.size(); i++) {
                    if (can_attack(user_hand[i], game_ind)) {
                        can_user_attack = true;
                        break;
                    }
                }

                if (can_user_attack) {
                    uint16_t number_of_card;
                    do {
                        std::wcout << L"Choose the card from your hand to attack: ";
                        std::wcin >> number_of_card;
                    }
                    while (number_of_card < 0 || number_of_card >= user_hand.size()
                        || !can_attack(user_hand[number_of_card], game_ind));

                    attacking_cards[game_ind] = user_hand[number_of_card];
                    user_hand.erase(user_hand.begin() + number_of_card);
                    user_hand_trump_edge = find_trump_edge(user_hand);

                    show_all();

                    std::this_thread::sleep_for(std::chrono::seconds(1));

                    if (can_bot_defend(game_ind)) {
                        std::wcout << L"Bot beated the card\n";

                        show_all();

                        game_ind++;
                    }
                    else {
                        std::wcout << L"Bot took the card(s)\n";

                        take_cards(bot_hand);

                        fill_hand(user_hand);
                        user_hand_trump_edge = find_trump_edge(user_hand);
                        bot_hand_trump_edge = find_trump_edge(bot_hand);

                        show_all();

                        game_ind = 0;
                    }
                }
                else {
                    defending = 1;
                    game_ind = 0;
                    empty_field();

                    fill_hand(user_hand);
                    fill_hand(bot_hand);
                    user_hand_trump_edge = find_trump_edge(user_hand);
                    bot_hand_trump_edge = find_trump_edge(bot_hand);

                    show_all();
                }
                std::this_thread::sleep_for(std::chrono::seconds(1));
                break;
            }
            case 1: {
                if (bot_hand.size() == 0) {
                    game = false;
                    break;
                }
                
                bool bot_attack = false;
                for (int8_t i = bot_hand.size() - 1; i >= 0; i--) {
                    if (can_attack(bot_hand[i], game_ind)) {
                        bot_attack = true;
                        attacking_cards[game_ind] = bot_hand[i];
                        bot_hand.erase(bot_hand.begin() + i);
                        bot_hand_trump_edge = find_trump_edge(bot_hand);

                        show_all();
                        break;
                    }
                }


                if (bot_attack) {
                    bool user_defend = false;
                    for (uint8_t i = 0; i < user_hand.size(); i++) {
                        if (can_user_defend(user_hand[i], game_ind)) {
                            user_defend = true;
                            break;
                        }
                    }

                    std::this_thread::sleep_for(std::chrono::seconds(1));

                    if (user_defend) {
                        uint16_t number_of_card;
                        do {
                            std::wcout << L"Choose the card from your hand to defend: ";
                            std::wcin >> number_of_card;
                        }
                        while (number_of_card < 0 || number_of_card >= user_hand.size() 
                            || !can_user_defend(user_hand[number_of_card], game_ind));

                        defending_cards[game_ind] = user_hand[number_of_card];
                        user_hand.erase(user_hand.begin() + number_of_card);
                        user_hand_trump_edge = find_trump_edge(user_hand);

                        show_all();

                        game_ind++;
                    }
                    else {
                        std::this_thread::sleep_for(std::chrono::seconds(1));

                        std::wcout << L"User took the card(s)\n";

                        take_cards(user_hand);

                        fill_hand(bot_hand);
                        user_hand_trump_edge = find_trump_edge(user_hand);
                        bot_hand_trump_edge = find_trump_edge(bot_hand);

                        show_all();

                        game_ind = 0;
                    }                    
                }
                else {
                    defending = 0;
                    game_ind = 0;
                    empty_field();

                    fill_hand(user_hand);
                    fill_hand(bot_hand);
                    user_hand_trump_edge = find_trump_edge(user_hand);
                    bot_hand_trump_edge = find_trump_edge(bot_hand);

                    show_all();
                }
                std::this_thread::sleep_for(std::chrono::seconds(1));
                break;
            }
        }
    }

    uint8_t user_hand_size = user_hand.size();
    uint8_t bot_hand_size = bot_hand.size();
    if (user_hand_size < bot_hand_size) {
        std::wcout << L"User wins!!!\n";
    }
    else if (user_hand_size > bot_hand_size) {
        std::wcout << L"Bot wins!!!\n";
    }
    else {
        std::wcout << L"It\'s draw.\n";
    }

    return 0;
}
