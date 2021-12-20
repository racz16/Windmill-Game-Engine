# Build instructions

To build the source code, you have to clone the repository (with submodules), install the Vulkan SDK and compile the code with a compiler which supports C++17. On Linux you have to install X11. It is also necessary to compile the shaders with the appropirate script in windmill/res/shader folder. You have to repeat the shader compilation every time you create or edit a shader. If you have trouble building the code, take a look at the following step-by-step build instructions.

Required versions:

- CMake 3.7+ (tested with 3.19.2)
- Vulkan SDK 1.2+ (tested with 1.2.189.2)
- Visual Studio 2017+ (tested with 2019)
- GCC 8+ (tested with 9.3.0)

## Windows

1. Download and install __CMake__ (<https://cmake.org/download>)
2. Download and install the LunarG __Vulkan SDK__ (<https://vulkan.lunarg.com/sdk/home#windows>)
3. Download and install __Visual Studio__ 2022 Community Edition (<https://visualstudio.microsoft.com/downloads>)
   - Include the __Desktop development with C++__ and the __Git for Windows__ components
4. Open Visual Studio and select the __Clone a repository__ option in the Start Window
5. Enter __<https://github.com/racz16/Windmill-Game-Engine.git>__ into the Repository location
6. Click on the __Clone__ button
7. Open the windmill/res/shader folder and run the __compile.ps1__ script (right click, Run with PowerShell)
   - You have to repeat this step every time you create or edit a shader
8. Click on the __Switch between solutions and available views__ button (top of the Solution Explorer)
9. Select the __example_1.exe__ (or any other) Debug Target (click on the little arrow pointing downwards next to the button Select Startup Item...)
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
    sudo wget -qO /etc/apt/sources.list.d/lunarg-vulkan-1.2.189-focal.list https://packages.lunarg.com/vulkan/1.2.189/lunarg-vulkan-1.2.189-focal.list
    ```

    ```bash
    sudo apt-get update
    ```

    ```bash
    sudo apt-get install vulkan-sdk
    ```

7. Clone the repository

    ```bash
    git clone --recursive https://github.com/racz16/Windmill-Game-Engine.git
    ```

    Don't forget the --recursive option to download the necessary dependencies

8. Run the shader compiler script

    ```bash
    Windmill-Game-Engine/windmill/res/shader/compile.sh
    ```

    You have to repeat this step every time you create or edit a shader

9. Go inside the repository's root folder

    ```bash
    cd Windmill-Game-Engine
    ```

10. Run CMake

    ```bash
    cmake .
    ```

11. Build the code

    ```bash
    cmake --build .
    ```

    or

    ```bash
    cmake --build . --config Release
    ```

12. Run one of the examples

    ```bash
    ./example_1
    ```
