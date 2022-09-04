# Comp-371 | Project

Comp-371 Computer graphics project, where we demonstrate a prodedural generated world with custom perlin-noise height maps and various other assets like houses, trees, textures, etc.


## Team Members
- Shiraz Siwadjian - 40136555
- Sevag Kaspar - 40100393
- Hamza Shah - 40092722
- Hadrien Ovize - 40025905
- Samdarshi Tiwari - 40113256


## Buttons
Camera Movement:
> W - Forward
> A - Left
> S - Backward
> D - Right
> SHIFT - Increase the camera speed


## Filesystem
- **CMakeLists.txt** : All cmake configurations
- **main.cpp**: Main c++ file containing the code for the quiz
- **/include/**: Include folder containing all the header files
- **/assets/Models/**: Folder containing the obj files for the integrated models
- **/assets/shaders/**: Folder containing the various fragment and vertex shaders used in the project
- **/assets/textures/**: Folder containing the image files for the textures
- **/assets/Models/**: Source code folder containing the cpp files


## Build Instructions

### Command Line

1. Download and Install CMake
2. Open a terminal
3. Run cmake:

```
cd <source_folder>
cmake -S . -B <build_folder>
cmake --build <build_folder> --target install
```

To generate an Xcode project from the command line use:

```
cd <source_folder>
cmake -S . -B <build_folder> -G Xcode
cmake --build <build_folder> --target install
```


**NOTE:** In the case of cmake the `<source_folder>` is the topmost folder with
a CMakeLists.txt file.

**NOTE:** The `<build_folder>` can be anything but is usually just named `build`.


### CMake GUI

You can use the CMake GUI to configure and generate the Visual Studio Project.

1. Download and Install CMake
2. Open the CMake GUI
3. Set the `Where is the source code:` field to the source folder
4. Set the `Where to build the binaries:` field to different location (e.g.: source_folder/build)
5. Click Configure
6. Select the project generator (this will be the project file that compiles program)
7. Click Generate

#### Windows

Once the project is created you can build the **ALL_BUILD** project followed by
building the **INSTALL** project.

#### Xcode

Once the project is created you can build the **ALL_BUILD** target followed by
the **INSTALL** target.

#### Unix Make File

In the terminal:

```
cd <build_folder>
make install
```


## Running

Once built and installed, all the relevant files will be in the **dist** folder
at the top level of the source folder.

### Linux/macOS

In the terminal type:

```
./lab03
```

### Windows

Click on the **lab03.exe** from the File Explorer.



## Sources
- Reused code from Labs and Assignments
- https://www.cgtrader.com/free-3d-mode
- https://github.com/Reputeless/PerlinNoise
- https://gist.github.com/Pikachuxxxx/a2454e98bb7723afd7251a891c18f194
- https://learnopengl.com/Getting-started/Hello-Triangle
- https://learnopengl.com/Advanced-OpenGL/Advanced-GLSL
- http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/
- https://learnopengl.com/Lighting/Basic-Lighting-
- https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
- https://github.com/ClaudiaLapalme/WoodlandWanderer/tree/master/Assets/Model
- https://learnopengl.com/height-map
