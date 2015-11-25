#include "student.h"
#include "printer.h"
#include "nameserver.h"
#include "groupoff.h"
#include "watcardoffice.h"
#include "vendingmachine.h"
#include "MPRNG.h"

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

    // Get cards and vending machine location
    WATCard::FWATCard watCard = cardOffice.create( id, 5 );
    WATCard::FWATCard giftCard = groupoff.giftCard();
    VendingMachine* machine = nameServer.getMachine( id );
    unsigned int purchase = 0;

    printer.print( Printer::Kind::Student, id, 'V', machine->getId() );

L1: for ( ;; ) {
    
      if ( purchase == numPurchases ) break L1;
        
        // Yield before buying
        yield( mprng( 1, 10 ) );

        // Attempt to buy a soda
    L2: for ( ;; ) {

            try {
                _Enable {

                     _Select( giftCard ) {

                        // Buy the soda
                        machine->buy( favourite, *giftCard() );
                        printer.print( Printer::Kind::Student, id, 'G', giftCard()->getBalance() );

                        // Reset the gift card to prevent further use
                        giftCard.reset();
                    
                    } or _Select( watCard ) {

                        // Buy the soda
                        machine->buy( favourite, *watCard() );
                        printer.print( Printer::Kind::Student, id, 'B', watCard()->getBalance() );
                    }
                    
                    purchase += 1;
                    break L2;
                }
            } catch ( WATCardOffice::Lost ) {
                // Get a new WATCard, but don't yield
                watCard = cardOffice.create( id, 5 );
                printer.print( Printer::Kind::Student, id, 'L' );
            } catch ( VendingMachine::Funds ) {
                // Add more funds to WATCard
                watCard = cardOffice.transfer( id, machine->cost() + 5, watCard );
                break L2;
            } catch ( VendingMachine::Stock ) {
                // Try a new vending machine
                machine = nameServer.getMachine( id );
                printer.print( Printer::Kind::Student, id, 'V', machine->getId() );
                break L2;
            }
        }
    }

    // TODO: The case of a student who only buys one drink with the gift card
    // Should we just cancel the WATCard?

    // Indicate the student has finished
    printer.print( Printer::Kind::Student, id, 'F' );
}
