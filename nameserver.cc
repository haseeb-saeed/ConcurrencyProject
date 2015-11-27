#include "nameserver.h"                 // NameServer class
#include "printer.h"                    // Printer class
#include "vendingmachine.h"             // VendingMachine class
#include <assert.h>                     // assert

//-------------------------------------------------------------------
// Constructor for nameserver task
//-------------------------------------------------------------------
NameServer::NameServer( Printer& prt, unsigned int numVendingMachines, unsigned int numStudents )
    : printer( prt ), numVMs( numVendingMachines ), numStudents( numStudents ), numRegistered( 0 ) {

    // Create lists
    machineList = new VendingMachine* [numVMs];
    studentMachines = new unsigned int [numStudents];

    // For now, crash if memory allocation fails
    assert( machineList != nullptr && studentMachines != nullptr );
}

//-------------------------------------------------------------------
// Destructor for nameserver task
//-------------------------------------------------------------------
NameServer::~NameServer() {
    
    // Free resources
    delete [] machineList;
    delete [] studentMachines;   
}

//-------------------------------------------------------------------
// Registers a vending machine with the nameserver
//-------------------------------------------------------------------
void NameServer::VMregister( VendingMachine* vendingmachine ) {

    machineList[vendingmachine->getId()] = vendingmachine;
    printer.print( Printer::Kind::NameServer, 'R', vendingmachine->getId() );
}

//-------------------------------------------------------------------
// Returns the current student's vending machine and cycles to the next
//-------------------------------------------------------------------
VendingMachine* NameServer::getMachine( unsigned int id ) {

    unsigned int index = studentMachines[id];
    studentMachines[id] = ( index + 1 ) % numVMs;

    VendingMachine* result = machineList[index];
    printer.print( Printer::Kind::NameServer, 'N', id, result->getId() );
    return result;
}

//-------------------------------------------------------------------
// Returns the vending machine list
//-------------------------------------------------------------------
VendingMachine** NameServer::getMachineList() {

    return machineList;
}

//-------------------------------------------------------------------
// Main function for NameServer task
//-------------------------------------------------------------------
void NameServer::main() {

    // Indicate we have started
    printer.print( Printer::Kind::NameServer, 'S' );

    for ( ;; ) {

        _Accept( ~NameServer ) {
            break;
        } or _When( numRegistered < numVMs ) _Accept( VMregister ) {

            numRegistered += 1;
            if ( numRegistered == numVMs ) {

                // Once all machines are registered, assign machines to students
                for ( unsigned int i = 0; i < numStudents; i += 1 ) {
                    studentMachines[i] = i % numVMs;
                }
            }
        } or _When( numRegistered == numVMs ) _Accept( getMachine, getMachineList ) {
        }
    }
    
    // Indicate we have finished
    printer.print( Printer::Kind::NameServer, 'F' );
}
