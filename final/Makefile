CC = g++ -std=c++11
NAME = game

ODIR = build

IDIR = ../include/global/ltd
IDIR_PROJ = ./include
LIBS = -l SDL2-2.0.0 -l SDL2_image-2.0.0 -l SDL2_mixer-2.0.0 -l SDL2_ttf-2.0.0 -l glew

MAC_LDIR = ../lib/darwin
MAC_FRAMEWORKS = -framework Cocoa -framework OpenGL
MAC_IDIR = ../include/darwin

WIN32_LDIR = ../lib/windows-x86
WIN64_LDIR = ../lib/windows-x64
WIN_IDIR = ../include/windows


SRC = $(shell find src -type f -name '*.cpp') \
	  	$(wildcard ../include/global/LoadTexture.cpp)
_OBJ = $(SRC:.cpp=.o)
OBJ = $(patsubst src/%,$(ODIR)/%,$(_OBJ))
DEP = $(OBJ:.o=.d)  # one dependency file for each source

LDFLAGS = $(libincl) $(LIBS) $(libgl)
CXXFLAGS = -g -I$(IDIR) -I$(IDIR_PROJ) $(incl)


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

# .PHONY: directories
# directories:
# 	if test -d $(ODIR); then echo ""; else mkdir $(ODIR); fi
# 	find src -type d >dirs.txt
# 	xargs mkdir -p <dirs.txt
# 	rm dirs.txt

$(ODIR)/%.o: src/%.cpp
	$(CC) -c -o $@ $< $(CXXFLAGS)

default: $(OBJ)
	if test -d $(ODIR); then echo ""; else mkdir $(ODIR); fi
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
