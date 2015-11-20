#include "printer.h"

#ifndef __NAMESERVER_H__
#define __NAMESERVER_H__

// Forward declare vending machine
_Task VendingMachine;

_Task NameServer {

    Printer& printer;
    const unsigned int numVMs;
    const unsigned int numStudents;
    VendingMachine** machineList;
    unsigned int* studentMachines;
    unsigned int numRegistered;

    void main();

  public:
    NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents );
    void VMregister( VendingMachine *vendingmachine );
    VendingMachine *getMachine( unsigned int id );
    VendingMachine **getMachineList();
};

#endif
