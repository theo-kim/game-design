CC = g++ -std=c++11
NAME = game

ODIR = build

IDIR = ../include/global
LIBS = -l SDL2-2.0.0 -l SDL2_image-2.0.0 -l SDL2_mixer-2.0.0

MAC_LDIR = ../lib/darwin
MAC_FRAMEWORKS = -framework Cocoa -framework OpenGL
MAC_IDIR = ../include/darwin

WIN32_LDIR = ../lib/windows-x86
WIN64_LDIR = ../lib/windows-x64
WIN_IDIR = ../include/windows


SRC = $(wildcard src/*.cpp) \
	  $(wildcard ../include/global/*.cpp)
_OBJ = $(SRC:.cpp=.o)
OBJ = $(patsubst src/%,$(ODIR)/%,$(_OBJ))
DEP = $(OBJ:.o=.d)  # one dependency file for each source

LDFLAGS = $(libincl) $(LIBS) $(libgl)
CXXFLAGS = -g -I$(IDIR) $(incl)


# DON'T EDIT BELOW THIS LINE

ifeq ($(OS),Windows_NT)
	incl = -I$(WIN_IDIR)
	ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
        libincl = -L$(WIN64_LDIR)
    else
        ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
            libincl = -L$(WIN64_LDIR)
        endif
        ifeq ($(PROCESSOR_ARCHITECTURE),x86)
			libincl = -L$(WIN32_LDIR)
		endif
	endif
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Darwin)
		libgl = $(MAC_FRAMEWORKS) 
		libincl = -L$(MAC_LDIR)
		incl = -I$(MAC_IDIR)
	endif
endif

$(ODIR)/%.o: src/%.cpp
	if test -d $(ODIR); then echo ""; else mkdir build; fi
	$(CC) -c -o $@ $< $(CXXFLAGS)

default: $(OBJ)
	$(CC) $^ -o $(NAME) $(CXXFLAGS) $(LDFLAGS) && ./$(NAME)

.PHONY: $(NAME)
$(NAME):
	./$(NAME)

-include $(DEP)
$(ODIR)/%.d: src/%.cpp
	@$(CPP) $(CXXFLAGS) $< -MM -MT $(@:.d=.o) >$@

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o ./$(NAME)
.PHONY: cleandeps
cleandeps:
	rm -f $(ODIR)/*.d ./$(NAME)
