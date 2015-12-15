
CXXFLAGS=-O3 -g -std=c++1y -I/usr/include/
LDFLAGS=-L/usr/local/lib64/
# LD_LIBS = -larmadillo
CXX = mpicxx
LD = mpicxx

all: main 

main: main.o
	$(LINK.cc) -o $@ $^ $(LD_LIBS)

main.o: main.cc
	$(COMPILE.cc) -o $@ $^

clean:
	rm -rf *.o *.dSYM main
