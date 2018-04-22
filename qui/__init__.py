import os
import os.path

if os.name == "nt":
    _root_path = os.path.abspath(os.path.dirname(__file__))
    # Make sure the shared libraries, platform plugin and qml modules are found
    os.environ["PATH"] = _root_path + "/Qt/bin" + ";" + os.environ["PATH"]
    os.environ["QT_PLUGIN_PATH"] = _root_path + "/Qt/plugins"
    os.environ["QML2_IMPORT_PATH"] = _root_path + "/Qt/qml"

from .qui_ import *


#QCoreApplication.add_library_path(os.path.dirname(inspect.getfile(qui_)) + "/plugins")
