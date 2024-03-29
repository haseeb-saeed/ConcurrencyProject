#include "watcardoffice.h"
#include "watcard.h"
#include "printer.h"
#include "bank.h"
#include "MPRNG.h"
#include <assert.h>
#include <deque>

// External random number generator
extern MPRNG mprng;

//---------------------------------------------------------------------
// Constructor for Args struct
//---------------------------------------------------------------------
WATCardOffice::Args::Args( unsigned int sid, unsigned int amount, WATCard* card, char type )
    : sid( sid ), amount( amount ), card( card ), type( type ) {

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
        } else {
            // Store the card into the future
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
    : printer( prt ), bank( bank ), numCouriers( numCouriers ), bench(), jobs(), numWaiting( 0 ),
        request( nullptr ) {

    
}

//---------------------------------------------------------------------
// Creates a new WATCard
//---------------------------------------------------------------------
WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {

    request = new Job( Args( sid, amount, new WATCard(), 'C' ) );
    assert( request != nullptr );
    return request->result;
}
    
//---------------------------------------------------------------------
// Transfers money onto WATCard
//---------------------------------------------------------------------
WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard* card ) {

    // Create a new request
    request = new Job( Args( sid, amount, card, 'T' ) );
    assert( request != nullptr );
    return request->result;
}

//---------------------------------------------------------------------
// Returns a job for the courier to do (or NULL if the office has closed)
//---------------------------------------------------------------------
WATCardOffice::Job* WATCardOffice::requestWork() {

    // Block to get a job
    Job* job = nullptr;
    bench.wait( reinterpret_cast<uintptr_t>( &job ) );
    return job;
}

//---------------------------------------------------------------------
// Assigns a request to the courier at the front of the queue 
// Unblocks the courier afterwards
//---------------------------------------------------------------------
void WATCardOffice::assignRequest() {
    
    // If a courier is waiting for a request wake and there is one, them
    if ( !bench.empty() && !jobs.empty() ) {
    
        // Get the job at the front of the list
        Job* job = jobs.front();
        jobs.pop_front();

        // Give the courier the job
        Job** courierJob = reinterpret_cast<Job**>( bench.front() );  
        *courierJob = job;

        // Wake them up
        wakeCourier();
    
        // If there was a job request, print it
        if ( job != nullptr ) {
            printer.print( Printer::Kind::WATCardOffice, 'W' );
        }
    }
}

//---------------------------------------------------------------------
// Wakes a blocked courier
//---------------------------------------------------------------------
void WATCardOffice::wakeCourier() {
    
    numWaiting -= 1;
    bench.signalBlock();
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
                wakeCourier();
            }

            break;

        } or _Accept( create, transfer ) {

            // Indicate the transfer call is complete
            Args args = request->args;
            printer.print( Printer::Kind::WATCardOffice, args.type, args.sid, args.amount );

            // Add the request to the list
            jobs.push_back( request );

            // Assign a request to a courier
            assignRequest();

        } or _Accept( requestWork ) {

            // Update the number of waiting couriers            
            numWaiting += 1;

            // Assign a request if possible
            assignRequest();
        }
    }

    // Wait for the couriers to finish
    for ( unsigned int i = 0; i < numCouriers; i += 1 ) {
        delete couriers[i];
    }

    // Indicate the office is closing
    printer.print( Printer::Kind::WATCardOffice, 'F' );
}
