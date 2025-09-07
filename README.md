**Pathfinding Visualizer**

This project is a simple pathfinding visualizer built with C++ and SFML.
It allows you to draw walls, run a pathfinding algorithm, and visualize how the shortest path is found in real-time.

**Features**

Grid-based map with customizable node size

Interactive wall drawing and removal

Real-time pathfinding visualization

Easy controls for starting, stopping, and resetting the search

**Controls**

Left Mouse Button -> Draw a wall

Right Mouse Button -> Remove a wall

Space -> Start the pathfinding algorithm

R -> Reset the grid

P → pause the search (while paused, you can draw or remove walls, then press P again to resume)

After the search finishes, you don’t need to reset the grid – you can immediately add or remove walls and start another search.

**Requirements**

C++17 or newer
SFML 3.0.1

**Building**

Clone the repository

Open the project in Visual Studio or your preferred IDE

Build the project using the provided CMakeLists.txt (the build system already uses a vcpkg manifest, so you don’t need to set up SFML manually)

**Setting up Visual Studio Project (Static SFML)**

To build locally without vcpkg, you need to configure your Visual Studio project to use the static version of SFML.

1. Install SFML

Download SFML 3.0.0 (static libraries) from the official website.
Extract it to:
C:\SFML-3.0.0
(the folder must contain include and lib directories).

2. Configure Visual Studio

Right-click on your project → Properties.
Apply the following settings separately for Debug and Release.

......Debug configuration......

C/C++ → Preprocessor → Preprocessor Definitions : SFML_STATIC; \_DEBUG;

C/C++ → Code Generation → Runtime Library : Multi-threaded Debug (/MTd)

C/C++ → General → Additional Include Directories : C:\SFML-3.0.0\include

Linker → General → Additional Library Directories : C:\SFML-3.0.0\lib

Linker → Input → Additional Dependencies:

sfml-graphics-s-d.lib<br>
sfml-window-s-d.lib<br>
sfml-system-s-d.lib<br>
winmm.lib<br>
dsound.lib<br>
dxguid.lib<br>
dinput8.lib<br>
opengl32.lib<br>

......Release configuration......

C/C++ → Preprocessor → Preprocessor Definitions : SFML_STATIC; NDEBUG;

C/C++ → Code Generation → Runtime Library : Multi-threaded (/MT)

C/C++ → General → Additional Include Directories : C:\SFML-3.0.0\include

Linker → General → Additional Library Directories : C:\SFML-3.0.0\lib

Linker → Input → Additional Dependencies:

sfml-graphics-s.lib<br>
sfml-window-s.lib<br>
sfml-system-s.lib<br>
winmm.lib<br>
dsound.lib<br>
dxguid.lib<br>
dinput8.lib<br>
opengl32.lib<br>

**Prebuilt executables**

You can download the latest Pathfinding Visualizer EXE from the GitHub Release here: **[Download EXE](https://github.com/rrasp94/Pathfinding/releases/tag/master)**
