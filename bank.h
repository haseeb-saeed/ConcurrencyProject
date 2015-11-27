#ifndef __BANK_H__
#define __BANK_H__

//-----------------------------------------------------------
// Bank monitor definition
//-----------------------------------------------------------
_Monitor Bank {

    uCondition* benches;                    // Benches to wait on if account doesn't have enough money
    unsigned int* accounts;                 // Bank accounts for each student
    const unsigned int numStudents;         // Number of students with accounts

  public:
    Bank( unsigned int numStudents );
    ~Bank();

    void deposit( unsigned int id, unsigned int amount );
    void withdraw( unsigned int id, unsigned int amount );
};

#endif
