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

**Prebuilt executables**

The project is automatically built with GitHub Actions.
You can download the latest .exe from the latest green workflow run under the Actions tab.

