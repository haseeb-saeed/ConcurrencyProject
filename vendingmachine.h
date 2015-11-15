#include "printer.h"
#include "nameserver.h"
#include "watcard.h"

#ifndef __VENDINGMACHINE_H__
#define __VENDINGMACHINE_H__

_Task VendingMachine {

    Printer& printer;
    NameServer& nameServer;
    const unsigned int id;
    const unsigned int sodaCost;
    const unsigned int maxStockPerFlavour;
    const unsigned int sodaTypes;
    unsigned int* stock;

    void main();

  public:
    enum Flavours { COLA_COCA, DR_SALT, BLONDE_ALE, 7_DOWN };   // flavours of soda (YOU DEFINE)
    _Event Funds {};                                            // insufficient funds
    _Event Stock {};                                            // out of stock for particular flavour

    VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
                    unsigned int maxStockPerFlavour );
    void buy( Flavours flavour, WATCard &card );
    unsigned int *inventory();
    void restocked();
    _Nomutex unsigned int cost();
    _Nomutex unsigned int getId();
};

#endif
