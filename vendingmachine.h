#include "printer.h"                    // Printer class
#include "nameserver.h"                 // NameServer class
#include "watcard.h"                    // WATCard class

#ifndef __VENDINGMACHINE_H__
#define __VENDINGMACHINE_H__

//---------------------------------------------------------
// VendingMachine task definition
//---------------------------------------------------------
_Task VendingMachine {

    Printer& printer;                           // Printer to print to
    NameServer& nameServer;                     // NameServer to register with
    const unsigned int id;                      // Unique machine id
    const unsigned int sodaCost;                // Cost of one soda bottle
    const unsigned int maxStockPerFlavour;      // The maximum stock per flavour
    unsigned int* stock;                        // The amount of each flavour

    void main();

  public:
    enum Flavours { COLA_COCA = 0, DR_SALT = 1,
        BLONDE_ALE = 2, SEVEN_DOWN = 3, NUM_TYPES = 4 };        // Flavours of soda
    _Event Funds {};                                            // Insufficient funds
    _Event Stock {};                                            // Out of stock for particular flavour

    VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
                    unsigned int maxStockPerFlavour );
    ~VendingMachine();
    
    void buy( Flavours flavour, WATCard &card );
    unsigned int *inventory();
    void restocked();
    _Nomutex unsigned int cost();
    _Nomutex unsigned int getId();
};

#endif
