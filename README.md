WebCam based games
==================

Old `OpenCV` based project brought to new live.
Works, but still early version.

Usage
-----

 * `CvGames 3d` Camera based 3D object rotation
 * `CvGames bob` Motion based bubble shot game
 * `CvGames vj` Motion based image fading
 * `CvGames` See all available arguments

Compiling
---------

Use GNU make

```
make
```

or compile directly

```
g++ -std=c++11 src/main.cpp src/Helpers/* src/Objects3D/* src/Output/* src/Tracker/* `pkg-config --cflags --libs opencv gl` -lglut -o CvGames
```

or use `cmake` (CLion IDE also supports it):

```
mkdir cmake-build-debug
cd cmake-build-debug
cmake ../
```

This will genrerate `OpenCV` executable.
    
Libraries used
--------------

 * OpenCV http://opencv.org/
 * OpenGl http://www.opengl.org/
 * Glut http://www.opengl.org/resources/libraries/glut/
    
Resources
---------

See `Data` folder for

 * Images used in `vj` part
 * WaveFront (`.obj`) meshes and materials (`.mtl`) are exported usign [Blender] (https://www.blender.org/).

License
-------

[MIT](LICENSE)

