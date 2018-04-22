# Example

The following example shows how Python can access QML objects and properties. Access to object is via objectName as the id property is not accessible.

example.py
```
import qui
import numpy as np
import time

app = qui.QApplication()

def add():
    a = float(root.textA.text)
    b = root.value
    c = root.func(a, b)
    root.textResult.text = str(c)

def update_image():
    image = 255 * (0.5 + 0.5 * np.sin(np.arange(1024)  / 1024 * 4 * np.pi + time.time()))
    image = np.tile(image, [1024, 1])
    root.image.image = image

view = qui.QQuickView("example.qml")
view.show()

root = view.root_object()

root.btnAdd.clicked.connect(add)
root.btnImage.clicked.connect(update_image)


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
    width: 600
    height: 600

    property int value: 1

    function func(a, b) {
        console.log("Func", a, b);
        return a + b
    }

    ColumnLayout  {
        x: 10
        y: 10
        spacing: 2
        RowLayout {
            TextField {
                objectName: "textA"
                text: "5"
            }
            Button {
              objectName: "btnAdd"
              text: "Add"
            }
              TextField {
                objectName: "textResult"
                readOnly: true
            }
        }

        Button {
            objectName: "btnImage"
            text: "Update image"
        }

        // ImageView is provided by qui. It has a property image of type QImage.
        ImageView {
            id: image
            objectName: "image"
            width: 500
            height: 500
        }
    }
}
```


