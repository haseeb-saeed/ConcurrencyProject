#include "vendingmachine.h"
#include "printer.h"
#include "nameserver.h"
#include "watcard.h"

VendingMachine::VendingMachine( Printer& printer, NameServer& nameServer, unsigned int id,
    unsigned int sodaCost, unsigned int maxStockPerFlavour )
    : printer( printer ), nameServer( nameServer ), id( id ), sodaCost( sodaCost ),
        maxStockPerFlavour( maxStockPerFlavour ) {

    // Initially no stock
    unsigned int numTypes = Flavours::NUM_TYPES;
    stock = new unsigned int [numTypes];

    for ( unsigned int i = 0; i < numTypes; i += 1 ) {
        stock[i] = 0;
    }
}

VendingMachine::~VendingMachine() {

    // Free resources
    delete [] stock;    
}

void VendingMachine::main() {

    // Register with nameserver
    printer.print( Printer::Kind::Vending, 'S', sodaCost );
    nameServer.VMregister( this ); 

    for ( ;; ) {
    
        // Allow nonlocal exceptions
        try {
            _Enable { 

               _Accept( ~VendingMachine ) {
                        
                    printer.print( Printer::Kind::Vending, 'F' );
                    break;    
                } or _Accept( buy ) {
                
                } or _Accept( inventory ) {
                   
                    printer.print( Printer::Kind::Vending, 'r' );
                   
                    // Wait until restocking is finished 
                    _Accept( restocked ) {
                        printer.print( Printer::Kind::Vending, 'R' );
                    } 
                } 
            }
        } catch ( uMutexFailure::RendezvousFailure ) {
            
        }
    }
}

void VendingMachine::buy( Flavours flavour, WATCard& card ) {

    unsigned int index = flavour;
    unsigned int balance = card.getBalance();
       
    if ( sodaCost > balance ) {
        // Not enough money for purchase
        _Throw Funds(); 
    } else if ( stock[index] == 0 ) {
        // Flavour is sold out
        _Throw Stock();    
    } 

    // Make transaction
    stock[index] -= 1;
    card.withdraw( sodaCost );
    printer.print( Printer::Kind::Vending, 'B', index, stock[index] );
}

unsigned int* VendingMachine::inventory() {

    return stock;
}

void VendingMachine::restocked() {

}

_Nomutex unsigned int VendingMachine::cost() {

    return sodaCost;
}

_Nomutex unsigned int VendingMachine::getId() {

    return id;
}
