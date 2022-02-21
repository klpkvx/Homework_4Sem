GCFLAGS=-O3 -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format

all: a01.out a02.out a03.out a04.out a05.out

%.o: %.cpp
	g++ -c $(GCFLAGS) $<

%.out: %.o
	g++ $^ -o $@

clean:
	rm *.o*


a01.o: a01.cpp query.h
a01.out: a01.o

a02.o: a02.cpp query.h
a02.out: a02.o

a03.o: a03.cpp query.h
a03.out: a03.o

a04.o: a04.cpp query.h
a04.out: a04.o


a05.o: a05.cpp query.h
a05.out: a05.o
