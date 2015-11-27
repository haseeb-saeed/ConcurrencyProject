#include "printer.h"                // Printer class
#include "nameserver.h"             // NameServer class
#include "groupoff.h"               // Groupoff class
#include "watcardoffice.h"          // WATCardOffice class

#ifndef __STUDENT_H__
#define __STUDENT_H__

//---------------------------------------------------------------
// Student task definition
//---------------------------------------------------------------
_Task Student {

    Printer& printer;                           // Printer to print to
    NameServer& nameServer;                     // Nameserver for vending machine info
    WATCardOffice& cardOffice;                  // Card office for WATCard transactions
    Groupoff& groupoff;                         // Groupoff for getting a gift card
    const unsigned int id;                      // Unique id of student
    const unsigned int maxPurchases;            // Maxium number of purchases allowed

    void main();

  public:
    Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, Groupoff &groupoff,
             unsigned int id, unsigned int maxPurchases );
};

#endif
