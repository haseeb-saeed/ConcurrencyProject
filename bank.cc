#include "bank.h"
#include <assert.h>

//---------------------------------------------------------------
// Constructor for Bank class
//---------------------------------------------------------------
Bank::Bank( unsigned int numStudents )
    : numStudents( numStudents ) {

    // Set balance to 0 for each student
    accounts = new unsigned int [numStudents];
    for ( unsigned int i = 0; i < numStudents; i += 1 ) {
        accounts[i] = 0;
    }

    // Create a condition bench for each student account
    benches = new uCondition [numStudents];

    // For now, crash if allocation fails
    assert( accounts != nullptr && benches != nullptr );
}

//---------------------------------------------------------------
// Destructor for Bank class
//---------------------------------------------------------------
Bank::~Bank() {
    
    // Free resources
    delete [] accounts;  
    delete [] benches;
}

//---------------------------------------------------------------
// Deposit to an account
//---------------------------------------------------------------
void Bank::deposit( unsigned int id, unsigned int amount ) {

    // Add funds to account
    accounts[id] += amount;

    // If someone was waiting for enough funds and now there is enough money
    // Wake them up
    if ( !benches[id].empty() && benches[id].front() <= accounts[id] ) {
        benches[id].signal();
    }
}

//---------------------------------------------------------------
// Withdraw from an account
//---------------------------------------------------------------
void Bank::withdraw( unsigned int id, unsigned int amount ) {

    // If there is not enough money, block
    if ( amount > accounts[id] ) {
        benches[id].wait( amount );
    }

    // Withdraw funds
    accounts[id] -= amount;
}
