#include "watcard.h"                // WATCard class
#include "printer.h"                // Printer class
#include "bank.h"                   // Bank class
#include <deque>                    // deque class

#ifndef __WATCARDOFFICE_H__
#define __WATCARDOFFICE_H__

//-----------------------------------------------------------------------
// Definition for WATCardOffice task
//-----------------------------------------------------------------------
_Task WATCardOffice {

    // Arguments for a job
    struct Args {

        unsigned int sid;               // Student id
        unsigned int amount;            // Amount to withdraw
        WATCard* card;                  // WATCard to deposit to
        char type;             // Request type

        Args( unsigned int sid, unsigned int amount, WATCard* card, char type );
    };

    // Job for a courier to execute
    struct Job {                          

        Args args;                      // Call arguments
        WATCard::FWATCard result;       // Future to return

        Job( Args args );
    };

    // Courier worker task
    _Task Courier {

        WATCardOffice& office;          // Office the courier belongs to
        const unsigned int id;          // Unique courier id

        void main();

      public:
        Courier( WATCardOffice& office, unsigned int id );
    };

    Printer& printer;                   // Printer to print to
    Bank& bank;                         // Bank for courier to withdraw from
    const unsigned int numCouriers;     // Number of couriers
    uCondition bench;                   // Bench for couriers to wait on for a job
    std::deque<Job*> jobs;              // Queue for jobs
    unsigned int numWaiting;            // Number of couriers waiting for a job
    Job* request;                       // Communication variable to store a job request

    void assignRequest();              
    void wakeCourier();
    void main();

  public:
    _Event Lost {};                     // Lost WATCard

    WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );

    WATCard::FWATCard create( unsigned int sid, unsigned int amount );
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job *requestWork();
};

#endif
