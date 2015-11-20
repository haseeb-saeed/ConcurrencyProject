#include "truck.h"
#include "printer.h"
#include "nameserver.h"
#include "bottlingplant.h"

Truck::Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour ) : printer( prt ), nameServer( nameServer ), plant( plant ), numVendingMachines( numVendingMachines ), maxStockPerFlavour( maxStockPerFlavour )  {

}

void Truck::main() {


}
