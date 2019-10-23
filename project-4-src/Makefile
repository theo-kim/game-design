default: main.cpp
	g++ -o game main.cpp -I ../include/SDL2 -L ../lib -l SDL2-2.0.0 && ./game

mac: main.cpp
	g++ -g main.cpp Game.cpp Text.cpp Entity.cpp Ship.cpp Star.cpp ../include/global/ShaderProgram.cpp ../include/global/LoadTexture.cpp -o game -I../include/global -I../include/darwin -L../lib/darwin -l SDL2-2.0.0 -l SDL2_image-2.0.0 -framework Cocoa -framework OpenGL && ./game

clean: all
	rm game
