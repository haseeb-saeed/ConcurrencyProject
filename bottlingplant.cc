#include "bottlingplant.h"                  // BottlingPlant class
#include "printer.h"                        // Printer class
#include "nameserver.h"                     // NameServer class
#include "vendingmachine.h"                 // VendingMachine class
#include "truck.h"                          // Truck class
#include "MPRNG.h"                          // MPRNG class
#include <assert.h>                         // assert
#include <iostream>
using namespace std;

// External random number generator
extern MPRNG mprng;

//---------------------------------------------------------------------
// Constructor for BottlingPlant task
//---------------------------------------------------------------------
BottlingPlant::BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
    unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments )
    : printer( prt ), nameServer( nameServer ), numVendingMachines( numVendingMachines ),
        maxShippedPerFlavour( maxShippedPerFlavour ), maxStockPerFlavour( maxStockPerFlavour ),
        timeBetweenShipments( timeBetweenShipments ), bench(), shutdown( false ) {

    // Create a buffer for a shipment
    shipment = new unsigned int [VendingMachine::Flavours::NUM_TYPES];
    assert( shipment != nullptr );
}

//---------------------------------------------------------------------
// Destructor for BottlingPlant task
//---------------------------------------------------------------------
BottlingPlant::~BottlingPlant() {

    // Free allocated memory
     delete [] shipment;
}

//---------------------------------------------------------------------
// Places shipment into the given cargo array
// Throws Shutdown if the plant is closing
//---------------------------------------------------------------------
void BottlingPlant::getShipment( unsigned int cargo[] ) {

    // If shutting down, let the truck know
    if ( shutdown ) {
        throw Shutdown();
    }
     
    // Block until shipment is copied over
    bench.wait( reinterpret_cast<uintptr_t>( cargo ) );
}

//---------------------------------------------------------------------
// Main function for BottlingPlant task
//---------------------------------------------------------------------
void BottlingPlant::main() {

    // Indicate we are starting
    printer.print( Printer::Kind::BottlingPlant, 'S' );
    //cout << "plant is starting" << endl;

    try {
    
        // Create a truck for delivering bottles
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
            //cout << "plant created " << total << " bottles" << endl;

            // If the destructor is called, indicate we are closing
            _Accept( ~BottlingPlant ) {

                //cout << "plant destructor called" << endl;
                shutdown = true;

                // Wait until the next getShipment so the truck knows to stop
                _Accept (getShipment) {
                    break;
                }

            } or _Accept( getShipment ) {

                if ( !bench.empty() ) {
                    
                    // Get the cargo array
                    unsigned int* cargo = reinterpret_cast<unsigned int*>( bench.front() );
                
                    // Copy the shipment
                    for ( unsigned int i = 0; i < VendingMachine::Flavours::NUM_TYPES; i += 1 ) {
                        cargo[i] = shipment[i];
                    }

                    // Unblock truck and indicate the shipment was taken
                    bench.signalBlock();
                    printer.print( Printer::Kind::BottlingPlant, 'P' );
                }
            }
        }
    } catch ( uMutexFailure::RendezvousFailure ) {
    }

    // Indicate we are done
    printer.print( Printer::Kind::BottlingPlant, 'F' );
    //cout << "plant is finishing" << endl;
}
