#include "groupoff.h"           // Groupoff class
#include "watcard.h"            // WATCard class
#include "MPRNG.h"              // MPRNG class
#include <assert.h>             // assert

// External random number generator
extern MPRNG mprng;

//------------------------------------------------------------------
// Constructor for Groupoff
//------------------------------------------------------------------
Groupoff::Groupoff( Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay )
    : printer (prt), numStudents( numStudents ), numGiftCards( 0 ), sodaCost( sodaCost ), groupoffDelay( groupoffDelay ) {

    // Create an array of futures
    giftCards = new WATCard::FWATCard[numStudents];
    assert( giftCards != nullptr );
}

//------------------------------------------------------------------
// Destructor for Groupoff
//------------------------------------------------------------------
Groupoff::~Groupoff() {

    // Free allocated memory
    delete [] giftCards;
}

//------------------------------------------------------------------
// Returns a future giftcard
//------------------------------------------------------------------
WATCard::FWATCard Groupoff::giftCard() {

    return giftCards[numGiftCards];
}

//------------------------------------------------------------------
// Main function for groupoff task
//------------------------------------------------------------------
void Groupoff::main() {

    WATCard::FWATCard fcard;
    unsigned int numUndelivered = numStudents;
    WATCard* card;

    // Indicate we have started
    printer.print( Printer::Kind::Groupoff, 'S' );

    for ( ;; ) {

        _Accept ( ~Groupoff ) {
            break;
        } or _When ( numGiftCards < numStudents ) _Accept( giftCard ) {
            numGiftCards += 1;
        } _Else {

            // If all students have taken a giftcard future, randomly deliver one
            if ( numGiftCards == numStudents ) {

                // Yield before delivery
                yield( groupoffDelay );

                // Create a new gift card
                card = new WATCard();
                assert( card != nullptr );

                printer.print( Printer::Kind::Groupoff, 'D' , sodaCost );
                card->deposit( sodaCost );

                // Delivery a randomly selected card
                numUndelivered -= 1;
                unsigned int index = mprng( numUndelivered );
                giftCards[index].delivery( card );

                // Move delivered gift card to back of list
                std::swap( giftCards[index], giftCards[numUndelivered] );

                // If all cards have been delivered, break out of the loop
              if ( numUndelivered == 0 ) break;
            }
        }
    }

    // Indicate we have finished
    printer.print( Printer::Kind::Groupoff, 'F' );
}
