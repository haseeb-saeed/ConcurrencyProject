#include "student.h"
#include "printer.h"
#include "nameserver.h"
#include "groupoff.h"

//--------------------------------------------------------------
// Student task constructor
//--------------------------------------------------------------
Student::Student( Printer &printer, NameServer &nameServer, WATCardOffice &cardOffice, Groupoff &groupoff,
    unsigned int id, unsigned int maxPurchases )
    : printer( printer ), nameserver( nameserver ), cardoffice( cardoffice ), groupoff ( groupoff )
    id( id ), maxPurchases( maxPurchases ) {

}

//--------------------------------------------------------------
// Student task main function
//--------------------------------------------------------------
Student::main() {

}
