#include "printer.h"                // Printer class

#ifndef __NAMESERVER_H__
#define __NAMESERVER_H__

// Forward declare vending machine
_Task VendingMachine;

//---------------------------------------------------------
// NameServer task definition
//---------------------------------------------------------
_Task NameServer {

    Printer& printer;                       // Printer to print to
    const unsigned int numVMs;              // Number of vending machines
    const unsigned int numStudents;         // Number of students
    VendingMachine** machineList;           // List of vending machines
    unsigned int* studentMachines;          // Machines assigned to each student
    unsigned int numRegistered;             // Number of registerd machines
    unsigned int client;                    // Communication variable to store id of client

    void main();

  public:
    NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents );
    ~NameServer();
    
    void VMregister( VendingMachine *vendingmachine );
    VendingMachine *getMachine( unsigned int id );
    VendingMachine **getMachineList();
};

#endif
