#include "watcard.h"

WATCard::WATCard()
    : balance( 0 ) {

}

WATCard::WATCard( const WATCard& card ) {

    balance = card.balance;
}

WATCard& WATCard::operator=( const WATCard& card ) {

    balance = card.balance;
    return *this;
}

void WATCard::deposit( unsigned int amount ) {

    balance += amount;
}

void WATCard::withdraw( unsigned int amount ) {

    balance -= amount;
}

unsigned int WATCard::getBalance() {

    return balance;
}
