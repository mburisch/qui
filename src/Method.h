#pragma once

#include "qui.h"

#include <QVariant>
#include <QMetaType>
#include <QMetaMethod>
#include <QMetaObject>
#include <QObject>
#include <QPointer>

class Connection;

class Callable
{
public:
    static QList<QMetaMethod> getMethods(QObject* object, const QString& name);
    static QMetaMethod getMethod(QObject* object, const QString& name);

    static bool isCallable(const QMetaMethod& method, const py::tuple& args, const py::dict& kwargs);
    static QMetaMethod getMethodForArgs(const QList<QMetaMethod>& methods, const py::tuple& args, const py::dict& kwargs);
    static py::object invokeMethod(QObject* object, const QMetaMethod& method, const py::tuple& args, const py::dict& kwargs);
    static py::object invokeMethod(QObject* object, const QList<QMetaMethod>& methods, const py::tuple& args, const py::dict& kwargs);

    template<class T> static py::object call(py::object self, py::args args, py::kwargs kwargs)
    {
        return self.call(args, kwargs);
    }
};

class Method
{
public:
    Method(QObject* object, const QString& name);

    operator bool() const { return _methods.size() > 0; }
    py::object call(py::args args, py::kwargs kwargs) const;

protected:
    QObject* _object;
    QList<QMetaMethod> _methods;
};


class SignalConnection
{
public:
    SignalConnection(Connection* c);

    void disconnect();

private:
    QPointer<Connection> _connection;
};


class Signal
{
public:
    Signal(QObject* object, const QString& name);

    operator bool() const { return _signal.isValid(); }

    py::object call(py::args args, py::kwargs kwargs) const;

    SignalConnection connect(const py::object& callable);
    bool disconnect(const py::object& callable);
    int disconnectAll();

protected:
    QObject* _object;
    QMetaMethod _signal;
};


