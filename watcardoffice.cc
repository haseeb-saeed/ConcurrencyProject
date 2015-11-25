#include "watcardoffice.h"
#include "watcard.h"
#include "printer.h"
#include "bank.h"
#include "MPRNG.h"
#include <assert.h>
#include <deque>
#include <iostream>
using namespace std;

// External random number generatori
extern MPRNG mprng;

WATCardOffice::Args::Args( unsigned int sid, unsigned int amount, WATCard* card )
    : sid( sid ), amount( amount ), card( card ) {

}

WATCardOffice::Job::Job( Args args )
    : args( args ) {

}

WATCardOffice::Courier::Courier( WATCardOffice& office, unsigned int id )
    : office( office ), id( id ) {

}

void WATCardOffice::Courier::main() {

    // Indicate the courier has started
    office.printer.print( Printer::Kind::Courier, id, 'S' );
    cout << "courier start " << id << endl;

    for ( ;; ) {

        // Request work
        Job* job = office.requestWork();
      if ( job == nullptr ) break;

        // Get the WATCard
        WATCard* card = job->args.card;
        assert( card != nullptr );

        unsigned int sid = job->args.sid;
        unsigned int amount = job->args.amount;

        // Withdraw the amount from the bank and add it to the card
        office.printer.print( Printer::Kind::Courier, id, 't', sid, amount );
        cout << "courier transfer begin " << id << " " << sid << " " << amount << endl;
        office.bank.withdraw( sid, amount );
        office.printer.print( Printer::Kind::Courier, id, 'T', sid, amount );
        cout << "courier transfer end " << id << " " << sid << " " << amount << endl;
        card->deposit( amount );

        // Did we lose the WATCard?
        if ( mprng( 5 ) == 0 ) {
            delete card;
            job->result.exception( new Lost() );
        } else {
            // Store the card into the future
            job->result.delivery( card );
        }

        // Delete the job since it has been completed
        delete job;
    }

    // Indicate the courier has finished
    office.printer.print( Printer::Kind::Courier, id, 'F' );
    cout << "courier finished " << id << endl;
}

WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers )
    : printer( prt ), bank( bank ), numCouriers( numCouriers ), bench(), jobs(), numWaiting( 0 ) {

    // Create couriers
    // For now, crash if the allocation fails
    couriers = new Courier* [numCouriers];
    assert( couriers != nullptr );

    for ( unsigned int i = 0; i < numCouriers; i += 1 ) {
        couriers[i] = new Courier( *this, i );
        assert( couriers[i] != nullptr );
    }
}

WATCardOffice::~WATCardOffice() {

    // Free allocated memory
    for ( unsigned int i = 0; i < numCouriers; i += 1 ) {
        delete couriers[i];
    }

    delete [] couriers;
}

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {

    // Add a new job to the list of requests
    Job* job = new Job( Args( sid, amount, new WATCard() ) );
    assert( job != nullptr );
    jobs.push_back( job );

    // Indicate the create call is complete
    printer.print( Printer::Kind::WATCardOffice, 'C', sid, amount );
    cout << "office create " << sid << " " << amount << " " << endl;    

    // Return the future value
	return job->result;
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard* card ) {

	// Add a new job to the list of requests
    Job* job = new Job( Args( sid, amount, card ) );
    assert( job != nullptr );
    jobs.push_back( job );

    // Indicate the transfer call is complete
    printer.print( Printer::Kind::WATCardOffice, 'T', sid, amount );
    cout << "office transfer " << sid << " " << amount << " " << endl;    

    // Return the future value    
    return job->result;
}

WATCardOffice::Job* WATCardOffice::requestWork() {

    // If there is no work, block
    if ( jobs.empty() ) {
        cout << "No job, I'm going to sleep" << endl;
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
    cout << "office request " << endl;    
    return job;
}

void WATCardOffice::main() {

    // Indicate the office is starting
    printer.print( Printer::Kind::WATCardOffice, 'F' );
    
    for ( ;; ) {

        // Accept destructor once all couriers have completed their jobs
        _When( numWaiting == numCouriers) _Accept( ~WATCardOffice ) {

            cout << "office has been told to finish " << endl;    
            
            // Wake up the blocked couriers
            while ( !bench.empty() ) {
                bench.signalBlock();
             }
            
            // Indicate the office is closing
            printer.print( Printer::Kind::WATCardOffice, 'F' );
            cout << "office finishing " << endl;    
            break;

        } or _Accept( create, transfer ) {

            // If a courier is waiting for a request wake, them
            if ( !bench.empty() ) {
                bench.signalBlock();
            }
        } or _Accept( requestWork ) {

        }
    }
}
