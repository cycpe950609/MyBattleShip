CXX = g++-8
CXXFLAG = -std=c++17 -O2 -Wall -Wextra -fPIC
GMAEINCLUDE = ./
BUILDDIR = ./build

all: gamerunner

libP1.so:
	$(CXX) $(CXXFLAG) -I$(GMAEINCLUDE) -shared TEST/A.cpp -o ${BUILDDIR}/a1.so

libP2.so:
	$(CXX) $(CXXFLAG) -I$(GMAEINCLUDE) -shared TEST/B.cpp -o ${BUILDDIR}/a2.so

gamerunner: libP1.so libP2.so
	$(CXX) $(CXXFLAG) -I$(GMAEINCLUDE) -pthread main.cpp -o ${BUILDDIR}/gamerunner -ldl

.PHONY: clean
clean:
	-rm gamerunner a1.so
