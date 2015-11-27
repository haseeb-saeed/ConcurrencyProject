#include "bottlingplant.h"
#include "printer.h"
#include "nameserver.h"
#include "vendingmachine.h"
#include "truck.h"
#include "MPRNG.h"
#include <assert.h>
#include <iostream>
using namespace std;

// External random number generator
extern MPRNG mprng;

BottlingPlant::BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
    unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments )
    : printer( prt ), nameServer( nameServer ), numVendingMachines( numVendingMachines ), maxShippedPerFlavour( maxShippedPerFlavour ),
        maxStockPerFlavour( maxStockPerFlavour ), timeBetweenShipments( timeBetweenShipments ), shutdown( false ) {

    // Create a buffer for a shipment
    shipment = new unsigned int [VendingMachine::Flavours::NUM_TYPES];


    // For now, crash if memory allocation fails
    assert( shipment != nullptr );
}

BottlingPlant::~BottlingPlant() {

    // Free allocated memory
     delete [] shipment;
}

void BottlingPlant::getShipment( unsigned int cargo[] ) {

    cout << "GET_SHIPMENT CALLED BITCHES" << endl;

    // If shutting down, let the truck know
    if ( shutdown ) {
        throw Shutdown();
    }

    // Pass the shipment to the truck
    for ( unsigned int i = 0; i < VendingMachine::Flavours::NUM_TYPES; i += 1 ) {
        cargo[i] = shipment[i];
    }
}

void BottlingPlant::main() {

    // Indicate we are starting
    //printer.print( Printer::Kind::BottlingPlant, 'S' );
    cout << "plant is starting" << endl;

    try {
        Truck truck( printer, nameServer, *this, numVendingMachines, maxStockPerFlavour );

        for ( ;; ) {

            // Get a coffee from Tom Hortons
            // Because going to Starbuks is for losers ;)
            yield( timeBetweenShipments );

            // Create a shipment
            unsigned int total = 0;

            for ( unsigned int i = 0; i < VendingMachine::Flavours::NUM_TYPES; i += 1 ) {
                shipment[i] = mprng( maxShippedPerFlavour );
                total += shipment[i];
            }

            printer.print( Printer::Kind::BottlingPlant, 'G', total );
            cout << "plant created " << total << " bottles" << endl;

            // If the destructor is called, indicate we are closing
            _Accept( ~BottlingPlant ) {

               cout << "plant destructor called" << endl;
               shutdown = true;

               _Accept (getShipment) {

               }

               break;

            } or _Accept( getShipment ) {

                // Indicate the shipment was taken
                printer.print( Printer::Kind::BottlingPlant, 'P' );
                cout << "plant - shipment taken" << endl;
            }
        }
    } catch ( uMutexFailure::RendezvousFailure ) {
    }

    // Indicate we are done
    printer.print( Printer::Kind::BottlingPlant, 'F' );
    cout << "plant is finishing" << endl;
}
