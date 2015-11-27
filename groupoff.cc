#include "groupoff.h"
#include "watcard.h"
#include <iostream>

using namespace std;

Groupoff::Groupoff( unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay, Printer& printer )
    : numStudents(numStudents),
      numGiftCards(0),
      sodaCost(sodaCost),
      groupoffDelay(groupoffDelay),
      printer (printer)
{

    giftCards = new WATCard::FWATCard[numStudents];
}

Groupoff::~Groupoff() {
    delete[] giftCards;
}

WATCard::FWATCard Groupoff::giftCard() {
    return giftCards[numGiftCards++];
}

void Groupoff::main() {
    WATCard::FWATCard fcard;
    unsigned int i;
    WATCard* card;

    i = 0;
    while (true) {
        _Accept ( ~Groupoff ) {
            break;
        } or _When (numGiftCards < numStudents) _Accept( giftCard ) {

        } _Else {

            if (numStudents == numGiftCards) {

                yield( groupoffDelay );
                card = new WATCard();
                printer.print(Printer::Kind::Groupoff, 'D' , sodaCost);
                card->deposit(sodaCost);

                // TODO: Select at random
                giftCards[i++].delivery(card);

                if (i == numStudents) {
                    break;
                }
            }
        }
    }

}
