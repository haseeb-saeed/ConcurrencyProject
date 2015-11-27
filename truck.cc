#include "truck.h"
#include "printer.h"
#include "nameserver.h"
#include "bottlingplant.h"
#include "MPRNG.h"
#include <iostream>

using namespace std;

extern MPRNG mprng;

Truck::Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant, unsigned int numVendingMachines, unsigned int maxStockPerFlavour ) :
    printer( prt ),
    nameServer( nameServer ),
    plant( plant ),
    nextVM(0),
    numVendingMachines( numVendingMachines ),
    maxStockPerFlavour( maxStockPerFlavour )
{

}

unsigned int Truck::GetNextMachine() {
    unsigned int index = nextVM;
    nextVM = (nextVM + 1) % numVendingMachines;
    return index;
}

void Truck::main() {
    unsigned int cargo[VendingMachine::Flavours::NUM_TYPES];
    unsigned int currVM, i, j, stock, quantity;
    VendingMachine** machines;
    unsigned int* inventory;

    machines = nameServer.getMachineList();

    try {
        _Enable {
            for ( ;; ) {
                yield(mprng(1, 10));
                plant.getShipment(cargo);

                currVM = GetNextMachine();

                for ( i = 0; i < numVendingMachines; i += 1) {

                    cout << " NEXT MACHINE " << currVM << endl;

                    inventory = machines[currVM]->inventory();
                    stock = 0;

                    for (j = 0; j < VendingMachine::Flavours::NUM_TYPES; j += 1) {

                        quantity = std::min(maxStockPerFlavour - inventory[j], cargo[j]);

                        inventory[j] += quantity;
                        cargo[j] -= quantity;

                        stock += cargo[j];
                    }

                    machines[currVM]->restocked();

                    if (stock == 0) break;
                    currVM = GetNextMachine();
                }
            }
        }
    } catch(BottlingPlant::Shutdown) {
    }
}