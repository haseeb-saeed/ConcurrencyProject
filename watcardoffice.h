#include "watcard.h"
#include "printer.h"
#include "bank.h""

#ifndef __WATCARDOFFICE_H__
#define __WATCARDOFFICE_H__

_Task WATCardOffice {

    struct Args {
        
        Args(); 
    }

    struct Job {                           // marshalled arguments and return future
        Args args;                         // call arguments (YOU DEFINE "Args")
        WATCard::FWATCard result;          // return future
        Job( Args args );
    };

    _Task Courier {
    
        void main();
        
      public:
        Courier();   
    };

    Printer& printer;
    Bank& bank;
    const unsigned int numCouriers;

    void main();

  public:
    _Event Lost {};                        // lost WATCard

    WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );
    WATCard::FWATCard create( unsigned int sid, unsigned int amount );
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job *requestWork();
};

#endif
