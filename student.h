#include "printer.h"
#include "nameserver.h"
#include "groupoff.h"
#include "watcardoffice.h"

#ifndef __STUDENT_H__
#define __STUDENT_H__

_Task Student {

    Printer& printer;
    NameServer& nameServer;
    WATCardOffice& cardOffice;
    Groupoff& groupoff;
    const unsigned int id;
    const unsigned int maxPurchases;

    void main();

  public:
    Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, Groupoff &groupoff,
             unsigned int id, unsigned int maxPurchases );
};

#endif
