#include "watcard.h"
#include "printer.h"
#include "bank.h"
#include <deque>

#ifndef __WATCARDOFFICE_H__
#define __WATCARDOFFICE_H__

_Task WATCardOffice {

    struct Args {

        unsigned int sid;
        unsigned int amount;
        WATCard* card;

        Args( unsigned int sid, unsigned int amount, WATCard* card );
    };

    struct Job {                           // marshalled arguments and return future

        Args args;                         // call arguments

        WATCard::FWATCard result;          // return future
        Job( Args args );
    };

    _Task Courier {

        WATCardOffice& office;
        const unsigned int id;     
        void main();

      public:
        Courier( WATCardOffice& office, unsigned int id );
    };

    Printer& printer;
    Bank& bank;
    const unsigned int numCouriers;
    Courier** couriers;
    uCondition bench;
    std::deque<Job*> jobs;
    unsigned int numWaiting;

    void main();

  public:
    _Event Lost {};                        // lost WATCard

    WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );
    ~WATCardOffice();

    WATCard::FWATCard create( unsigned int sid, unsigned int amount );
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job *requestWork();
};

#endif
