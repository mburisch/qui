

#include "Types.h"

#include <QQuickWindow>
#include <QQuickItem>
#include <QQuickPaintedItem>
#include <QQuickView>
#include <QQmlEngine>
#include <QQuickStyle>
#include <QQuickImageProvider>


class PyQuickImageProvider : public QQuickImageProvider
{
public:
    PyQuickImageProvider(const py::object& callable) :
        QQuickImageProvider(QQmlImageProviderBase::Image),
        _callable(callable)
    {
        Require::callable(_callable);
    }

    QImage requestImage(const QString& id, QSize* size, const QSize& requestedSize) override
    {
        py::tuple r = _callable(id, requestedSize.isValid() ? py::cast(requestedSize) : py::none());
        py::object image = r[0];
        py::object s = r[1];

        *size = Type<QSize>::fromPython(s);
        return Type<QImage>::fromPython(image);
    }

private:
    py::object _callable;
};


void initQMLTypes(py::module& m)
{
    py::class_<QQuickStyle>(m, "QQuickStyle")
        .def_static("available_styles", &QQuickStyle::availableStyles)
        .def_static("name", &QQuickStyle::name)
        .def_static("set_style", &QQuickStyle::setStyle)
        ;

    py::class_<QQuickView, QObject>(m, "QQuickView")
        .def(py::init<>())
        .def(py::init<QUrl>())
        .def("root_object", &QQuickView::rootObject, py::return_value_policy::reference)
        .def_property("source", &QQuickView::source, &QQuickView::setSource)
        .def("size", &QQuickView::size)
        .def("engine", &QQuickView::engine, py::return_value_policy::reference)
        ;

    py::class_<QQmlEngine, QObject>(m, "QQmlEngine")
        .def(py::init<>())
        .def("add_image_provider", [](QQmlEngine& self, const QString& providerId, const py::object& callable) { self.addImageProvider(providerId, new PyQuickImageProvider(callable)); })
        .def("add_import_path", &QQmlEngine::addImportPath)
        ;

    py::class_<QQuickItem, QObject>(m, "QQuickItem")
        .def(py::init<>())
        ;

    py::class_<QQuickPaintedItem, QQuickItem>(m, "QQuickPaintedItem")
        ;

}


