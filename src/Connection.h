
#pragma once

#include "qui.h"

#include <QObject>
#include <QMetaObject>
#include <QMetaMethod>
#include <QVariant>
#include <QVector>

class Connection;


class Connections : public QObject
{
    Q_OBJECT
public:
    Connections(QObject* parent);

    Connection* connect(const QMetaMethod& signal, const py::object& callable);
};


// Object for making signal/slot connections
class Connection : public QObject
{
    Q_OBJECT

private:
    static QVariant getArg(void*& v, int type);

public:
    Connection(QObject* sender, const QMetaMethod& signal, const py::object& callable);
    ~Connection();

    py::object callable() const { return _callable; }

public Q_SLOTS:
    void receive() { dispatch({}); }
    void receive(void*& v1) { dispatch({ &v1 }); }
    void receive(void*& v1, void*& v2) { dispatch({ &v1, &v2 }); }
    void receive(void*& v1, void*& v2, void*& v3) { dispatch({ &v1, &v2, &v3 }); }
    void receive(void*& v1, void*& v2, void*& v3, void*& v4) { dispatch({ &v1, &v2, &v3, &v4 }); }
    void receive(void*& v1, void*& v2, void*& v3, void*& v4, void*& v5) { dispatch({ &v1, &v2, &v3, &v4, &v5 }); }

private:
    void disconnect();
    void dispatch(const QVector<void**>& args);

private:
    py::object _callable;
    const QMetaMethod _signal;
    QMetaObject::Connection _connection;
};


