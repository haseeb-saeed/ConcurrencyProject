#include "printer.h"
#include "nameserver.h"
#include "bottlingplant.h"

#ifndef __TRUCK_H__
#define __TRUCK_H__

_Task Truck {

    Printer& printer;
    NameServer& nameServer;
    BottlingPlant& plant;
    const unsigned int numVendingMachines,
                       maxStockPerFlavour;

    void main();

  public:
    Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};

#endif
