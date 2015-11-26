#ifndef __TEST_BANK__
#define __TEST_BANK__

#include <bank.h>                   // Bank class
#include <printer.h>                // Printer class
#include <parent.h>                 // Parent class

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

#endif