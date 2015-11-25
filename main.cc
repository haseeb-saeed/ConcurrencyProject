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

// Random number generator
MPRNG mprng;

void testBank() {
    
    _Task bankTest {
   
        Bank& bank; 
        const unsigned int account;

        void main() {
            bank.withdraw( account, 20 );      
        }    
    
    public:

        bankTest( Bank& bank, unsigned int account )
            : bank( bank ), account( account ) {}
    };
    
    
    const unsigned int STUDENTS = 3;
    Bank bank( STUDENTS );
    Printer printer( STUDENTS, 1, 1 );
    Parent parent( printer, bank, STUDENTS, 5 );
    
    {
        bankTest b1( bank, 0 );     
        bankTest b2( bank, 1 );     
        bankTest b3( bank, 2 );     
        bankTest b4( bank, 0 );     
        bankTest b5( bank, 1 );     
        bankTest b6( bank, 1 );     
    }
    
}

void testNameServer() {

    _Task serverTest {
        
        NameServer& server;
        const unsigned int id;
        
        void main() {
           
            for ( int i = 0; i < 5; i += 1 ) 
                osacquire( cout ) << "id " << id << " machine " << server.getMachine( id ) << endl;   
        }
        
      public:
        serverTest( NameServer& server, unsigned int id ) 
            : server( server ), id( id ) {}
    };


    const int NUM_MACHINES = 3;
    const int NUM_STUDENTS = 5;
        
    Printer printer( NUM_STUDENTS, NUM_MACHINES, 1 );
    NameServer server( printer, NUM_MACHINES, NUM_STUDENTS );    
    WATCard card;

    {
        serverTest t0( server, 0 );
        serverTest t1( server, 1 );
        serverTest t2( server, 2 );
        serverTest t3( server, 3 );
        serverTest t4( server, 4 );
   
        VendingMachine v0( printer, server, 0, 5, 5 ); 
        VendingMachine v1( printer, server, 1, 5, 5 ); 
        VendingMachine v2( printer, server, 2, 5, 5 ); 
    
        osacquire(cout) << "v0 " << &v0 << endl;
        osacquire(cout) << "v1 " << &v1 << endl;
        osacquire(cout) << "v2 " << &v2 << endl;

        try {
            _Enable {
                card.deposit( 20 );
                v0.buy( VendingMachine::Flavours::DR_SALT, card )  ;      
            } 
        } catch ( VendingMachine::Funds f  ) {
            osacquire(cout) << "no funds" << endl;
        } catch ( VendingMachine::Stock s ) {
            osacquire(cout) << "no stock" << endl;
        }
        
        osacquire(cout) << card.getBalance() << endl;
        auto list = v0.inventory();
        v0.restocked();


        for ( int i = 0; i < VendingMachine::Flavours::NUM_TYPES; i += 1 )
            osacquire(cout) << list[i] << " ";
        osacquire(cout) << endl;
    }
}

void testOffice() {

    _Task officeTest {

        void main() {
            
            for ( ;; ) {
                try {

                    _Enable {

                        WATCard::FWATCard card = office.create( id, 5 );
                        cout << id << " balance is " << card()->getBalance() << endl;
   
                        while ( card()->getBalance() < 15 ) {
                            card = office.transfer( id, 5, card() );
                            cout << id << " balance is " << card()->getBalance() << endl;
                        }
                        
                        card.cancel();
                        delete card();
                        break;
                    }
                } catch ( WATCardOffice::Lost ex ) {
                    cout << id << " watcard lost - restart " << endl;
                    continue;
                }       
            }
        }

        WATCardOffice& office;
        const unsigned int id;

      public:
        officeTest( WATCardOffice& office, unsigned int id )
            : office( office ), id( id ) {}

    };

    const int NUM_STUDENTS = 3;
    const int NUM_COURIERS = 7;

    Bank bank( NUM_STUDENTS );
    Printer printer( NUM_STUDENTS, 1, NUM_COURIERS );
    Parent parent( printer, bank, NUM_STUDENTS, 5 );
    WATCardOffice office( printer, bank, NUM_COURIERS );

    officeTest t0( office, 0 );
    officeTest t1( office, 1 );
    officeTest t2( office, 2 );
    //officeTest t3( office, 3 );
    //officeTest t4( office, 4 );
}

//-------------------------------------------------------------------------
// Converts a C-style string to an integer
//-------------------------------------------------------------------------
bool convert( int &val, char *buffer ) {

    std::stringstream ss( buffer );     
    ss >> dec >> val;                  

    // Was the conversion succssful?
    return !ss.fail() &&          
        string( buffer ).find_first_not_of( " ", ss.tellg() ) == string::npos;
}

//-------------------------------------------------------------------------
// Output a usage message to cerr and terminate
//-------------------------------------------------------------------------
void usage( const string& prog ) {

    // Print usage message
    cerr << "Usage: " << prog
    << " [ config-file [ random-seed (> 0) ] ]"
    << endl;
 
    // Terminate with a failure exit code   
    exit( EXIT_FAILURE );              
} 

/*
//-------------------------------------------------------------------------
// Output a memory allocation failure  message to cerr and terminate
//-------------------------------------------------------------------------
void alloc_fail() {

    // Print error message
    cerr << "Memory allocation failed! Aborting." << endl;

     // Terminate with a failure exit code   
    exit( EXIT_FAILURE );            
}
*/

//-------------------------------------------------------------------------
// Entry point for program
//-------------------------------------------------------------------------
void uMain::main() {

    testOffice();
    
/*
    string configFile = "soda.config";
    int seed = getpid();

    // Parse command line arguments
    switch ( argc ) {
      // Seed
      case 3:
        if ( !convert( seed, argv[2] ) || seed <= 0 ) {
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
    processConfigFile( configFile.c_str(), cparms );

    // Create objects for soda delivery
    Printer printer( cparms.numStudents, cparms.numVendingMachines, cparms.numCouriers );
    Bank bank( cparms.numStudents );
    Parent parent( printer, bank, cparms.numStudents, cparms.parentalDelay );
    WATCardOffice office( printer, bank, cparms.numCouriers );
    Groupoff groupoff( cparms.numStudents, cparms.sodaCost, cparms.groupoffDelay );
    NameServer server( printer, cparms.numVendingMachines, cparms.numStudents );
    
    // Create vending machines
    VendingMachine* machines [cparms.numVendingMachines];

    for ( unsigned int i = 0; i < cparms.numVendingMachines; i += 1 ) {
        machines[i] = new VendingMachine( printer, server, i, cparms.sodaCost, cparms.maxStockPerFlavour );
    }

    {    
        // Create bottling plant
        BottlingPlant plant( printer, server, cparms.numVendingMachines, cparms.maxShippedPerFlavour,
            cparms.maxStockPerFlavour, cparms.timeBetweenShipments );

        // Create students
        Student* students [cparms.numStudents];
        for ( unsigned int i = 0; i < cparms.numStudents; i += 1 ) {
            students[i] = new Student( printer, server, office, groupoff, i, cparms.maxPurchases );
        }

        // Wait for students to finish purchases
        for ( unsigned int i = 0; i < cparms.numStudents; i += 1 ) {
            delete students[i];
        }
    }

    // Shut down vending machines
    for ( unsigned int i = 0; i < cparms.numVendingMachines; i += 1 ) {
        delete machines[i];
    }
    */
}
