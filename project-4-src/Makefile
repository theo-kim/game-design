default: main.cpp
	g++ -o game main.cpp -I ../include/SDL2 -L ../lib -l SDL2-2.0.0 && ./game

mac: main.cpp
	g++ -g -std=c++11 main.cpp Game.cpp Text.cpp Entity.cpp Ship.cpp Star.cpp Quadtree.cpp Sensor.cpp ShipTypes.cpp Gun.cpp Bullet.cpp ../include/global/ShaderProgram.cpp ../include/global/LoadTexture.cpp -o game -I../include/global -I../include/darwin -L../lib/darwin -l SDL2-2.0.0 -l SDL2_image-2.0.0 -framework Cocoa -framework OpenGL && ./game

g++ -std=c++11 src/Camera.cpp src/Character.cpp src/CollisionEngine.cpp src/Entity.cpp src/Game.cpp src/PhysicsEngine.cpp src/Scene.cpp src/Sensor.cpp src/Text.cpp src/main.cpp ../include/global/LoadTexture.cpp ../include/global/ShaderProgram.cpp -o game -g -I../include/global -I../include/darwin -L../lib/darwin -framework Cocoa -framework OpenGL  -l SDL2-2.0.0 -l SDL2_image-2.0.0 && ./game

clean: all
	rm game
