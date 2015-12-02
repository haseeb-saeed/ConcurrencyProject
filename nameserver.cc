#include "nameserver.h"                 // NameServer class
#include "printer.h"                    // Printer class
#include "vendingmachine.h"             // VendingMachine class
#include <assert.h>                     // assert

//-------------------------------------------------------------------
// Constructor for nameserver task
//-------------------------------------------------------------------
NameServer::NameServer( Printer& prt, unsigned int numVendingMachines, unsigned int numStudents )
    : printer( prt ), numVMs( numVendingMachines ), numStudents( numStudents ), numRegistered( 0 ), client( 0 ) {

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
void NameServer::VMregister( VendingMachine* machine ) {

    client = machine->getId();
    machineList[client] = machine;
}

//-------------------------------------------------------------------
// Returns the current student's vending machine and cycles to the next
//-------------------------------------------------------------------
VendingMachine* NameServer::getMachine( unsigned int id ) {

    client = id;
    VendingMachine* result = machineList[studentMachines[id]];

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

            // Indicate the machine registered
            printer.print( Printer::Kind::NameServer, 'R', client );
            
            numRegistered += 1;
            if ( numRegistered == numVMs ) {

                // Once all machines are registered, assign machines to students
                for ( unsigned int i = 0; i < numStudents; i += 1 ) {
                    studentMachines[i] = i % numVMs;
                }
            }
        } or _When( numRegistered == numVMs ) _Accept( getMachine ) {
            
            // Cycle to nect machine
            unsigned int& index = studentMachines[client];
            printer.print( Printer::Kind::NameServer, 'N', client, machineList[index]->getId() );
            index = ( index + 1 ) % numVMs;

        } or _When( numRegistered == numVMs ) _Accept ( getMachineList ) {
        }
    }
    
    // Indicate we have finished
    printer.print( Printer::Kind::NameServer, 'F' );
}
