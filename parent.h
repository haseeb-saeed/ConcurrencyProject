#include "printer.h"                // Printer class
#include "bank.h"                   // Bank class

#ifndef __PARENT_H__
#define __PARENT_H__

//--------------------------------------------------------------
// Parent task definition
//--------------------------------------------------------------
_Task Parent {

    Printer& printer;                       // Printer to print to
    Bank& bank;                             // Bank to deposit to
    const unsigned int numStudents;         // Number of students
    const unsigned int parentalDelay;       // Delay time

    void main();

  public:
    Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay );
};

#endif
