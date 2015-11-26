#include "nameserver.h"
#include "printer.h"

#ifndef __BOTTLINGPLANT_H__
#define __BOTTLINGPLANT_H__

// Forward declare Truck class
_Task Truck;

_Task BottlingPlant {

    Printer& printer;
    Truck* truck;
    const unsigned int maxShippedPerFlavour;
    const unsigned int timeBetweenShipments;
    unsigned int* shipment;
    bool shutdown;
    void main();

  public:
    _Event Shutdown {};                    // shutdown plant

    BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments );
    ~BottlingPlant();

    void getShipment( unsigned int cargo[] );
};

#endif
