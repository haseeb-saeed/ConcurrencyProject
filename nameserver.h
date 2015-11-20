#include "printer.h"

#ifndef __NAMESERVER_H__
#define __NAMESERVER_H__

// Forward declare vending machine
_Task VendingMachine;

_Task NameServer {

    void main();

  public:
    NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents );
    void VMregister( VendingMachine *vendingmachine );
    VendingMachine *getMachine( unsigned int id );
    VendingMachine **getMachineList();
};

#endif
