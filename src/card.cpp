#include <card.h>

card::card() : rank(0), suit(' ') {}

card::card(uint8_t rank, wchar_t suit) : rank(rank), suit(suit) {}

uint8_t card::get_rank() {
    return rank;
}

wchar_t card::get_suit() {
    return suit;
}

std::string card::get_symbols() {
    std::string symbols = "  ";

    if (this->rank < 10) {
        symbols[1] = '0' + this->rank;
    }
    else if (this->rank == 10) {
        symbols[0] = '1';
        symbols[1] = '0';
    }
    else if (this->rank > 10) {
        switch (this->rank) {
            case 11:
                symbols[1] = 'J';
                break;
            case 12:
                symbols[1] = 'Q';
                break;
            case 13:
                symbols[1] = 'K';
                break;
            case 14:
                symbols[1] = 'A';
                break;
            default:
                symbols[0] = 'E';
                symbols[1] = 'R';
                break;
        }
    }

    return symbols;
}

void card::show_card() {
    std::string symbols = this->get_symbols();

    std::wcout << L"/¯¯¯¯¯¯¯\\\n";
    std::wcout << L"|       |\n";
    std::wcout << L"|  " << this->suit << symbols[0] << symbols[1] << L"  |\n";
    std::wcout << L"|       |\n";
    std::wcout << L"\\_______/\n";
}