#include "student.h"
#include "printer.h"
#include "nameserver.h"
#include "groupoff.h"
#include "watcardoffice.h"

//--------------------------------------------------------------
// Student task constructor
//--------------------------------------------------------------
Student::Student( Printer &printer, NameServer &nameServer, WATCardOffice &cardOffice, Groupoff &groupoff,
    unsigned int id, unsigned int maxPurchases )
    : printer( printer ), nameServer( nameServer ), cardOffice( cardOffice ), groupoff ( groupoff ),
    id( id ), maxPurchases( maxPurchases ) {

}

//--------------------------------------------------------------
// Student task main function
//--------------------------------------------------------------
void Student::main() {

}
