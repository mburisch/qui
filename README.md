# qui

Qt QML bindings for Python

This packet allows interaction between QML user interfaces and Python. Therefore, a set of relevant Qt classes are exposed to Python.


## Example

The following example shows how Python can access QML objects and properties. Access to object is via objectName as the id property is not accessible. All properties, signals and slots of QObject types are accessible. 

example.py
```
import qui
import numpy as np

app = qui.QApplication()

def show_click():
    print("Button clicked)

view = qui.QQuickView("example.qml")
view.show()
root = view.root_object()
root.btn.clicked.connect(show_click)
app.exec()

```

example.qml
```
import QtQuick 2.8
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import qui 1.0

Item {
    id: root
    
     Button {
         objectName: "btn"
         text: "Add"
     }
}
```




## Build

### Requirements
Compilation is done using CMake.

The following C++ libraries are needed for compilation:
- Qt
- pybind11 (c++ bindings to python)

Qt is most easily installed with the installer program.
pybind11 is available at https://github.com/pybind/pybind11. The library is included via CMake find_package, hence it needs to be "build".

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
