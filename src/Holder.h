#pragma once

#include "qui.h"

#include <QVariant>
#include <QMetaType>

// Allows to store "typed" QVariant data in a Python type
class VariantHolder
{
public:
    template<class T> static VariantHolder create(const py::object& o)
    {
        return VariantHolder(QVariant::fromValue(Type<T>::fromPython(o))); 
    }

public:
    VariantHolder() = default;
    VariantHolder(const QVariant& v) : _value(v) {}
    VariantHolder(QVariant&& v) : _value(v) {}

    operator QVariant() const { return _value; }
    QVariant get() const { return _value; }
    QString typeName() const { return _value.typeName(); }
    void clear() { _value.clear(); }
    bool isValid() const { return _value.isValid(); }

    QString str() const { return QString("VariantHolder(%1)").arg(_value.typeName()); }

private:
    QVariant _value;
};


// Class to store in a QVariant for PyObject* values.
// The only reason to use this class instead of directly storing a py::object
// inside a QVariant is the conversion function. For non-member functions
// QMetaType only supports non-failing conversions, which cannot be
// guaranteed. 
class ObjectHolder
{
public:
    ObjectHolder() = default;
    ObjectHolder(const py::object& o) : _object(o) {}

    py::object get() const { return _object; }
    PyObject* ptr() const { return _object.ptr(); }

    QString repr() const
    {
        py::object repr = _object.attr("__repr__")();
        return QString::fromStdString(py::cast<std::string>(repr));
    }


    template<class T> T convert(bool* ok) const
    {
        if (Type<T>::isValid(_object))
        {
            *ok = true;
            return Type<T>::fromPython(_object);
        }
        else
        {
            return failedConversion<T>(ok);
        }
    }
    
private:
    // This is used to be able to return a string representation of the object.
    // QVariant uses a QString converison to get a textual representation of the object.
    // In case the object is not a string, failedConversion<> is called. 
    // We specialize the QString overload to return the result of "__repr__" as the value
    // of the conversion. This can be used in console.log() from QML to show the type of the
    // object. Also this means every PyObject can be converted to QString.

    template<class T> T failedConversion(bool* ok) const
    {
        *ok = true;
        return T{};
    }

    template<> QString failedConversion<QString>(bool* ok) const
    {
        *ok = true;
        return repr();
    }


private:
    py::object _object;
};

Q_DECLARE_METATYPE(ObjectHolder)
