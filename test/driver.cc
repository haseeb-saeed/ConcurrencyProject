#include <iostream>                 // C++ std I/O
#include <sstream>                  // stringstream class
#include "../MPRNG.h"                  // MPRNG class
#include "testbank.h"
#include "testnameserver.h"
#include "../bottlingplant.h"          // BottlingPlant class
#include "../config.h"                 // ConfigParms class
#include "../groupoff.h"               // Groupoff class
#include "../student.h"                // Student class
#include "../watcardoffice.h"          // WATCardOffice class
#include <unistd.h>                 // getpid
using namespace std;                // direct access to std
#include <cstdlib>                  // exit

// Random number generator
MPRNG mprng;

typedef struct _test {

    const char* name;
    void(*fn)(void);

} Test;

static const Test TEST_DIRECTORY[] = {
    { "Test Bank", &testBank }
};

void uMain::main() {
    Test test;
    unsigned int i;

    for (i = 0; i < 1; i++) {
        test = TEST_DIRECTORY[i];
        cout << "Running Test " << i + 1 << " : " << test.name << endl;
        test.fn();

        cout << endl;
        cout << "============" << endl;
        cout << "Test Passed!" << endl;
        cout << endl;
    }
}

/*


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
}*/
