
#include "Connection.h"
#include "Types.h"

Connections::Connections(QObject* parent) :
    QObject(parent)
{
}


Connection* Connections::connect(const QMetaMethod& signal, const py::object& callable)
{
    Connection* c = new Connection(parent(), signal, callable);
    c->setParent(this);
    return c;
}




Connection::Connection(QObject* sender, const QMetaMethod& signal, const py::object& callable) :
    _signal(signal),
    _callable(callable)
{
    Require::callable(_callable);

    const QMetaObject* srcMeta = sender->metaObject();
    const QMetaObject* dstMeta = this->metaObject();

    QStringList args = QVector<QString>(signal.parameterCount(), QLatin1String("void*&")).toList();
    QString method = QLatin1String("receive(") + args.join(QLatin1Char(',')) + QLatin1Char(')');
    int dstIdx = dstMeta->indexOfMethod(method.toLatin1().data());
    if (dstIdx < 0)
        throw std::runtime_error("No slot for signal: " + std::string(signal.name().constData()));

    QMetaMethod dstMethod = dstMeta->method(dstIdx);

    _connection = QMetaObject::connect(sender, signal.methodIndex(), this, dstIdx, Qt::DirectConnection);
}


Connection::~Connection()
{
    disconnect();
}


void Connection::disconnect()
{
    QObject::disconnect(_connection);
    _connection = QMetaObject::Connection();
}


void Connection::dispatch(const QVector<void**>& args)
{
    py::tuple arguments(args.size());
    for (int i = 0; i < args.size(); i++)
    {
        QVariant v = getArg(*args[i], _signal.parameterType(i));
        arguments[i] = Type<QVariant>::toPython(v);
    }

    _callable(*arguments);
}



QVariant Connection::getArg(void*& v, int type)
{
    if (type == QMetaType::QVariant)
        return *((QVariant*)&v);
    else
        return QVariant(type, &v);
}