#include "watcard.h"
#include <deque>

#ifndef __GROUPOFF_H__
#define __GROUPOFF_H__

_Task Groupoff {
    unsigned int numStudents, numGiftCards, sodaCost, groupoffDelay;
    std::deque<WATCard::FWATCard> giftCards;
    void main();

  public:
    Groupoff( unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
    WATCard::FWATCard giftCard();
};

#endif
