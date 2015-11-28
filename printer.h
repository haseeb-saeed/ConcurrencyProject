#ifndef __PRINTER_H__
#define __PRINTER_H__

_Monitor Printer {

  public:
    enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };

    Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
    ~Printer();
    void print( Kind kind, char state );
    void print( Kind kind, char state, int value1 );
    void print( Kind kind, char state, int value1, int value2 );
    void print( Kind kind, unsigned int lid, char state );
    void print( Kind kind, unsigned int lid, char state, int value1 );
    void print( Kind kind, unsigned int lid, char state, int value1, int value2 );

  private:

    struct data {
       int value1, value2;
       bool active;
       char state;
       Kind kind;
    };

    struct data* buffer;
    unsigned int bufferSize, numStudents, numVendingMachines;

    void setBuffer( Kind kind, unsigned int lid, char state, int value1 = -1, int value2 = -1);
    unsigned int getKindIndex( Kind kind , unsigned int offset = 0);
    void flush(char state);
};

#endif
