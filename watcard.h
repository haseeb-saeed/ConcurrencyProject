#include <uFuture.h>    // Future_ISM class

#ifndef __WATCARD_H__
#define __WATCARD_H__

//--------------------------------------------------------------------
// WATCard class definition
//--------------------------------------------------------------------
class WATCard {

    unsigned int balance;                       // Balance in watcard

    WATCard( const WATCard & );                 // Prevent copying
    WATCard &operator=( const WATCard & );

  public:
    typedef Future_ISM<WATCard *> FWATCard;     // Future watcard pointer

    WATCard();
    
    void deposit( unsigned int amount );
    void withdraw( unsigned int amount );
    unsigned int getBalance();
};

#endif
