# libuipc
A Modern C++20 Library of Unified Incremental Potential Contact.

## Dependencies

| Name                                   | Version | Usage                                               | Import         |
| -------------------------------------- | ------- | --------------------------------------------------- | -------------- |
| [muda](https://github.com/MuGdxy/muda) | -       | improve safety and readability of CUDA programming. | submodule      |
| cuda                                   | >=12.0  | GPU programming                                     | system install |
| eigen3                                 | 3.4.0   | matrix calculation                                  | package        |
| catch2                                 | 3.5.3   | unit tests                                          | package        |
| libigl                                 | 2.5.0   | mesh processing                                     | package        |
| rapidcsv                               | 8.80    | csv file IO                                         | package        |
| spdlog                                 | 1.12.0  | logging                                             | package        |
| fmt                                    | 10.1.1  | fast string formatting                              | package        |
| cppitertools                           | 2.1#3   | python-like iteration tools                         | package        |
| bgfx                                   | 1.127#1 | cross-platform RHI                                  | package        |
| dylib                                  | 2.2.1   | cross-platform dynamic library loader               | package        |
| benchmark                              | 1.8.3#3 | microbenchmark support library                      | package        |
| nlohmann_json                          | 3.11.2  | json file IO                                        | package        |
| imgui                                  | 1.90.7  | GUI                                                 | package        |
| glfw3                                  | 3.3.8#2 | window management                                   | package        |

## Build
We use [vcpkg](https://github.com/microsoft/vcpkg) to manage the libraries we need and use [CMake](https://cmake.org/) to build the project. The simplest way to let CMake detect vcpkg is to set the system environment variable `CMAKE_TOOLCHAIN_FILE` to `(YOUR_VCPKG_PARENT_FOLDER)/vcpkg/scripts/buildsystems/vcpkg.cmake`

CMake minimum version: 3.27

Vcpkg supports both windows and linux. We recommend using vcpkg to manage the dependencies to keep the consistency of the development environment.

### Automatic Dependency Installation

We use `vcpkg.json` to manage the dependencies. Normally, when you build with CMake, the dependencies will be automatically installed by vcpkg.

If any problem occurs, run the following command:
```shell
vcpkg install
```
under the project root directory to see if the dependencies are installed correctly. 

If the automatic installation fails, please raise an issue with the error message given by the above command.

You can also try install the dependencies [manually](#manaully-dependency-installation).

### Build Project

#### Windows
On windows, you can use the `CMake-GUI` to configure the project and generate the Visual Studio solution file. Almost no effort is needed.

#### Linux
On linux, you can use the following commands to build the project.
```shell
cd libuipc; cd ..; mkdir CMakeBuild; cd CMakeBuild
cmake -S ../libuipc -DUIPC_BUILD_GUI=0
cmake --build .
```
Note: A compile problem of bgfx(1.127#1) is found. So now we just turn off the GUI support on linux. [TODO] Maybe we can fix it in the future.

Install the project
```shell
cmake --install . --config <Debug/Release/RelWithDebInfo>
```
Run the program.
Note, you need to setup the `LD_LIBRARY_PATH`, or the shared uipc library and the dependent backends will not be found.
```shell
cd <Debug/Release/RelWithDebInfo>/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./
./hello_uipc
./uipc_test_world
...
```

### Build Document

Install [mkdocs](https://www.mkdocs.org/)
```shell
pip install mkdocs mkdocs-material mkdocs-literate-nav
```

Download [doxide](https://www.doxide.org/installation/), and add the `doxide` binary folder to the system path.

Run the following command at the root of the project:
```shell
doxide build
```

Turn on the local server:
```shell
mkdocs serve
```

## Troubleshooting

### Mannaully Dependency Installation
If the automatic installation fails, you can manually install the dependencies, for a temporary workaround.

First, remove the installed dependencies and the `vcpkg.json` file by:
```shell
cd libuipc
rm -r ./vcpkg_installed
rm ./vcpkg.json
```
Thus the vcpkg manifest mode is disabled.

Then, install the dependencies manually.
```shell
git submodule update --init
vcpkg install eigen3 catch2 spdlog libigl fmt cppitertools dylib rapidcsv benchmark nlohmann-json imgui glfw3
```

GUI support. (Optional) On linux it may fail to build bgfx.
```shell
vcpkg install bgfx
```