#include "vendingmachine.h"                     // VendingMachine class
#include "printer.h"                            // Printer class
#include "nameserver.h"                         // NameServer class
#include "watcard.h"                            // WATCard class
#include <assert.h>                             // assert
#include <iostream>
using namespace std;

//--------------------------------------------------------------
// Constructor for VendingMachine task
//--------------------------------------------------------------
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

//--------------------------------------------------------------
// Destructor for VendingMachine task
//--------------------------------------------------------------
VendingMachine::~VendingMachine() {

    //cout << "destructing vending machine " << id << endl;

    // Free resources
    delete [] stock;    
}

//--------------------------------------------------------------
// Main function for VendingMachine task
//--------------------------------------------------------------
void VendingMachine::main() {

    // Register with nameserver
    printer.print( Printer::Kind::Vending, id, 'S', sodaCost );
    nameServer.VMregister( this ); 

    for ( ;; ) {
    
        // Allow nonlocal exceptions
        try {
            _Enable { 

               _Accept( ~VendingMachine ) {
                    break;    
                } or _Accept( buy ) {
                } or _Accept( inventory ) {
                   
                    printer.print( Printer::Kind::Vending, id, 'r' );
                   
                    // Wait until restocking is finished 
                    _Accept( restocked ) {
                        printer.print( Printer::Kind::Vending, id, 'R' );
                    } 
                } 
            }
        } catch ( uMutexFailure::RendezvousFailure ) {
        }
    }
    
    // Indicate we have finished         
    printer.print( Printer::Kind::Vending, id, 'F' );
}

//--------------------------------------------------------------
// Buys a flavour of soda
//--------------------------------------------------------------
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
    printer.print( Printer::Kind::Vending, id, 'B', index, stock[index] );
}

//--------------------------------------------------------------
// Return the inventory for the machine
//--------------------------------------------------------------
unsigned int* VendingMachine::inventory() {

    return stock;
}

//--------------------------------------------------------------
// Indicate that restocking is complete
//--------------------------------------------------------------
void VendingMachine::restocked() {

}

//--------------------------------------------------------------
// Get the cost of a soda
//--------------------------------------------------------------
_Nomutex unsigned int VendingMachine::cost() {

    return sodaCost;
}

//--------------------------------------------------------------
// Get the machine's ID
//--------------------------------------------------------------
_Nomutex unsigned int VendingMachine::getId() {

    return id;
}
