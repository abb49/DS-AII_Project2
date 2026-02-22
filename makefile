CXX = g++
CXXFLAGS = -std=c++11 -Wall

OBJS = main.o

p :	${OBJS}
	${CXX} $(CXXFLAGS) -o $@ ${OBJS}

main.o : Customer.hpp
	${CXX} ${CXXFLAGS} -c main.cpp

clean: 
	rm -f *.o
	rm p