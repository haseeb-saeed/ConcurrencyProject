BUILD=DEBUG

CXX = u++
CXXFLAGS = -g -Wall -Wno-unused-label -Wfatal-errors -MMD -D"${BUILD}" -O2 -std=c++11

SRC = $(wildcard *.cc)
OBJECTS = $(SRC:.c=.o)
DEPENDS = ${OBJECTS}
EXEC = soda

.PHONY : clean

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} $^ -o $@

clean:
	rm -f *.d *.o ${EXEC}
