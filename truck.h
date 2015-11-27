#include "printer.h"                        // Printer class
#include "nameserver.h"                     // NameServer class
#include "bottlingplant.h"                  // BottlingPlant class
#include "vendingmachine.h"                 // Vending machine class

#ifndef __TRUCK_H__
#define __TRUCK_H__

//-------------------------------------------------------
// Truck task definition
//-------------------------------------------------------
_Task Truck {

    Printer& printer;                           // Printer to print to
    NameServer& nameServer;                     // NameServer for machine info
    BottlingPlant& plant;                       // Plant to get shipments from
    unsigned int nextVM;                        // Next machine to stock
    const unsigned int numVendingMachines,      // Number of vending machines
                       maxStockPerFlavour;      // Maximum number of bottles to stock per flavour

    unsigned int GetNextMachine();
    void main();

  public:
    Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};

#endif
