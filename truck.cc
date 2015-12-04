#include "truck.h"                      // Truck class
#include "printer.h"                    // Printer class
#include "nameserver.h"                 // NameServer class
#include "bottlingplant.h"              // BottlingPlant class
#include "MPRNG.h"                      // MPRNG class
#include <iostream>

using namespace std;

// External random number generator
extern MPRNG mprng;

//--------------------------------------------------------------------
// Constructor for Truck task
//--------------------------------------------------------------------
Truck::Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant, unsigned int numVendingMachines,
    unsigned int maxStockPerFlavour )
    : printer( prt ), nameServer( nameServer ), plant( plant ), nextVM(0), numVendingMachines( numVendingMachines ),
        maxStockPerFlavour( maxStockPerFlavour ) {

}

//--------------------------------------------------------------------
// Get next vending machine to stock up
//--------------------------------------------------------------------
unsigned int Truck::GetNextMachine() {

    unsigned int index = nextVM;
    nextVM = (nextVM + 1) % numVendingMachines;
    return index;
}

//--------------------------------------------------------------------
// Main function for truck task
//--------------------------------------------------------------------
void Truck::main() {

    // Indicate the truck is starting
    printer.print( Printer::Kind::Truck, 'S' );

    // Create a cargo
    unsigned int cargo[VendingMachine::Flavours::NUM_TYPES];
    unsigned int currVM, i, j, stock, quantity, total;
    VendingMachine** machines;
    unsigned int* inventory;

    machines = nameServer.getMachineList();
    const unsigned int numFlavours = VendingMachine::Flavours::NUM_TYPES; 
    const unsigned int maxBottles = maxStockPerFlavour * numFlavours;

    try {
        _Enable {
            for ( ;; ) {

                // Yield before stocking a machine
                yield(mprng(1, 10));
                plant.getShipment(cargo);

                // Get the total cargo
                stock = 0;
                for ( i = 0; i < numFlavours; i += 1 ) {
                    stock += cargo[i];
                }

                printer.print( Printer::Kind::Truck, 'P', stock );
                currVM = GetNextMachine();

                for ( i = 0; i < numVendingMachines; i += 1) {

                    // Indicate we are stocking the machine
                    inventory = machines[currVM]->inventory();
                    printer.print( Printer::Kind::Truck, 'd', machines[currVM]->getId(), stock );
                    total = 0;

                    // Restock the machine
                    for (j = 0; j < VendingMachine::Flavours::NUM_TYPES; j += 1) {

                        quantity = std::min(maxStockPerFlavour - inventory[j], cargo[j]);

                        inventory[j] += quantity;
                        total += inventory[j];

                        cargo[j] -= quantity;
                        stock -= quantity;
                    }

                    // Was the machine fully restocked?
                    if ( total != maxBottles ) {
                        printer.print( Printer::Kind::Truck, 'U', machines[currVM]->getId(), ( maxBottles - total ) );
                    }

                    // Indicate the machine was restocked
                    machines[currVM]->restocked();
                    printer.print( Printer::Kind::Truck, 'D', machines[currVM]->getId(), stock );

                  // If all stock is depleted, we are done  
                  if (stock == 0) break;
                    currVM = GetNextMachine();
                }
            }
        }
    } catch(BottlingPlant::Shutdown) {
    }

    printer.print( Printer::Kind::Truck, 'F' );
}
