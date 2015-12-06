#include "parent.h"             // Parent class
#include "printer.h"            // Printer class
#include "MPRNG.h"              // MPRNG class
#include "bank.h"               // Bank class

// External mprng
extern MPRNG mprng;

//----------------------------------------------------------------------------
// Constructor for Parent class
//----------------------------------------------------------------------------
Parent::Parent( Printer& printer, Bank& bank, unsigned int numStudents, unsigned int parentalDelay )
    : printer( printer ), bank( bank ), numStudents( numStudents ), parentalDelay( parentalDelay ) {

}

//----------------------------------------------------------------------------
// Main function for Parent class
//----------------------------------------------------------------------------
void Parent::main() {

    // Indicate we have started
    printer.print( Printer::Kind::Parent, 'S' );

    for ( ;; ) {

        // Check to see if the destructor has been called
        _Accept( ~Parent ) {

            break;
        } _Else {

            // Yield and give a student some money
            yield( parentalDelay );
            unsigned int amount = mprng( 1, 3 );
            unsigned int student = mprng( numStudents - 1 );

            printer.print( Printer::Kind::Parent, 'D', student, amount );
            bank.deposit( student, amount );
        }
    }

    // Indicate we have finished
    printer.print( Printer::Kind::Parent, 'F' );
}
