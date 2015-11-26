#include <iostream>
#include "printer.h"

using namespace std;

enum CONST_FIXED_HEADERS {
    PARENT   = 0,
    GROUPOFF,
    WATOFF,
    NAMES,
    TRUCK,
    PLANT,
    FIXED_HEADER_COUNT
};

static const char*
CONST_STR_PRINTER_HEADERS[] = {
	"Parent",
	"Gropoff",
	"WATOff",
	"Names",
	"Truck",
	"Plant"
};

Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers ) {

	unsigned int i;

	bufferSize = numStudents +
                 numVendingMachines +
                 numCouriers +
                 FIXED_HEADER_COUNT;

    buffer = new struct data[bufferSize];

    for (i = 0; i < numStudents; i++) {
        buffer[i].active = false;
    }

    for (i = 0; i < FIXED_HEADER_COUNT; i++) {
        cout << CONST_STR_PRINTER_HEADERS[i] << "\t";
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

    cout << endl;

    for (i = 0; i < bufferSize; i++) {
        cout << "*****\t";
    }

    cout << endl;
}

Printer::~Printer() {
    delete[] buffer;
}

unsigned int Printer::getKindIndex( Kind kind ) {
	switch (kind)
    {
        case Parent:
            return PARENT;
        case Groupoff:
            return GROUPOFF;
        case WATCardOffice:
            return WATOFF;
        case NameServer:
            return NAMES;
        case Truck:
            return TRUCK;
        case BottlingPlant:
            return PLANT;
		default:
            return 0;
	}
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
