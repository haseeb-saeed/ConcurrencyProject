#include <iostream>
#include "printer.h"
using namespace std;

static const unsigned int
CONST_HEADER_SIZE = 6;

static const char*
CONST_PRINTER_HEADERS[] = {
	"Parent",
	"Gropoff",
	"WATOff",
	"Names",
	"Truck",
	"Plant"
};

Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers ) {

	unsigned int i;

	bufferSize = numStudents + numVendingMachines + numCouriers + CONST_HEADER_SIZE;
        buffer = new struct data[bufferSize];

        for (i = 0; i < numStudents; i++) {
	   buffer[i].active = false;
        }

        for (i = 0; i < CONST_HEADER_SIZE; i++) {
	   cout << CONST_PRINTER_HEADERS[i] << "\t";
        }

	for (i = 0; i < numStudents; i++) {
	   cout << "Stud" << i << "\t";
	}

        for (i = 0; i < numVendingMachines; i++) {
           cout << "Mach" << i << "\t";
        }

        for (i = 0; i < numCouriers; i++) {
	   cout << "Cour" << i << "\t";
        }

        for (i = 0; i < bufferSize; i++) {
           cout << "******\t";
        }

        cout << endl;
}

Printer::~Printer() {
        delete[] buffer;
}

unsigned int Printer::getKindIndex( Kind kind ) {
	/*switch (kind) {
		// TODO
	}*/

	return 0;
}

void Printer::_print( Kind kind, unsigned int lid, char state, int value1, int value2 ) {
	if (buffer[lid].active) {
		// TODO: Flush
        }

	buffer[lid].active = true;
	buffer[lid].kind = kind;
	buffer[lid].state = state;
	buffer[lid].value1 = value1;
	buffer[lid].value2 = value2;
}

void Printer::print( Kind kind, char state ) {
	_print(kind, getKindIndex(kind), state);
}

void Printer::print( Kind kind, char state, int value1 ) {
	_print(kind, getKindIndex(kind), state, value1);
}

void Printer::print( Kind kind, char state, int value1, int value2 ) {
	_print(kind, getKindIndex(kind), state, value1, value2);
}

void Printer::print( Kind kind, unsigned int lid, char state ) {
	_print(kind, lid, state);
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1 ) {
	_print(kind, lid, state, value1);
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1, int value2 ) {
	_print(kind, lid, state, value1, value2);
}
