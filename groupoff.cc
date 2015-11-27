#include "groupoff.h"           // Groupoff class
#include "watcard.h"            // WATCard class
#include <assert.h>             // assert
#include <iostream>

using namespace std;

//------------------------------------------------------------------
// Constructor for Groupoff
//------------------------------------------------------------------
Groupoff::Groupoff( unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay, Printer& printer )
    : numStudents( numStudents ), numGiftCards( 0 ), sodaCost( sodaCost ), groupoffDelay( groupoffDelay ),  printer (printer) {

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
 
    return giftCards[numGiftCards++];
}

//------------------------------------------------------------------
// Main function for groupoff task
//------------------------------------------------------------------
void Groupoff::main() {
 
    WATCard::FWATCard fcard;
    unsigned int numDelivered = 0;
    WATCard* card;

    // Indicate we have started
    printer.print( Printer::Kind::Groupoff, 'S' );

    for ( ;; ) {

        _Accept ( ~Groupoff ) {
            break;
        } or _When ( numGiftCards < numStudents ) _Accept( giftCard ) {

        } _Else {

            // If all students have taken a giftcard future, randomly deliver one
            if ( numStudents == numGiftCards ) {

                // Yield before delivery
                yield( groupoffDelay );

                // Create a new gift card 
                card = new WATCard();
                assert( card != nullptr );

                printer.print( Printer::Kind::Groupoff, 'D' , sodaCost );
                card->deposit( sodaCost );

                // TODO: Select at random
                // Delivery a randomly selected card
                giftCards[numDelivered++].delivery( card );

                // If all cards have been delivered, break out of the loop
                if ( numDelivered == numStudents ) {
                    break;
                }
            }
        }
    }

    // Indicate we have finished
    printer.print( Printer::Kind::Groupoff, 'F' );
}
