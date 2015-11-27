#include "watcard.h"            // WATCard class

//-------------------------------------------
// Constructor for WATCard class
//-------------------------------------------
WATCard::WATCard()
    : balance( 0 ) {

}

//-------------------------------------------
// Copy constructor for WATCard class
//-------------------------------------------
WATCard::WATCard( const WATCard& card ) {

    balance = card.balance;
}

//-------------------------------------------
// Assignment operator for WATCard class
//-------------------------------------------
WATCard& WATCard::operator=( const WATCard& card ) {

    balance = card.balance;
    return *this;
}

//-------------------------------------------
// Deposit money into card
//-------------------------------------------
void WATCard::deposit( unsigned int amount ) {

    balance += amount;
}

//-------------------------------------------
// Withdraw money from card
//-------------------------------------------
void WATCard::withdraw( unsigned int amount ) {

    balance -= amount;
}

//-------------------------------------------
// Get balance on card
//-------------------------------------------
unsigned int WATCard::getBalance() {

    return balance;
}
