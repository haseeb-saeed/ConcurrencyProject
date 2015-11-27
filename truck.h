#include "printer.h"
#include "nameserver.h"
#include "bottlingplant.h"
#include "vendingmachine.h"

#ifndef __TRUCK_H__
#define __TRUCK_H__

_Task Truck {

    Printer& printer;
    NameServer& nameServer;
    BottlingPlant& plant;
    unsigned int nextVM;
    const unsigned int numVendingMachines,
                       maxStockPerFlavour;

    unsigned int GetNextMachine();
    void main();

  public:
    Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};

#endif
