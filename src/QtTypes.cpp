
#include "Types.h"

#include <QMetaObject>
#include <QMetaMethod>
#include <QObject>
#include <QCoreApplication>
#include <QGuiApplication>
#include <QApplication>
#include <QTimer>
#include <QVector>
#include <QByteArray>
#include <QQmlProperty>

#include <QDebug>

#include "Utility.h"
#include "Method.h"
#include "Connection.h"


struct QMetaObjectImpl
{
    static py::list getMethods(const QMetaObject* self)
    {
        py::list list;
        for (int i = 0; i < self->methodCount(); i++)
        {
            QMetaMethod m = self->method(i);
            if ((m.methodType() != QMetaMethod::Method) && (m.methodType() != QMetaMethod::Slot))
                continue;

            list.append(QString::fromLatin1(m.methodSignature()));
        }

        return list;
    }

    static py::list getProperties(const QMetaObject* self)
    {
        py::list list;
        for (int i = 0; i < self->propertyCount(); i++)
        {
            list.append(QString::fromLatin1(self->property(i).name()));
        }

        return list;
    }

    static py::list getSignals(const QMetaObject* self)
    {
        py::list list;
        for (int i = 0; i < self->methodCount(); i++)
        {
            QMetaMethod m = self->method(i);
            if (m.methodType() != QMetaMethod::Signal)
                continue;

            list.append(QString::fromLatin1(m.methodSignature()));
        }

        return list;
    }
};






struct QObjectImpl
{
    [[noreturn]] static void throwAttributeError(const QString& message)
    {
        PyErr_SetString(PyExc_AttributeError, message.toUtf8().constData());
        throw py::error_already_set();
    }

    static py::object getattr(QObject& self, const QString& name)
    {
        // If name is a property of the object
        QQmlProperty prop(&self, name);

        // If it is a regular property (normal or QObject)
        if (prop.isProperty())
            return Type<QVariant>::toPython(prop.read());

        // If it is a child
        else if (QObject* child = self.findChild<QObject*>(name))
            return Type<QObject*>::toPython(child);

        // If it is a method
        else if (Method m = Method(&self, name))
            return py::cast(m);

        // If it is a signal
        else if (Signal s = Signal(&self, name))
            return py::cast(s);
                
        PyErr_SetString(PyExc_AttributeError, name.toUtf8().constData());
        throw py::error_already_set();
    }


    static void setattr(QObject& self, const QString& name, const py::object& o)
    {
        QQmlProperty prop(&self, name);
        if (prop.isValid() == false)
            throwAttributeError(name);

        
        if (prop.propertyTypeCategory() == QQmlProperty::Object)
        {
            throwAttributeError("Cannot assign property of type QObject");
        }
        if (prop.propertyTypeCategory() == QQmlProperty::List)
        {
            throwAttributeError("Cannot assign property of type QQmlListProperty");
        }
        else if (prop.propertyTypeCategory() == QQmlProperty::Normal)
        {
            prop.write(Type<QVariant>::extractType(prop.propertyType(), o));
        }
    }

    static QList<QObject*> findChildren(QObject& self, const QString& name, bool recursive = true)
    {
        QList<QObject*> childs = self.findChildren<QObject*>(name, recursive ? Qt::FindChildrenRecursively : Qt::FindDirectChildrenOnly);
        return childs;
    }
};


namespace
{
    template<class T>
    T* applicationConstructor(const py::list& args)
    {
        ApplicationArguments* arguments = new ApplicationArguments(args);
        T* app = new T(arguments->argc, arguments->argv.data());
        QObject::connect(app, &QObject::destroyed, [arguments](QObject*) { delete arguments; });
        return app;
    }
}

void initHelperTypes(py::module& m)
{
    py::class_<Method>(m, "Method")
        .def("__call__", &Method::call)
        ;

    py::class_<Signal>(m, "Signal")
        .def("__call__", &Signal::call)
        .def("connect", &Signal::connect)
        .def("disconnect", &Signal::disconnect)
        .def("disconnect", &Signal::disconnectAll)
        ;

    py::class_<SignalConnection>(m, "SignalConnection")
        .def("disconnect", &SignalConnection::disconnect)
        ;
}


void initQtTypes(py::module& m)
{
    py::module types = m.def_submodule("types");
    
    initHelperTypes(types);
    
    py::class_<QMetaObject>(m, "QMetaObject")
        .def_property_readonly("class_name", &QMetaObject::className, py::return_value_policy::reference)
        .def_property_readonly("super_class", &QMetaObject::superClass, py::return_value_policy::reference)
        .def("methods", &QMetaObjectImpl::getMethods)
        .def("signals", &QMetaObjectImpl::getSignals)
        .def("properties", &QMetaObjectImpl::getProperties)
        ;

    py::class_<QObject>(m, "QObject")
        .def(py::init<>())
        .def("meta_object", &QObject::metaObject, py::return_value_policy::reference)
        .def("parent", &QObject::parent, py::return_value_policy::reference)
        .def_property("parent", &QObject::parent, &QObject::setParent, py::return_value_policy::reference)
        .def("property", &QObject::property)
        .def("set_property", &QObject::setProperty)
        .def("find_children", &QObjectImpl::findChildren)
        .def("__getattr__", &QObjectImpl::getattr)
        .def("__setattr__", &QObjectImpl::setattr)
        ;


    py::class_<QCoreApplication, QObject>(m, "QCoreApplication")
        .def(py::init(&applicationConstructor<QCoreApplication>), py::arg("args") = py::list())
        .def_static("add_library_path", &QCoreApplication::addLibraryPath)
        .def_static("exec", &QCoreApplication::exec)
        ;

    py::class_<QGuiApplication, QCoreApplication>(m, "QGuiApplication")
        .def(py::init(&applicationConstructor<QGuiApplication>), py::arg("args") = py::list())
        .def_static("exec", &QGuiApplication::exec)
        ;


    py::class_<QApplication, QGuiApplication>(m, "QApplication")
        .def(py::init(&applicationConstructor<QApplication>), py::arg("args") = py::list())
        .def_static("exec", &QApplication::exec)
        ;

    
    py::class_<QTimer, QObject>(m, "QTimer")
        .def(py::init<>())
        .def_static("single_shot", [](int msec, const py::object& callable) { Require::callable(callable); QTimer::singleShot(msec, [callable]() { callable(); }); })
        ;

}