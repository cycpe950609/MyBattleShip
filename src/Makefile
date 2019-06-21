CXX = g++
CXXFLAG = -std=c++17 -O2 -Wall -Wextra -fPIC
GAMEINCLUDE = ./
BUILDDIR = ./build
ALLEGRO = `pkg-config --libs allegro-5 allegro_font-5 allegro_ttf-5 allegro_dialog-5 allegro_primitives-5 allegro_image-5 allegro_audio-5 allegro_acodec-5`
ENGINE_DIR = Engine
OBJS = $(patsubst %,$(ENGINE_DIR)/%,$(wildcard ./Engine/*.o))

all: gamerunner AI

AI:
	$(CXX) $(CXXFLAG) -I$(GAMEINCLUDE) -shared AITemplate/Porting.cpp -o ${BUILDDIR}/AI.so
	cp ${BUILDDIR}/AI.so ${BUILDDIR}/a1.so
	cp ${BUILDDIR}/AI.so ${BUILDDIR}/a2.so
$(ENGINE_DIR)/%.o: $(ENGINE_DIR)/%.cpp 
	$(CC) -c -g -o $@ $< $(CXXFLAG) -I$(GAMEINCLUDE)

gamerunner:
	$(CXX) $(CXXFLAG) -c -g $(wildcard *.cpp) -I$(GAMEINCLUDE)
	$(CXX) *.o -o  ${BUILDDIR}/gamerunner $(ALLEGRO) -Iallegro -pthread -ldl 
	#$(CXX) $(CXXFLAG) -I$(GMAEINCLUDE) -pthread main.cpp $(wildcard ./*.cpp) -o ${BUILDDIR}/gamerunner -ldl

.PHONY: clean
clean:
	#-rm gamerunner a1.so
	rm $(wildcard ./*o ./**/*.o) 
