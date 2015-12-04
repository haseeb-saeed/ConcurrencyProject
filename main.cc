#include <iostream>                 // C++ std I/O
#include <sstream>                  // stringstream class
#include "MPRNG.h"                  // MPRNG class
#include "bank.h"                   // Bank class
#include "bottlingplant.h"          // BottlingPlant class
#include "config.h"                 // ConfigParms class
#include "groupoff.h"               // Groupoff class
#include "nameserver.h"             // NameServer class
#include "parent.h"                 // Parent class
#include "printer.h"                // Printer class
#include "student.h"                // Student class
#include "vendingmachine.h"         // VendingMachine class
#include "watcardoffice.h"          // WATCardOffice class
#include <unistd.h>                 // getpid
using namespace std;                // direct access to std
#include <cstdlib>                  // exit

static const char* DEFAULT_CONFIG_FILE = "soda.config";

// Random number generator
MPRNG mprng;

//-------------------------------------------------------------------------
// Output a usage message to cerr and terminate
//-------------------------------------------------------------------------
void usage( const char* prog ) {

    // Print usage message
    cerr << "Usage: " << prog
    << " [ config-file [ random-seed (> 0) ] ]"
    << endl;

    // Terminate with a failure exit code
    exit( EXIT_FAILURE );
}

//-------------------------------------------------------------------------
// Entry point for program
//-------------------------------------------------------------------------
void uMain::main() {

    const char* configFile;
    unsigned int i;
    int seed;

    configFile = DEFAULT_CONFIG_FILE;
    seed = getpid();

    // Parse command line arguments
    switch ( argc ) {
        // Seed
        case 3:
            seed = atoi(argv[2]);
            if (seed <= 0) {
                usage( argv[0] );
            }
        // Config file
        case 2:
            configFile = argv[1];
        // Default usage
        case 1:
            break;
        // Wrong number of arguments
        default:
            usage( argv[0] );
    }

    // Seed the random number generator
    mprng.seed( seed );

    // Parse the configurations
    ConfigParms cparms;
    processConfigFile( configFile, cparms );

    // Create objects for soda delivery
    Printer printer( cparms.numStudents, cparms.numVendingMachines, cparms.numCouriers );
    Bank bank( cparms.numStudents );
    Parent parent( printer, bank, cparms.numStudents, cparms.parentalDelay );
    WATCardOffice office( printer, bank, cparms.numCouriers );
    Groupoff groupoff( printer, cparms.numStudents, cparms.sodaCost, cparms.groupoffDelay );
    NameServer server( printer, cparms.numVendingMachines, cparms.numStudents );

    // Create vending machines
    VendingMachine* machines [cparms.numVendingMachines];

    for ( i = 0; i < cparms.numVendingMachines; i += 1 ) {
        machines[i] = new VendingMachine( printer, server, i, cparms.sodaCost, cparms.maxStockPerFlavour );
    }

    {
        // Create bottling plant
        BottlingPlant plant( printer, server, cparms.numVendingMachines, cparms.maxShippedPerFlavour,
            cparms.maxStockPerFlavour, cparms.timeBetweenShipments );

        // Create students
        Student* students [cparms.numStudents];
        for ( i = 0; i < cparms.numStudents; i += 1 ) {
            students[i] = new Student( printer, server, office, groupoff, i, cparms.maxPurchases );
        }

        // Wait for students to finish purchases
        for ( i = 0; i < cparms.numStudents; i += 1 ) {
            delete students[i];
        }
    }

    // Shut down vending machines
    for ( i = 0; i < cparms.numVendingMachines; i += 1 ) {
        delete machines[i];
    }
}
