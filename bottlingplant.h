#include "printer.h"                                // Printer class
#include "nameserver.h"                             // NameServer class

#ifndef __BOTTLINGPLANT_H__
#define __BOTTLINGPLANT_H__

// Forward declare Truck class
_Task Truck;

//----------------------------------------------------------
// BottlinhPlant task definition
//----------------------------------------------------------
_Task BottlingPlant {

    Printer& printer;                               // Printer for printing
    NameServer& nameServer;                         // Server for vending machine info
    const unsigned int numVendingMachines;          // Number of vending machines
    const unsigned int maxShippedPerFlavour;        // Maximum amount to ship for each flavour
    const unsigned int maxStockPerFlavour;          // Maximum amount of each flavour in vending machine
    const unsigned int timeBetweenShipments;        // Delay time between shipments
    unsigned int* shipment;                         // Stores a newly created shipment
    uCondition bench;                               // Bench for truck to sit on
    bool shutdown;                                  // Whether the plant is closing

    void main();

  public:
    _Event Shutdown {};                             // Shutdown plant

    BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments );
    ~BottlingPlant();

    void getShipment( unsigned int cargo[] );
};

#endif
