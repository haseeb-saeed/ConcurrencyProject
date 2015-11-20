#include "watcardoffice.h"
#include "watcard.h"
#include "printer.h"
#include "bank.h"
   
WATCardOffice::Args::Args() {
    
}

WATCardOffice::Job::Job( Args args )
    : args( args ) {
    
}
    
WATCardOffice::Courier::Courier() {
    
}

void WATCardOffice::Courier::main() {
    
}

WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ) {
    : printer( prt ), bank( bank ), numCouriers( numCouriers );
   
}

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
    
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard* card ) {
    
}

WATCardOffice::Job* WATCardOffice::requestWork() {
    
}

void WATCardOffice::main();
