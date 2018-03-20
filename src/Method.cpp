
#include "Method.h"
#include "Types.h"
#include "Connection.h"

//-----------------------------------------------------------------------------
// Callable
//-----------------------------------------------------------------------------


QMetaMethod Callable::getMethod(QObject* object, const QString& name)
{
    const QMetaObject* meta = object->metaObject();
    QByteArray n = name.toUtf8();

    for (int i = 0; i < meta->methodCount(); i++)
    {
        QMetaMethod m = meta->method(i);
        if (m.name() == n)
            return m;
    }
    return QMetaMethod();
}

QList<QMetaMethod> Callable::getMethods(QObject* object, const QString& name)
{
    const QMetaObject* meta = object->metaObject();
    QByteArray n = name.toUtf8();

    QList<QMetaMethod> methods;

    for (int i = 0; i < meta->methodCount(); i++)
    {
        QMetaMethod m = meta->method(i);
        if (m.name() == n)
            methods.append(m);
    }
    return methods;
}


bool Callable::isCallable(const QMetaMethod& method, const py::tuple& args, const py::dict& kwargs)
{
    std::size_t count = py::len(args);

    if (method.parameterCount() != count)
        return false;

    for (int i = 0; i < method.parameterCount(); i++)
    {
        py::object arg = args[i];
        if (Type<QVariant>::isValidType(method.parameterType(i), arg) == false)
            return false;
    }

    return true;
}

QMetaMethod Callable::getMethodForArgs(const QList<QMetaMethod>& methods, const py::tuple& args, const py::dict& kwargs)
{
    for (const auto& m : methods)
    {
        if (isCallable(m, args, kwargs))
            return m;
    }
    return QMetaMethod{};
}



py::object Callable::invokeMethod(QObject* object, const QMetaMethod& method, const py::tuple& args, const py::dict& kwargs)
{
    if (kwargs.size() != 0)
        throw std::invalid_argument("Keyword arguments not supported.");

    std::size_t count = py::len(args);

    if (count != method.parameterCount())
        throw std::invalid_argument("Invalid number of arguments.");

    QList<QVariant> arguments;
    for (int i = 0; i < count; i++)
    {
        QVariant v = Type<QVariant>::extractType(method.parameterType(i), args[i]);
        arguments.append(v);
    }

    QVariant result;
    bool r = method.invoke(object, Qt::DirectConnection,
        method.returnType() != QMetaType::Void ? Q_RETURN_ARG(QVariant, result) : QGenericReturnArgument(),
        count > 0 ? Q_ARG(QVariant, arguments[0]) : QGenericArgument(),
        count > 1 ? Q_ARG(QVariant, arguments[1]) : QGenericArgument(),
        count > 2 ? Q_ARG(QVariant, arguments[2]) : QGenericArgument(),
        count > 3 ? Q_ARG(QVariant, arguments[3]) : QGenericArgument(),
        count > 4 ? Q_ARG(QVariant, arguments[4]) : QGenericArgument(),
        count > 5 ? Q_ARG(QVariant, arguments[5]) : QGenericArgument(),
        count > 6 ? Q_ARG(QVariant, arguments[6]) : QGenericArgument(),
        count > 7 ? Q_ARG(QVariant, arguments[7]) : QGenericArgument(),
        count > 8 ? Q_ARG(QVariant, arguments[8]) : QGenericArgument(),
        count > 9 ? Q_ARG(QVariant, arguments[9]) : QGenericArgument());

    return Type<QVariant>::toPython(result);
}


py::object Callable::invokeMethod(QObject* object, const QList<QMetaMethod>& methods, const py::tuple& args, const py::dict& kwargs)
{
    QMetaMethod method = getMethodForArgs(methods, args, kwargs);
    if (method.isValid() == false)
        throw std::invalid_argument("Invalid arguments for methods.");

    return invokeMethod(object, method, args, kwargs);
}



//-----------------------------------------------------------------------------
// Method
//-----------------------------------------------------------------------------

Method::Method(QObject* object, const QString& name) :
    _object(object)
{
    for (auto& m : Callable::getMethods(object, name))
    {
        if ((m.methodType() == QMetaMethod::Method) || (m.methodType() == QMetaMethod::Slot))
        {
            _methods.append(m);
        }
    }
}

py::object Method::call(py::args args, py::kwargs kwargs) const
{
    return Callable::invokeMethod(_object, _methods, args, kwargs);
}




//-----------------------------------------------------------------------------
// Signal / SignalConnection
//-----------------------------------------------------------------------------

Signal::Signal(QObject* object, const QString& name) :
    _object(object)
{
    QList<QMetaMethod> m = Callable::getMethods(object, name);
    for (auto& m : Callable::getMethods(object, name))
    {
        if (m.methodType() == QMetaMethod::Signal)
        {
            _signal = m;
            break;
        }
    }
}


py::object Signal::call(py::args args, py::kwargs kwargs) const
{
    return Callable::invokeMethod(_object, _signal, args, kwargs);
}


SignalConnection Signal::connect(const py::object& callable)
{
    Connections* c = _object->findChild<Connections*>(QString(), Qt::FindDirectChildrenOnly);
    if (c == nullptr)
        c = new Connections(_object);

    return SignalConnection(c->connect(_signal, callable));
}


bool Signal::disconnect(const py::object& callable)
{
    Connections* c = _object->findChild<Connections*>(QString(), Qt::FindDirectChildrenOnly);
    if (c == nullptr)
        return false;

    auto items = c->findChildren<Connection*>();
    int count = 0;
    for (auto item : items)
    {
        if (item->callable().is(callable))
        {
            delete item;
            count++;
        }
    }

    return count > 0;
}


int Signal::disconnectAll()
{
    _object->disconnect();

    Connections* c = _object->findChild<Connections*>();
    if (c == nullptr)
        return 0;

    auto items = c->findChildren<Connection*>();
    int count = items.count();
    delete c;
    return count;
}



//-----------------------------------------------------------------------------

SignalConnection::SignalConnection(Connection* c) :
    _connection(c)
{

}


void SignalConnection::disconnect()
{
    if (_connection)
    {
        delete _connection;
        _connection = nullptr;
    }
}