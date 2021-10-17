# Build instructions

To build the source code, you have to clone the repository (with submodules) and compile it with a compiler which supports C++17. In Linux you have to install X11. If you have trouble building the code, take a look at the following step-by-step build instructions.

## Windows

1. Download and install __CMake__ (<https://cmake.org/download>)
2. Download and install __Visual Studio__ 2019 Community Edition (<https://visualstudio.microsoft.com/downloads>)
   - Include the __Desktop development with C++__ and the __Git for Windows__ components
3. Open Visual Studio and select the __Clone a repository__ option in the Start Window
4. Enter __<https://github.com/racz16/Windmill-Game-Engine.git>__ into the Repository location
5. Click on the __Clone__ button
6. Click on the __Switch between solutions and available views__ button (top of the Solution Explorer)
7. Select the __example_1.exe__ (or any other) Debug Target (click on the little arrow pointing downwards next to the button Select Startup Item...)
   - You can switch between the __debug-win__ and the __release-win__ configurations
8. Click on the __example_1.exe__ button

## Linux

1. Open the Terminal
2. Install git

        sudo apt install git

3. Install CMake

        sudo apt install cmake

4. Install the C++ build tools

        sudo apt install build-essential

5. Install X11

        sudo apt install xorg-dev

6. Clone the repository

        git clone --recursive https://github.com/racz16/Windmill-Game-Engine.git

    Don't forget the --recursive option to download the necessary dependencies

7. Go inside the repository's root folder

        cd Windmill-Game-Engine

8. Run CMake

        cmake .

9. Build the code

        cmake --build .

    or

        cmake --build . --config Release

10. Run one of the examples

        ./example_1
