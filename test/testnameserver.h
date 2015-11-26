#ifndef __TEST_NAMESERVER__
#define __TEST_NAMESERVER__

#include <printer>
#include <nameserver>
#include <watcard>
#include <vendingmachine>

void testNameServer() {

    _Task serverTest {

        NameServer& server;
        const unsigned int id;

        void main() {

            for ( int i = 0; i < 5; i += 1 )
                osacquire( cout ) << "id " << id << " machine " << server.getMachine( id ) << endl;
        }

      public:
        serverTest( NameServer& server, unsigned int id )
            : server( server ), id( id ) {}
    };


    const int NUM_MACHINES = 3;
    const int NUM_STUDENTS = 5;

    Printer printer( NUM_STUDENTS, NUM_MACHINES, 1 );
    NameServer server( printer, NUM_MACHINES, NUM_STUDENTS );
    WATCard card;

    {
        serverTest t0( server, 0 );
        serverTest t1( server, 1 );
        serverTest t2( server, 2 );
        serverTest t3( server, 3 );
        serverTest t4( server, 4 );

        VendingMachine v0( printer, server, 0, 5, 5 );
        VendingMachine v1( printer, server, 1, 5, 5 );
        VendingMachine v2( printer, server, 2, 5, 5 );

        osacquire(cout) << "v0 " << &v0 << endl;
        osacquire(cout) << "v1 " << &v1 << endl;
        osacquire(cout) << "v2 " << &v2 << endl;

        try {
            _Enable {
                card.deposit( 20 );
                v0.buy( VendingMachine::Flavours::DR_SALT, card )  ;
            }
        } catch ( VendingMachine::Funds f  ) {
            osacquire(cout) << "no funds" << endl;
        } catch ( VendingMachine::Stock s ) {
            osacquire(cout) << "no stock" << endl;
        }

        osacquire(cout) << card.getBalance() << endl;
        auto list = v0.inventory();
        v0.restocked();


        for ( int i = 0; i < VendingMachine::Flavours::NUM_TYPES; i += 1 )
            osacquire(cout) << list[i] << " ";
        osacquire(cout) << endl;
    }
}


#endif