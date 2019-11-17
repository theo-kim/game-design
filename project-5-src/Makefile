CC = g++ -std=c++11
NAME = game

ODIR = src/obj
IDIR = ../include/global
LIBS = -l SDL2-2.0.0 -l SDL2_image-2.0.0

MAC_LDIR = ../lib/darwin
MAC_FRAMEWORKS = -framework Cocoa -framework OpenGL
MAC_IDIR = ../include/darwin

SRC = $(wildcard src/*.cpp) \
	  main.cpp \
	  $(wildcard ../include/global/*.cpp)
_OBJ = $(SRC:.cpp=.o)
OBJ = $(patsubst src/%,$(ODIR)/%,$(_OBJ))
DEP = $(OBJ:.o=.d)  # one dependency file for each source

LDFLAGS = $(libgl) $(LIBS)
CXXFLAGS = -g -I$(IDIR) $(incl)

ifeq ($(shell uname -s), Darwin)
	libgl = $(MAC_FRAMEWORKS)
	incl = -I$(MAC_IDIR)
endif

$(ODIR)/%.o: src/%.cpp
	$(CC) -c -o $@ $< $(CXXFLAGS)

game: $(OBJ)
	$(CC) -o $@ $(LDFLAGS) $^ && ./$@

-include $(DEP)
%.d: %.cpp
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

.PHONY: clean
clean:
	rm -f src/obj/*.o game
