CC = g++ -std=c++11
NAME = game

ODIR = src/obj
IDIR = ../include/global
LIBS = -l SDL2-2.0.0 -l SDL2_image-2.0.0

MAC_LDIR = ../lib/darwin
MAC_FRAMEWORKS = -framework Cocoa -framework OpenGL
MAC_IDIR = ../include/darwin

MAC_CC_FLAGS = -g -I$(IDIR) -I$(MAC_IDIR) -L$(MAC_LDIR) $(LIBS) $(MAC_FRAMEWORKS) -o $(NAME)

SRC = $(wildcard *.c)
OBJ = $(src:.c=.o)

default: main.cpp
	g++ -o game main.cpp -I ../include/SDL2 -L ../lib -l SDL2-2.0.0 && ./game

mac: main.cpp, 
	$(CC) main.cpp Game.cpp Text.cpp Entity.cpp CollisionEngine.cpp PhysicsEngine.cpp Character.cpp Sensor.cpp ../include/global/ShaderProgram.cpp ../include/global/LoadTexture.cpp $(MAC_CC_FLAGS) && ./$(NAME)

.PHONY: clean
clean: all
	rm -f $(NAME)
