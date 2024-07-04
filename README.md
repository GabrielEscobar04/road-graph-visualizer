# City Road Graph Visualizer

## Project Overview

This project visualizes city road graphs using data from OpenStreetMap. It utilizes OpenGL and GLFW for rendering, providing an interactive 3D visualization of urban road networks.

## Prerequisites

- Python 3.x
- C++17 compatible compiler
- CMake (3.10 or later recommended)

## Dependencies

### Python Libraries
- osmnx
- json

### C++ Libraries
- OpenGL
- GLFW
- GLM (OpenGL Mathematics)
- GLAD

## Installation and Setup

### Ubuntu (Tested on Ubuntu 22.04)

1. Install Python and pip:
```sh
sudo apt-get update
sudo apt-get install python3 python3-pip
```

2. Install required Python libraries:
```sh
pip3 install osmnx json
```

3. Install CMake and necessary development libraries:
```sh
sudo apt-get install cmake libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
```

4. Clone the repository:
```sh
git clone https://github.com/yourusername/city-road-graph-visualizer.git
cd city-road-graph-visualizer
```

### macOS and Windows

The application has not been tested on macOS or Windows. The setup process may be similar to Ubuntu, but additional steps might be required. Contributions for installation instructions on these platforms are welcome.

## Usage

1. Configure the visualization:
Edit the `config.txt` file to set desired parameters such as city, node size, edge size, and some other configuration.

2. Import and parse OpenStreetMap data:
```sh
python3 import/main.py
```
This will generate `nodes.txt` and `edges.txt` files with the necessary data for visualization.

3. Build and run the project:
```sh
mkdir build
cd build
cmake ..
cmake --build . --target run
```

## Controls

When the application is running, the following controls are available:

- WASD keys: Move around
- Up/Down arrow keys: Zoom in/out
- Left/Right arrow keys: Roll the camera
- R key: Reset camera view

## Cleanup

To clean up build files:
```sh
rm -rf build/*
```
To rebuild and run after cleanup, follow step 3 in the Usage section.

## Troubleshooting

If you encounter any issues, please check the following:
- Ensure all dependencies are correctly installed
- Verify that the `config.txt` file is properly configured
- Check that the Python script successfully generated the `nodes.txt` and `edges.txt` files

If problems persist, please open an issue on the project's GitHub page.

## Limitations

The application uses the osmnx library to fetch data from OpenStreetMap. As such, it is subject to the limitations of both osmnx and OpenStreetMap. Very large cities or complex road networks may cause performance issues or fail to load properly.

## Contributing

Contributions to this project are welcome, especially for improving compatibility with different operating systems. Please follow these steps:
1. Fork the repository
2. Create a new branch for your feature
3. Commit your changes
4. Push to the branch
5. Create a new Pull Request

## Acknowledgments

- OpenStreetMap contributors for providing the map data
- GLFW, GLAD, and GLM projects for graphics and mathematics libraries
- osmnx library for facilitating OpenStreetMap data retrieval

## Contact

For issues, feature requests, or questions, please open an issue on the project's GitHub