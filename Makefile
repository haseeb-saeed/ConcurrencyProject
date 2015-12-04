# Soda Makefile
# Authors: Haseeb Saeed, Mayank Sindwani
# Date: 2015-11-26

# Compiler Options
CXX = u++
CXXFLAGS = -g -I"." -Wall -Wno-unused-label -Wfatal-errors -MMD -O2 -std=c++11
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}

# Main Source Files
OBJECTS =              \
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
	main.o             \

EXEC = soda
DEPENDS = ${OBJECTS:.o=.d}

#############################################################

.PHONY : all clean

${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} $^ -o $@


#############################################################

${OBJECTS} : ${MAKEFILE_NAME}           # OPTIONAL : changes to this file => recompile

-include ${DEPENDS}                     # include *.d files containing program dependences

clean :                                 # remove files that can be regenerated
	rm -f *.d *.o ${EXEC}
