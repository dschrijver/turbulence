# turbulence
Simulation of a turbulent pipe flow around an obstacle using the Lattice Boltzmann Equation. An in-depth explanation can be found [here](https://dschrijver.github.io/turbulence.html).

## Requirements

### Compiling for Linux

This program is developed for GNU Linux. The required programs can be installed using the following command:

```
sudo apt install build-essential binutils clang git
```

Additionally, the [raylib](https://github.com/raysan5/raylib) library needs to be built and installed to a standard location. First install the required [libraries](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux). The following command installs the libraries for Ubuntu:

```
sudo apt install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev
```

Download the raylib library using:

```
git clone https://github.com/raysan5/raylib.git
cd raylib/src/
```

Now edit the config file [config.h](https://github.com/raysan5/raylib/blob/master/src/config.h) in this folder to uncomment the following line (remove '//' at the start of the line):

```
//#define SUPPORT_CUSTOM_FRAME_CONTROL    1
```

The library can now be built and installed:

```
make PLATFORM=PLATFORM_DESKTOP
sudo make install
```

### Compiling for Windows
A Windows executable can also be created, using ```mingw-w64```. The additional requirements can be installed using:

```
sudo apt install mingw-w64 wine
```

Raylib needs to be compiled using the ```gcc``` and ```ar``` executables provided by ```mingw-w64```. These can for instance be found in the folder /usr/bin, with names such as ```x86_64-w64-mingw32-gcc``` and ```x86_64-w64-mingw32-ar```. Move into the root directory of this project, and run the commands:

```
git clone https://github.com/raysan5/raylib.git
cd raylib/src/
```

In this folder, edit the [config.h](https://github.com/raysan5/raylib/blob/master/src/config.h) file as mentioned in the previous section. After this, change the following lines in the [Makefile](https://github.com/raysan5/raylib/blob/master/src/Makefile):

```
CC = gcc
AR = ar
```

into:

```
CC = x86_64-w64-mingw32-gcc
AR = x86_64-w64-mingw32-ar
```

To compile the library, run the command:

```
make GLFW_LINUX_ENABLE_WAYLAND=FALSE GLFW_LINUX_ENABLE_X11=FALSE
```

## Running the simulation
The program can be compiled and ran using the command ```make```. Settings can be changed in the file [turbulence.h](turbulence.h). To compile for windows and run the executable using ```wine```, use the command ```make turbulence_windows```.
