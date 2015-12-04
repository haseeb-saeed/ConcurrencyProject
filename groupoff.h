#include "watcard.h"            // WATCard class
#include "printer.h"            // Printer class

#ifndef __GROUPOFF_H__
#define __GROUPOFF_H__

//---------------------------------------------------------------------
// Groupoff task definition
//---------------------------------------------------------------------
_Task Groupoff {

    Printer& printer;                           // Printer to print to
    const unsigned int numStudents;             // Number of students to give cards to
    unsigned int numGiftCards;                  // Number of futures returned
    const unsigned int sodaCost;                // Cost of a single soda
    const unsigned int groupoffDelay;           // Delay time
    WATCard::FWATCard* giftCards;               // Giftcard futures for each student

    void main();

  public:
    Groupoff( Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
    ~Groupoff();

    WATCard::FWATCard giftCard();
};

#endif
