# qui
Qt QML bindings for Python



## Build

### Requirements
Compilation is done using CMake.

The following C++ libraries are needed for compilation:
- Qt
- pybind11 (c++ bindings to python)

Qt is most easily installed with the installer program.
pybind11 is available at https://github.com/pybind/pybind11 and needs to be installed using CMake.

Furthermore, the following Python packages are required for building:
- scikit-build (CMake integration into setuptools)


### Steps
1) Make sure cmake and python are found on the path 
```
set PATH=c:\path\to\cmake\bin;c:\path\to\python;%PATH%
```
2) Set the environment variable CMAKE_PREFIX_PATH to point to the location of pybind11 and the Qt binaries,
```
set CMAKE_PREFIX_PATH=c:\path\to\pybind11;c:\path\to\Qt\5.10.0\msvc2017_64
```
3) Run setup.py optionally specificying the CMake generator
```
python setup.py bdist_wheel -G "Visual Studio 15 2017 Win64"
```
4) Install the wheel using pip
```
pip install dist/qui-1.0.0-cp36-cp36m-win_amd64.whl
```
