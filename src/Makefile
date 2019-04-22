CXX = g++-8
CXXFLAG = -std=c++17 -O2 -Wall -Wextra -fPIC
GMAEINCLUDE = ./
BUILDDIR = ./build

all: gamerunner

AI:
	$(CXX) $(CXXFLAG) -I$(GMAEINCLUDE) -shared AITemplate/Porting.cpp -o ${BUILDDIR}/a1.so

gamerunner: AI
	$(CXX) $(CXXFLAG) -I$(GMAEINCLUDE) -pthread main.cpp -o ${BUILDDIR}/gamerunner -ldl

.PHONY: clean
clean:
	-rm gamerunner a1.so
