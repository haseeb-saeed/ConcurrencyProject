#include "printer.h"
#include "bank.h"

#ifndef __PARENT_H__
#define __PARENT_H__

_Task Parent {

    Printer& printer;
    Bank& bank;
    const unsigned int numStudents;
    const unsigned int parentalDelay;

    void main();

  public:
    Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay );
};

#endif
