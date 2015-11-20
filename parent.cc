#include "parent.h"
#include "printer.h"
#include "bank.h"

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
Parent::main() {

    printer.print( Printer::Kind::Parent, 'S' );

    for ( ;; ) {

        // Check to see if the destructor has been called
        _Accept( ~Parent ) {

            printer.print( Printer::Kind::Parent, 'F' );
            break;
        } _Else {

            // Yield and give a student some money
            yield( parentalDelay );
            unsigned int student = mprng( numStudents - 1 );
            unsigned int amount = mprng( 1, 3 );

            printer.print( Printer::Kind::Parent, 'D', student, amount );
            bank.deposit( students, amount );
        }
    }
}