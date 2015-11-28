# Soda Makefile
# Authors: Haseeb Saeed, Mayank Sindwani
# Date: 2015-11-26

# Build variables
BUILD=NODEBUG

# Compiler Options
CXX = u++
CXXFLAGS = -g -I"." -Wall -Wno-unused-label -Wfatal-errors -MMD -D"${BUILD}" -O2 -std=c++11

# Shared objects
OBJS =                 \
	bank.o             \
	bottlingplant.o    \
	config.o           \
	groupoff.o         \
	nameserver.o       \
	parent.o           \
	printer.o          \
	student.o          \
	truck.o            \
	vendingmachine.o   \
	watcard.o          \
	watcardoffice.o    \

# Main Source Files
SODA_OBJS = ${OBJS} main.o
SODA = soda

# Test Source Files
TEST_OBJS = ${OBJS} test/driver.o
TEST = testsoda

.PHONY : clean

# Main Target
${SODA}: ${SODA_OBJS}
	${CXX} ${CXXFLAGS} $^ -o $@

# Test Target
${TEST}: ${TEST_OBJS}
	${CXX} ${CXXFLAGS} $^ -o $@

clean:
	rm -f *.d *.o test/*.d test/*.o ${SODA} ${TEST}
