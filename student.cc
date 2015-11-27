#include "student.h"                        // Student class
#include "printer.h"                        // Printer class
#include "nameserver.h"                     // NameServer class
#include "groupoff.h"                       // Groupoff class
#include "watcardoffice.h"                  // WATCardOffice class
#include "vendingmachine.h"                 // VendingMachine class
#include "MPRNG.h"                          // MPRNG class
#include <iostream>
using namespace std;

// External random number generator
extern MPRNG mprng;

//--------------------------------------------------------------
// Student task constructor
//--------------------------------------------------------------
Student::Student( Printer &printer, NameServer &nameServer, WATCardOffice &cardOffice, Groupoff &groupoff,
    unsigned int id, unsigned int maxPurchases )
    : printer( printer ), nameServer( nameServer ), cardOffice( cardOffice ), groupoff ( groupoff ),
        id( id ), maxPurchases( maxPurchases ) {

}

//--------------------------------------------------------------
// Student task main function
//--------------------------------------------------------------
void Student::main() {

    // Choose a favourite and purchase amount
    unsigned int numPurchases = mprng( 1, maxPurchases );
    VendingMachine::Flavours favourite = static_cast<VendingMachine::Flavours>( mprng( 3 ) );

    // Indicate the student has started
    printer.print( Printer::Kind::Student, id, 'S', favourite, numPurchases );
    cout << "student " << id << " starting" << endl;

    // Get cards and vending machine location
    WATCard::FWATCard watCard = cardOffice.create( id, 5 );
    WATCard::FWATCard giftCard = groupoff.giftCard();
    VendingMachine* machine = nameServer.getMachine( id );
    unsigned int purchase = 0;
    bool usedGiftCard = false;

    printer.print( Printer::Kind::Student, id, 'V', machine->getId() );
    cout << "student " << id << " machine " << machine->getId() << endl;

L1: for ( ;; ) {

      if ( purchase == numPurchases ) break L1;

        // Yield before buying
        cout << "student " << id << " yielding" << endl;
        yield( mprng( 1, 10 ) );

        // Attempt to buy a soda
    L2: for ( ;; ) {

            try {
                _Enable {

                     _Select( giftCard ) {

                        // Buy the soda using the giftcard
                        cout << "student " << id << " buying with gift card " << endl;
                        machine->buy( favourite, *giftCard() );
                        printer.print( Printer::Kind::Student, id, 'G', giftCard()->getBalance() );
                        cout << "student " << id << " bought with gift card " << endl;

                        // Reset the gift card to prevent further use
                        delete giftCard();
                        giftCard.reset();
                        usedGiftCard = true;

                    } or _Select( watCard ) {

                        // Buy the soda using the watcard
                        cout << "student " << id << " buying with watcard " << endl;
                        machine->buy( favourite, *watCard() );
                        printer.print( Printer::Kind::Student, id, 'B', watCard()->getBalance() );
                        cout << "student " << id << " bought with watcard " << endl;
                    }

                    purchase += 1;
                    break L2;
                }
            } catch ( WATCardOffice::Lost ) {
                // Get a new WATCard, but don't yield
                cout << "student " << id << " lost card" << endl;
                watCard.reset();
                watCard = cardOffice.create( id, 5 );
                printer.print( Printer::Kind::Student, id, 'L' );
            } catch ( VendingMachine::Funds ) {
                // Add more funds to WATCard
                watCard = cardOffice.transfer( id, machine->cost() + 5, watCard );
                cout << "student " << id << " no funds" << endl;
                break L2;
            } catch ( VendingMachine::Stock ) {
                // Try a new vending machine
                machine = nameServer.getMachine( id );
                printer.print( Printer::Kind::Student, id, 'V', machine->getId() );
                cout << "student " << id << " no stock" << endl;
                cout << "student " << id << " machine " << machine->getId() << endl;
                break L2;
            }
        }
    }

    // If the giftcard hasn't been used, free its memory
    if ( !usedGiftCard ) {
        delete giftCard();
    }

    // Delete the watcard even if it got lost
    try {
        delete watCard();
    } catch ( WATCardOffice::Lost ) {
        watCard.reset();
    }

    // Indicate the student has finished
    printer.print( Printer::Kind::Student, id, 'F' );
    cout << "student " << id << " finished" << endl;
}
