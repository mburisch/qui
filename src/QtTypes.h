#pragma once

#include "qui.h"

#include <QVariant>
#include <QJSValue>
#include <QJSValueIterator>

#include "QtSimpleTypes.h"
#include "Holder.h"

template<> struct Type<QVariant>;
template<> struct Type<QJSValue>;


py::object variantToPython(const QVariant& v);

//-----------------------------------------------------------------------------
// VariantHolder
//-----------------------------------------------------------------------------

template<>
struct Type<VariantHolder>
{
    static py::object toPython(const VariantHolder& v)
    {
        return py::cast(v);
    }


    static VariantHolder fromPython(const py::object& o)
    {
        return py::cast<VariantHolder>(o);
    }

    static QVariant get(const py::object& o)
    {
        return py::cast<VariantHolder>(o).get();
    }

    static bool isValid(const py::object& o)
    {
        return py::isinstance<VariantHolder>(o);
    }
};



//-----------------------------------------------------------------------------
// QJSValue / QVariant
//-----------------------------------------------------------------------------



template<>
struct Type<QJSValue>
{
    static py::object toPython(const QJSValue& v)
    {
        if (v.isNull())
            return py::none();
        else if (v.isNumber())
            return Float::toPython(v.toNumber());
        else if (v.isBool())
            return Bool::toPython(v.toBool());
        else if (v.isString())
            return Type<QString>::toPython(v.toString());
        else if (v.isDate())
            return Type<QDateTime>::toPython(v.toDateTime());
        else if (v.isArray())
        {
            // Array needs to be before isObject, because an array is also an object
            py::list list;
            int len = v.property("length").toInt();
            for (int i = 0; i < len; i++)
                list.append(Type<QJSValue>::toPython(v.property(i)));

            return list;
        }
        else if (v.isVariant())
            return variantToPython(v.toVariant());
        else if (v.isQObject())
            return Type<QObject*>::toPython(v.toQObject());
        else if (v.isQMetaObject())
            return Type<QMetaObject*>::toPython(v.toQMetaObject());
        else if (v.isObject())
        {
            py::dict dict;

            QJSValueIterator it(v);
            while (it.hasNext())
            {
                it.next();
                py::object key = Type<QString>::toPython(it.name());
                py::object value = Type<QJSValue>::toPython(it.value());
                dict[key] = value;
            }

            return dict;
        }
        else
            throw std::invalid_argument("Unsupported QJSValue");
    }

    
    static QJSValue fromPython(const py::object& o)
    {
        throw std::runtime_error("Cannot convert QJSValue");
    }
    

    static bool isValid(const py::object& o)
    {
        return false;
    }
};



template<>
struct Type<QVariant>
{
    static py::object toPython(const QVariant& v)
    {
        return Conversion::convertObject<py::object, const QVariant&, Conversion::Py>(v.userType(), v);
    }

    static QVariant fromPython(const py::object& o)
    {
        // If the PyObject is a basic type extract it, otherwise store the object

        if (Bool::isValid(o))
            return Bool::fromPython(o);

        if (Long::isValid(o))
            return Long::variantFromPython(o);

        if (Float::isValid(o))
            return Float::fromPython<double>(o);

        if (Type<QString>::isValid(o))
            return Type<QString>::fromPython(o);

        if (Type<QDateTime>::isValid(o))
            return Type<QDateTime>::fromPython(o);

        if (Type<VariantHolder>::isValid(o))
            return Type<VariantHolder>::get(o);
        
        return QVariant::fromValue(ObjectHolder(o));
    }

    static bool isValid(const py::object& o)
    {
        // Every Python object can be converted into a QVariant
        return true;
    }


    static QVariant extractType(int dataType, const py::object& o)
    {
        return Conversion::convertObject<QVariant, py::object, Conversion::QVar>(dataType, o);
    }

    static bool isValidType(int dataType, const py::object& o)
    {
        return Conversion::convertObject<bool, py::object, Conversion::Valid>(dataType, o);
    }

    struct Conversion
    {
        // PyObject*
        struct Py
        {
            static py::object unsupported(const QVariant&) { throw std::invalid_argument("Unsupported type"); }

            template<class T> static py::object create(const QVariant& v) { return Type<T>::toPython(v.value<T>()); }
            template<>        static py::object create<void>(const QVariant& v) { return py::none(); }
        };

        // QVariant
        struct QVar
        {
            static QVariant unsupported(const py::object&) { throw std::invalid_argument("Unsupported type"); }

            template<class T> static QVariant create(const py::object& o) { return QVariant::fromValue(Type<T>::fromPython(o)); }
            template<>        static QVariant create<void>(const py::object&) { return QVariant(); }
            template<>        static QVariant create<QVariant>(const py::object& o) { return Type<QVariant>::fromPython(o); }
        };

        struct Valid
        {
            static bool unsupported(const py::object&) { return false; }

            template<class T> static bool create(const py::object&) { return true; }
        };


        template<class R, class V, class Conv>
        static R convertObject(int dataType, V v)
        {
            switch (dataType)
            {
                case QMetaType::Void:           return Conv::create<void>(v);
                case QMetaType::UnknownType:    return Conv::create<void>(v);

                case QMetaType::QVariant:       return Conv::create<QVariant>(v);

                case QMetaType::Bool:           return Conv::create<bool>(v);
                case QMetaType::Char:           return Conv::create<char>(v);
                case QMetaType::SChar:          return Conv::create<signed char>(v);
                case QMetaType::UChar:          return Conv::create<unsigned char>(v);
                case QMetaType::Short:          return Conv::create<short>(v);
                case QMetaType::UShort:         return Conv::create<unsigned short>(v);
                case QMetaType::Int:            return Conv::create<int>(v);
                case QMetaType::UInt:           return Conv::create<unsigned int>(v);
                case QMetaType::Long:           return Conv::create<long>(v);
                case QMetaType::ULong:          return Conv::create<unsigned long>(v);
                case QMetaType::LongLong:       return Conv::create<long long>(v);
                case QMetaType::ULongLong:      return Conv::create<unsigned long long>(v);
                case QMetaType::Float:          return Conv::create<float>(v);
                case QMetaType::Double:         return Conv::create<double>(v);

                case QMetaType::QString:        return Conv::create<QString>(v);
                case QMetaType::QUrl:           return Conv::create<QUrl>(v);
                case QMetaType::QByteArray:     return Conv::create<QByteArray>(v);

                case QMetaType::QSize:          return Conv::create<QSize>(v);
                case QMetaType::QSizeF:         return Conv::create<QSizeF>(v);
                case QMetaType::QRect:          return Conv::create<QRect>(v);
                case QMetaType::QRectF:         return Conv::create<QRectF>(v);
                case QMetaType::QPoint:         return Conv::create<QPoint>(v);
                case QMetaType::QPointF:        return Conv::create<QPointF>(v);

                case QMetaType::QMatrix:        return Conv::create<QMatrix>(v);
                case QMetaType::QMatrix4x4:     return Conv::create<QMatrix4x4>(v);

                case QMetaType::QTransform:     return Conv::create<QTransform>(v);
                case QMetaType::QQuaternion:    return Conv::create<QQuaternion>(v);

                case QMetaType::QVector2D:      return Conv::create<QVector2D>(v);
                case QMetaType::QVector3D:      return Conv::create<QVector3D>(v);
                case QMetaType::QVector4D:      return Conv::create<QVector4D>(v);

                case QMetaType::QPolygon:       return Conv::create<QPolygon>(v);
                case QMetaType::QPolygonF:      return Conv::create<QPolygonF>(v);

                case QMetaType::QColor:         return Conv::create<QColor>(v);
                case QMetaType::QDateTime:      return Conv::create<QDateTime>(v);

                case QMetaType::QImage:         return Conv::create<QImage>(v);

                case QMetaType::QObjectStar:    return Conv::create<QObject*>(v);

                /*
                case QMetaType::QJsonValue:     return Converter<QJsonValue>::create(v.value<QJsonValue>());
                case QMetaType::QJsonObject:    return Converter<QJsonObject>::create(v.value<QJsonObject>());

                case QMetaType::QJsonArray:     return Converter<QJsonArray>::create(v.value<QJsonArray>());
                case QMetaType::QJsonDocument:  return Converter<QJsonDocument>::create(v.value<QJsonDocument>());

                case QMetaType::QStringList:    return Converter<QStringList>::toPython(v.value<QStringList>());
                case QMetaType::QVariantHash:   return Converter<QVariantHash>::toPython(v.value<QVariantHash>());
                case QMetaType::QVariantMap:    return Converter<QVariantMap>::toPython(v.value<QVariantMap>());
                */
            }

            if (QMetaType::typeFlags(dataType) & QMetaType::PointerToQObject) return Conv::create<QObject*>(v);

            if (dataType == qMetaTypeId<ObjectHolder>())        return Conv::create<ObjectHolder>(v);
            if (dataType == qMetaTypeId<py::object>())          return Conv::create<py::object>(v);
            if (dataType == qMetaTypeId<QJSValue>())            return Conv::create<QJSValue>(v);
            if (dataType == qMetaTypeId<QVector<QPoint>>())     return Conv::create<QVector<QPoint>>(v);
            if (dataType == qMetaTypeId<QVector<QPointF>>())    return Conv::create<QVector<QPointF>>(v);
            if (dataType == qMetaTypeId<QVector<QVector2D>>())  return Conv::create<QVector<QVector2D>>(v);
            if (dataType == qMetaTypeId<QVector<QVector3D>>())  return Conv::create<QVector<QVector3D>>(v);
            if (dataType == qMetaTypeId<QVector<QVector4D>>())  return Conv::create<QVector<QVector4D>>(v);

            return Conv::unsupported(v);
        }
    };
};


inline py::object variantToPython(const QVariant& v)
{
    return Type<QVariant>::toPython(v);
}



