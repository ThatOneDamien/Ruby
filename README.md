# Ruby2D the 2D Game Engine

### Description
<img src="Ruby_logo.png" align="right"
     alt="Ruby2D Logo" width="128" height="128">
Ruby2D is my personal cross-platform C++ game engine project for designing small but efficient 2D (and eventually 3D) games. Currently Ruby only has build support for Windows and Linux systems, but eventually I hope to expand support to Mac as well.

### Disclaimer
A couple things I want to point out before explaining the project further are: firstly, this project was my first REAL time using version control so when looking at the commit history you may see taboos that have been broken, but please understand it has been and still is a learning process; secondly, I used The Cherno's Game Engine series to get started, as I had no experience with game engine development nor with cross-platform software development prior to this project. You can find his YouTube channel [here](https://www.youtube.com/@TheCherno), so go check him out!<br>

## Basic Setup

Prior to any attempt to use or download, ensure you are working on a supported environment. Currently, only systems running Windows or Linux are able to build Ruby, and on Windows Visual Studio 2019 is needed to utilize **Premake5**, the build system used by Ruby.

### 1. Cloning the Repo

First, simply clone the repo using the command `git clone --recursive https://github.com/ThatOneDamien/Ruby2D.git`.<br>Using the recursive flag when cloning ensures the submodules are cloned as well.

In the event that you accidentally clone the repo non-recursively, you can run the command `git submodule update --init`.

### 2. Generating Required Build Files

After cloning the repo, navigate to the build_scripts folder where you will find the scripts used to generate the files required to build the project.<br>

On **Linux** run [gen_gmake.sh](https://github.com/ThatOneDamien/Ruby2D/blob/master/build_scripts/gen_gmake.sh) to generate the Makefiles associated with the project.<br>

On **Window** run [gen_vs_solution.bat](https://github.com/ThatOneDamien/Ruby2D/blob/master/build_scripts/gen_vs_solution.bat) to generate the VS project/solution files for opening the project in VS 2019.

### 3. Build the project

Now that the build files have been generated, you can simply build the project using Visual Studio, or make on Linux! Note that this repo ships with 2 premade applications that depend on the core Ruby engine, those being the Ruby Editor, and Sandbox. Further details about the Ruby Editor can be found [here](RubyEditor/README.md). Sandbox at the moment is merely a test project that I have used to test various features. In the future I will make changes so that this project will be a true example application with all of the necessary groundwork to create a standalone app, but for now the code within `Sandbox.cpp` can be discarded and replaced with your own application if desired.

For help on starting an app that uses the Ruby Engine from scratch, navigate to the wiki section below.

## Dependencies

- [premake5](https://premake.github.io) - Build system for cross-platform support.
- [glad](https://glad.dav1d.de) - For OpenGL C++ binding.
- [glm](https://github.com/icaven/glm) - For easy-to-use OpenGL math defines and functions.
- [GLFW](https://www.glfw.org) - For cross-platform windowing using its robust API.
- [FreeType](https://freetype.org) - For font rasterization, used in msdf-atlas-gen.
- [msdf-atlas-gen](https://github.com/Chlumsky/msdf-atlas-gen) - For Multi-Signed-Distance-Field atlas generation used for high quality font rendering capabilities.
- [ImGui](https://github.com/ocornut/imgui) - For customizable GUI and docking capabilities.
- [soloud](https://solhsa.com/soloud/) - For audio support
- [entt](https://github.com/skypjack/entt) - For a robust and memory/speed efficient ECS(entity-component system).
- [stb_image](https://github.com/nothings/stb) - For reading images from disk.