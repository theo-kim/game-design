CC = g++ -std=c++11
NAME = game

ODIR = build
IDIR = ../include/global
LIBS = -l SDL2-2.0.0 -l SDL2_image-2.0.0 -l SDL2_mixer-2.0.0

MAC_LDIR = ../lib/darwin
MAC_FRAMEWORKS = -framework Cocoa -framework OpenGL
MAC_IDIR = ../include/darwin

SRC = $(wildcard src/*.cpp) \
	  $(wildcard ../include/global/*.cpp)
_OBJ = $(SRC:.cpp=.o)
OBJ = $(patsubst src/%,$(ODIR)/%,$(_OBJ))
DEP = $(OBJ:.o=.d)  # one dependency file for each source

LDFLAGS = $(libincl) $(LIBS) $(libgl)
CXXFLAGS = -g -I$(IDIR) $(incl)

ifeq ($(shell uname -s), Darwin)
	libgl = $(MAC_FRAMEWORKS) 
	libincl = -L$(MAC_LDIR)
	incl = -I$(MAC_IDIR)
endif

$(ODIR)/%.o: src/%.cpp
	$(CC) -c -o $@ $< $(CXXFLAGS)

game: $(OBJ)
	$(CC) $^ -o $@ $(CXXFLAGS) $(LDFLAGS) && ./$@

-include $(DEP)
$(ODIR)/%.d: src/%.cpp
	@$(CPP) $(CXXFLAGS) $< -MM -MT $(@:.d=.o) >$@

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o main.o game
.PHONY: cleandeps
cleandeps:
	rm -f $(ODIR)/*.d main.o game
