#include "watcardoffice.h"
#include "watcard.h"
#include "printer.h"
#include "bank.h"
#include "MPRNG.h"
#include <assert.h>
#include <deque>
#include <iostream>
using namespace std;

// External random number generator
extern MPRNG mprng;

//---------------------------------------------------------------------
// Constructor for Args struct
//---------------------------------------------------------------------
WATCardOffice::Args::Args( unsigned int sid, unsigned int amount, WATCard* card )
    : sid( sid ), amount( amount ), card( card ) {

}

//---------------------------------------------------------------------
// Constructor for Jobs struct
//---------------------------------------------------------------------
WATCardOffice::Job::Job( Args args )
    : args( args ) {

}

//---------------------------------------------------------------------
// Constructor for Courier task
//---------------------------------------------------------------------
WATCardOffice::Courier::Courier( WATCardOffice& office, unsigned int id )
    : office( office ), id( id ) {

}

//---------------------------------------------------------------------
// Main function for Courier task
//---------------------------------------------------------------------
void WATCardOffice::Courier::main() {

    // Indicate the courier has started
    office.printer.print( Printer::Kind::Courier, id, 'S' );

    for ( ;; ) {

        // Request work
        Job* job = office.requestWork();

      // Null pointer indicates the office has closed
      if ( job == nullptr ) break;

        // Get the WATCard
        WATCard* card = job->args.card;
        assert( card != nullptr );

        unsigned int sid = job->args.sid;
        unsigned int amount = job->args.amount;

        // Withdraw the amount from the bank and add it to the card
        office.printer.print( Printer::Kind::Courier, id, 't', sid, amount );
        office.bank.withdraw( sid, amount );
        office.printer.print( Printer::Kind::Courier, id, 'T', sid, amount );
        card->deposit( amount );

        // Did we lose the WATCard?
        if ( mprng( 5 ) == 0 ) {
            delete card;
            job->result.exception( new Lost() );
            cout << "courier " << id << " lost card" << endl;
        } else {
            // Store the card into the future
            cout << "courier " << id << " delivering card" << endl;
            job->result.delivery( card );
        }

        // Delete the job since it has been completed
        delete job;
    }

    // Indicate the courier has finished
    office.printer.print( Printer::Kind::Courier, id, 'F' );
}


//---------------------------------------------------------------------
// Constructor for WATCardOffice task
//---------------------------------------------------------------------
WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers )
    : printer( prt ), bank( bank ), numCouriers( numCouriers ), bench(), jobs(), numWaiting( 0 ) {

    
}

//---------------------------------------------------------------------
// Creates a new WATCard
//---------------------------------------------------------------------
WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {

    // Add a new job to the list of requests
    Job* job = new Job( Args( sid, amount, new WATCard() ) );
    assert( job != nullptr );
    jobs.push_back( job );

    // Indicate the create call is complete
    printer.print( Printer::Kind::WATCardOffice, 'C', sid, amount );

    // Return the future value
	return job->result;
}

//---------------------------------------------------------------------
// Transfers money onto WATCard
//---------------------------------------------------------------------
WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard* card ) {

	// Add a new job to the list of requests
    Job* job = new Job( Args( sid, amount, card ) );
    assert( job != nullptr );
    jobs.push_back( job );

    // Indicate the transfer call is complete
    printer.print( Printer::Kind::WATCardOffice, 'T', sid, amount );

    // Return the future value
    return job->result;
}

//---------------------------------------------------------------------
// Returns a job for the courier to do (or NULL if the office has closed)
//---------------------------------------------------------------------
WATCardOffice::Job* WATCardOffice::requestWork() {

    // If there is no work, block
    if ( jobs.empty() ) {
        numWaiting += 1;
        bench.wait();
        numWaiting -= 1;
     }

    // nullptr means office is closing
    if ( jobs.empty() ) {
        return nullptr;
    }

    // Get the job at the front of the list
    Job* job = jobs.front();
    jobs.pop_front();

    // Indicate the request call is complete
    printer.print( Printer::Kind::WATCardOffice, 'W' );
    return job;
}

//---------------------------------------------------------------------
// Main function for WATCardOffice task
//---------------------------------------------------------------------
void WATCardOffice::main() {

    // Indicate the office is starting
    printer.print( Printer::Kind::WATCardOffice, 'S' );

    // Create couriers
    Courier* couriers [numCouriers];

    for ( unsigned int i = 0; i < numCouriers; i += 1 ) {
        couriers[i] = new Courier( *this, i );
        assert( couriers[i] != nullptr );
    }

    for ( ;; ) {

        // Accept destructor once all couriers have completed their jobs
        _When( numWaiting == numCouriers) _Accept( ~WATCardOffice ) {

            // Wake up the blocked couriers
            while ( !bench.empty() ) {
                bench.signalBlock();
            }

            break;

        } or _Accept( create, transfer ) {

            // If a courier is waiting for a request wake, them
            if ( !bench.empty() ) {
                bench.signalBlock();
            }
        } or _Accept( requestWork ) {

        }
    }

    // Wait for the couriers to finish
    for ( unsigned int i = 0; i < numCouriers; i += 1 ) {
        delete couriers[i];
    }

    // Indicate the office is closing
    printer.print( Printer::Kind::WATCardOffice, 'F' );
}
