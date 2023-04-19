# Build instructions

To build the source code, you have to clone the repository (with submodules), install the Vulkan SDK and compile the code with a compiler which supports C++17. On Linux you have to install X11 and on Windows you have to use Direct3D 11.1. If you have trouble building the code, take a look at the following step-by-step build instructions.

Required versions:

- CMake 3.16+ (tested with 3.22.1)
- Vulkan SDK 1.2+ (tested with 1.3.224.1)
- Python (tested with 3.11.0)

On Windows:

- Visual Studio 2017+ (tested with 2022)
- Direct3D 11.1

On Linux:

- GCC 8+ (tested with 11.2.0)

## Windows

1. Download and install __CMake__ (<https://cmake.org/download>)
2. Download and install __Python__ (<https://www.python.org/downloads/>)
3. Download and install the LunarG __Vulkan SDK__ (<https://vulkan.lunarg.com/sdk/home#windows>)
4. Download and install __Visual Studio__ 2022 Community Edition (<https://visualstudio.microsoft.com/downloads>)
   - Include the __Desktop development with C++__ and the __Git for Windows__ components
5. Open Visual Studio and select the __Clone a repository__ option in the Start Window
6. Enter __<https://github.com/racz16/Windmill-Game-Engine.git>__ into the Repository location
7. Click on the __Clone__ button
8. Click on the __Switch between solutions and available views__ button (top of the Solution Explorer)
9. Select the __example_1.exe__ Debug Target (click on the little arrow pointing downwards next to the button Select Startup Item...)
    - You can switch between the __debug-win__ and the __release-win__ configurations
10. Click on the __example_1.exe__ button

## Linux

1. Open the Terminal
2. Install git

    ```bash
    sudo apt install git
    ```

3. Install CMake

    ```bash
    sudo apt install cmake
    ```

4. Install the C++ build tools

    ```bash
    sudo apt install build-essential
    ```

5. Install X11

    ```bash
    sudo apt install xorg-dev
    ```

6. Install the Vulkan SDK

    ```bash
    wget -qO - https://packages.lunarg.com/lunarg-signing-key-pub.asc | sudo apt-key add -
    ```

    ```bash
    sudo wget -qO /etc/apt/sources.list.d/lunarg-vulkan-1.3.224-focal.list https://packages.lunarg.com/vulkan/1.3.224/lunarg-vulkan-1.3.224-focal.list
    ```

    ```bash
    sudo apt update
    ```

    ```bash
    sudo apt install vulkan-sdk
    ```

7. Clone the repository

    ```bash
    git clone --recursive https://github.com/racz16/Windmill-Game-Engine.git
    ```

    Don't forget the --recursive option to download the necessary dependencies

8. Go inside the repository's root folder

    ```bash
    cd ./Windmill-Game-Engine
    ```

9. Create a build folder

    ```bash
    mkdir build
    ```

10. Go inside the build folder

    ```bash
    cd ./build
    ```

11. Create a config-specific folder

    ```bash
    mkdir linux-debug
    ```

    or

    ```bash
    mkdir linux-release
    ```

12. Go inside the config-specific folder

    ```bash
    cd ./linux-debug
    ```

    or

    ```bash
    cd ./linux-release
    ```

13. Run CMake

    ```bash
    cmake -DCMAKE_BUILD_TYPE=Debug ../..
    ```

    or

    ```bash
    cmake -DCMAKE_BUILD_TYPE=Release ../..
    ```

14. Build the code

    ```bash
    cmake --build .
    ```

15. Go inside the runtime folder

    ```bash
    cd ./runtime
    ```

16. Run one of the examples

    ```bash
    ./example_1
    ```
