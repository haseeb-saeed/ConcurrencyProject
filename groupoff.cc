#include "groupoff.h"
#include "watcard.h"
#include <iostream>

using namespace std;

Groupoff::Groupoff( unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay )
    : numStudents(numStudents),
      numGiftCards(0),
      sodaCost(sodaCost),
      groupoffDelay(groupoffDelay) {

}

WATCard::FWATCard Groupoff::giftCard() {
    WATCard::FWATCard card;
    giftCards.push_back(card);
    return card;
}

void Groupoff::main() {
    WATCard::FWATCard fcard;
    WATCard* card;

    while (true) {
        _Accept ( ~Groupoff ) {
            break;
        } or _When (numGiftCards < numStudents) _Accept( giftCard ) {
            numGiftCards++;
        } _Else {
            if (!giftCards.empty()) {
                yield( groupoffDelay );

                fcard = giftCards.back();
                giftCards.pop_back();

                card = new WATCard();
                card->deposit(sodaCost);
                fcard.delivery(card);
            } else if (numGiftCards == numStudents) {
                break;
            }
        }
    }

}
