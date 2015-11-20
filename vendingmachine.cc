#include "vendingmachine.h"
#include "printer.h"
#include "nameserver.h"
#include "watcard.h"

VendingMachine::VendingMachine( Printer& printer, NameServer& nameServer, unsigned int id,
    unsigned int sodaCost, unsigned int maxStockPerFlavour )
    : printer( printer ), nameServer( nameServer ), id( id ), sodaCost( sodaCost ),
        maxStockPerFlavour( maxStockPerFlavour ), sodaTypes( 4 ) {

    // Initially no stock
    stock = new unsigned int [sodaTypes];
    for ( unsigned int i = 0; i < sodaTypes; i += 1 ) {
        stock[i] = 0;
    }
}

void VendingMachine::main() {

    // Register with nameserver
    nameServer.VMregister( this );

}

void VendingMachine::buy( Flavours flavour, WATCard& card ) {

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
