# Intro to Game Design Projects (Fall 2019)

By **Theodore Kim**

This is the Repo for my Game Design Projects, written in C++ using OpenGL and SDL.

## Projects

* **Project 1**: Simple Scene using SDL
* **Project 2**: Pong
* **Project 3**: Lunar Lander
* **Project 4**: Rise of AI
* **Project 5**: Platformer*
* **Final Project**: TBD*

<sub>\* Project 5 and the Final Project were done with [JinZhao Su](https://github.com/JinZSu)</sub>

## Building

To build the projects, it depends on your OS. As of this commit Projects 1 - 4 only support macOS. Project 5 supports macOS and Windows.

### Using Make:

If you don't have make, you can install using homebrew on Mac (`brew install make`), Linux (i.e. `apt-get install make`), or Windows (MinGW, Chocolatey, or GNUWin32, check this [Stack Overflow post](https://stackoverflow.com/questions/32127524/how-to-install-and-use-make-in-windows)). You also need a C++ compiler like g++.

* For projects 1 - 4, you need to run the command `make mac` for building for macOS.

* For project 5 and the final project, you can just run `make` and it will build the project for any OS (macOS and Windows as of the most recent commit).

### Build files:

Not yet implemented, but hopefully I will get around to adding a Shell Script that you can just run to build.

### Other notes:

On Mac, you may also need to install SDL, SDL_image, and SDL_mixer using homebrew:

```
$ brew install sdl2
$ brew install sdl2_image
$ brew install sdl2_mixer
```