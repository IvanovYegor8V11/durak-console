#include <card.h>

card::card(uint8_t rank, wchar_t suit) : rank(rank), suit(suit) {}

uint8_t card::get_rank() {
    return rank;
}

wchar_t card::get_suit() {
    return suit;
}

void card::show_card() {
    char rank_on_card[2] = {' ', ' '}; // Ensure it's initialized

    if (this->rank < 10) {
        rank_on_card[1] = '0' + this->rank;
    }
    else if (this->rank == 10) {
        rank_on_card[0] = '1';
        rank_on_card[1] = '0';
    }
    else if (this->rank > 10) {
        switch (this->rank) {
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

    std::wcout << L"/¯¯¯¯¯¯¯\\\n";
    std::wcout << L"|       |\n";
    std::wcout << L"|  " << this->suit << rank_on_card[0] << rank_on_card[1] << L"  |\n";
    std::wcout << L"|       |\n";
    std::wcout << L"\\_______/\n";
}