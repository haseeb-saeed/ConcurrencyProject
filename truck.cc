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

    // Create a cargo
    unsigned int cargo[VendingMachine::Flavours::NUM_TYPES];
    unsigned int currVM, i, j, stock, quantity;
    VendingMachine** machines;
    unsigned int* inventory;

    machines = nameServer.getMachineList();

    try {
        _Enable {
            for ( ;; ) {

                // Yield before stoking a machine
                yield(mprng(1, 10));
                plant.getShipment(cargo);

                currVM = GetNextMachine();

                for ( i = 0; i < numVendingMachines; i += 1) {

                    cout << " NEXT MACHINE " << currVM << endl;

                    // Indicate we are stocking the machine
                    inventory = machines[currVM]->inventory();
                    stock = 0;

                    for (j = 0; j < VendingMachine::Flavours::NUM_TYPES; j += 1) {

                        quantity = std::min(maxStockPerFlavour - inventory[j], cargo[j]);

                        inventory[j] += quantity;
                        cargo[j] -= quantity;

                        stock += cargo[j];
                    }

                    // Indicate the machine was restocked
                    machines[currVM]->restocked();

                  // If all stock is depleted, we are done  
                  if (stock == 0) break;
                    currVM = GetNextMachine();
                }
            }
        }
    } catch(BottlingPlant::Shutdown) {
    }
}
