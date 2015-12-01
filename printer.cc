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

//-------------------------------------------------------------------
// Constructor for printer
//-------------------------------------------------------------------
Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers ) :
    numStudents(numStudents), numVendingMachines(numVendingMachines) {

	unsigned int i;

    // Calculate the buffer size.
	bufferSize = numStudents +
                 numVendingMachines +
                 numCouriers +
                 FIXED_HEADER_COUNT;

    buffer = new struct data[bufferSize];

    // Print the static headers.
    for (i = 0; i < FIXED_HEADER_COUNT; i++) {
        cout << CONST_STR_PRINTER_HEADERS[i] << "\t";
    }

    // Print student headers.
	for (i = 0; i < numStudents; i++) {
        cout << "Stud" << i << "\t";
	}

    // Print machine headers.
    for (i = 0; i < numVendingMachines; i++) {
        cout << "Mach" << i << "\t";
    }

    // Print courier headers.
    for (i = 0; i < numCouriers; i++) {
        cout << "Cour" << i << "\t";
    }

    cout << endl;

    // Print separators.
    for (i = 0; i < bufferSize; i++) {
        buffer[i].active = false;
        cout << "*******\t";
    }

    cout << endl;
}

//-------------------------------------------------------------------
// Destructor for printer
//-------------------------------------------------------------------
Printer::~Printer() {
    // Finish printing.
    flush();
    cout << "***********************" << endl;

    // Delete the buffer.
    delete[] buffer;
}

//-------------------------------------------------------------------
// Flushes the current buffer.
//-------------------------------------------------------------------
void Printer::flush() {
    struct data column;
    int i, maxCol;

    maxCol = -1;

    // Find the largest index where the data is active.
    for (i = 0; i < (int)bufferSize; i++) {
        if (buffer[i].active) {
            maxCol = i;
        }
    }

    // Process the necessary number of columns.
    for (i = 0; i <= maxCol; i++) {
        column = buffer[i];

        // Check to see if there is content to write.
        if (column.active) {

            // Print out column data if it exists.
            cout << column.state;

            // Print values.
            if (column.value1 != -1) {
                cout << column.value1;
            }
            if (column.value2 != -1) {
                cout << "," << column.value2;
            }
        }

        cout << "\t";
        buffer[i].active = false;
    }

    // If anything was printed, add a new line.
    if (maxCol != -1) {
        cout << endl;
    }
}

//-------------------------------------------------------------------
// Maps kinds (with optional offsets) to their buffer indecies.
// offset = 0 by default
//-------------------------------------------------------------------
unsigned int Printer::getKindIndex( Kind kind , unsigned int offset) {
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
        case Student:
            // Start after fixed headers.
            return offset + FIXED_HEADER_COUNT;
        case Vending:
            // Start after student headers.
            return offset + FIXED_HEADER_COUNT + numStudents;
        case Courier:
            // Start after vending machine headers.
            return offset + FIXED_HEADER_COUNT + numStudents + numVendingMachines;
		default:
            return 0;
	}
}

//-------------------------------------------------------------------
// Sets the buffer for any print scenario.
// value1 = -1 by default
// value2 = -1 by default
//-------------------------------------------------------------------
void Printer::setBuffer( Kind kind, unsigned int lid, char state, int value1, int value2 ) {
    unsigned int i;

    if (state == 'F') {
        flush();

        // Print out a finished row.
        for (i = 0; i < bufferSize; i++) {
            if (i == lid) {
                cout << "F" << "\t";
            } else {
                cout << "...\t";
            }
        }

        cout << endl;
    } else {
        // Flush the buffer if the column is active.
        if (buffer[lid].active) {
    	   flush();
        }

        // Update the column.
    	buffer[lid].active = true;
    	buffer[lid].state = state;
    	buffer[lid].value1 = value1;
    	buffer[lid].value2 = value2;
    }
}

//-------------------------------------------------------------------
// Prints data using a buffer mechanism.
//-------------------------------------------------------------------
void Printer::print( Kind kind, char state ) {
    // Sets only the state.
	setBuffer(kind, getKindIndex(kind), state);
}

void Printer::print( Kind kind, char state, int value1 ) {
    // Sets the state and a single value.
	setBuffer(kind, getKindIndex(kind), state, value1);
}

void Printer::print( Kind kind, char state, int value1, int value2 ) {
    // Sets the state and two values.
	setBuffer(kind, getKindIndex(kind), state, value1, value2);
}

void Printer::print( Kind kind, unsigned int lid, char state ) {
    // Sets only the state with index offset.
	setBuffer(kind, getKindIndex(kind, lid), state);
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1 ) {
    // Sets the state and a single value with index offset.
	setBuffer(kind, getKindIndex(kind, lid), state, value1);
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1, int value2 ) {
    // Sets the state and two values with index offset.
	setBuffer(kind, getKindIndex(kind, lid), state, value1, value2);
}
