#pragma once

#include "qui.h"
#include "BaseTypes.h"

#include <QVariant>

struct Bool
{
    static py::object toPython(bool value)
    {
        if (value)
            return py::reinterpret_borrow<py::object>(Py_True);
        else
            return py::reinterpret_borrow<py::object>(Py_False);
    }

    static bool fromPython(const py::object& o)
    {
        if (PyObject_IsTrue(o.ptr()) == 1)
            return true;
        else
            return false;
    }

    static bool isValid(const py::object& o)
    {
        return PyBool_Check(o.ptr());
    }
};


struct Long
{
    static bool isValid(const py::object& o)
    {
        return PyLong_Check(o.ptr()) != 0;
    }

    template<class T> static T           fromPython(const py::object& o);
    template<> static char               fromPython(const py::object& o) { return static_cast<char>              (PyLong_AsLong(o.ptr())); }
    template<> static signed char        fromPython(const py::object& o) { return static_cast<signed char>       (PyLong_AsLong(o.ptr())); }
    template<> static unsigned char      fromPython(const py::object& o) { return static_cast<unsigned char>     (PyLong_AsUnsignedLong(o.ptr())); }
    template<> static short              fromPython(const py::object& o) { return static_cast<short>             (PyLong_AsLong(o.ptr())); }
    template<> static unsigned short     fromPython(const py::object& o) { return static_cast<unsigned short>    (PyLong_AsUnsignedLong(o.ptr())); }
    template<> static int                fromPython(const py::object& o) { return static_cast<int>               (PyLong_AsLong(o.ptr())); }
    template<> static unsigned int       fromPython(const py::object& o) { return static_cast<unsigned int>      (PyLong_AsUnsignedLong(o.ptr())); }
    template<> static long               fromPython(const py::object& o) { return static_cast<long>              (PyLong_AsLong(o.ptr())); }
    template<> static unsigned long      fromPython(const py::object& o) { return static_cast<unsigned long>     (PyLong_AsUnsignedLong(o.ptr())); }
    template<> static long long          fromPython(const py::object& o) { return static_cast<long long>         (PyLong_AsLongLong(o.ptr())); }
    template<> static unsigned long long fromPython(const py::object& o) { return static_cast<unsigned long long>(PyLong_AsUnsignedLongLong(o.ptr())); }
    template<> static float              fromPython(const py::object& o) { return static_cast<float>             (PyLong_AsDouble(o.ptr())); }
    template<> static double             fromPython(const py::object& o) { return static_cast<double>            (PyLong_AsDouble(o.ptr())); }

    static py::object toPython(char v)                 { return py::reinterpret_steal<py::object>(PyLong_FromLong(v)); }
    static py::object toPython(signed char v)          { return py::reinterpret_steal<py::object>(PyLong_FromLong(v)); }
    static py::object toPython(unsigned char v)        { return py::reinterpret_steal<py::object>(PyLong_FromUnsignedLong(v)); }
    static py::object toPython(short v)                { return py::reinterpret_steal<py::object>(PyLong_FromLong(v)); }
    static py::object toPython(unsigned short v)       { return py::reinterpret_steal<py::object>(PyLong_FromUnsignedLong(v)); }
    static py::object toPython(int v)                  { return py::reinterpret_steal<py::object>(PyLong_FromLong(v)); }
    static py::object toPython(unsigned int v)         { return py::reinterpret_steal<py::object>(PyLong_FromUnsignedLong(v)); }
    static py::object toPython(long v)                 { return py::reinterpret_steal<py::object>(PyLong_FromLong(v)); }
    static py::object toPython(unsigned long v)        { return py::reinterpret_steal<py::object>(PyLong_FromUnsignedLong(v)); }
    static py::object toPython(long long v)            { return py::reinterpret_steal<py::object>(PyLong_FromLongLong(v)); }
    static py::object toPython(unsigned long long v)   { return py::reinterpret_steal<py::object>(PyLong_FromUnsignedLong(v)); }
    static py::object toPython(float v)                { return py::reinterpret_steal<py::object>(PyLong_FromDouble(v)); }
    static py::object toPython(double v)               { return py::reinterpret_steal<py::object>(PyLong_FromDouble(v)); }


    static QVariant variantFromPython(const py::object& o)
    {
        int overflow = 0;
        long v = PyLong_AsLongAndOverflow(o.ptr(), &overflow);
        if (overflow == 0)
            return QVariant(v);

        overflow = 0;
        long long v2 = PyLong_AsLongLongAndOverflow(o.ptr(), &overflow);
        if (overflow == 0)
            return QVariant(v2);

        throw std::runtime_error("Overflow in extracting long.");
    }
};


struct Float
{
    static bool isValid(const py::object& o)
    {
        return PyFloat_Check(o.ptr()) != 0;
    }

    template<class T> static T fromPython(const py::object& o)
    {
        return static_cast<T>(PyFloat_AsDouble(o.ptr()));
    }

    template<class T> static py::object toPython(T value)
    {
        static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value, "Numeric type required");
        return py::reinterpret_steal<py::object>(PyFloat_FromDouble(static_cast<double>(value)));
    }
};


struct Number
{
    static bool isValid(const py::object& o)
    {
        return PyNumber_Check(o.ptr()) != 0;
    }


    template<class T, typename std::enable_if_t<std::is_integral_v<T>>* = nullptr>
    static T fromPython(const py::object& o)
    {
        if (Long::isValid(o))
            return Long::fromPython<T>(o);
        else if (Float::isValid(o))
            return Float::fromPython<T>(o);
        else
        {
            if (py::object l = py::reinterpret_steal<py::object>(PyNumber_Long(o.ptr())))
                return Long::fromPython<T>(l);
            else if (auto f = py::reinterpret_steal<py::object>(PyNumber_Float(o.ptr())))
                return Float::fromPython<T>(f);
        }
        throw std::invalid_argument("Not a numeric type.");
    }


    template<class T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    static T fromPython(const py::object& o)
    {
        if (Float::isValid(o))
            return Float::fromPython<T>(o);
        else if (Long::isValid(o))
            return Long::fromPython<T>(o);
        else
        {
            if (auto f = py::reinterpret_steal<py::object>(PyNumber_Float(o.ptr())))
                return Float::fromPython<T>(f);
            else if (auto l = py::reinterpret_steal<py::object>(PyNumber_Long(o.ptr())))
                return Long::fromPython<T>(l);
        }
        throw std::invalid_argument("Not a numeric type.");
    }

    template<class T, typename std::enable_if_t<std::is_integral_v<T>>* = nullptr>
    static py::object toPython(T v) { return Long::toPython(v); }


    template<class T, typename std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
    static py::object toPython(T v) { return Float::toPython(v); }
};


template<class T>
struct NumberType
{
    static py::object toPython(T v)
    {
        return Number::toPython(v);
    }

    static T fromPython(const py::object& o)
    {
        return Number::fromPython<T>(o);
    }

    static bool isValid(const py::object& o)
    {
        return Number::isValid(o);
    }
};


template<> struct Type<bool>               : public Bool {};
template<> struct Type<char>               : public NumberType<char>  {};
template<> struct Type<signed char>        : public NumberType<signed char> {};
template<> struct Type<unsigned char>      : public NumberType<unsigned char> {};
template<> struct Type<short>              : public NumberType<short> {};
template<> struct Type<unsigned short>     : public NumberType<unsigned short> {};
template<> struct Type<int>                : public NumberType<int> {};
template<> struct Type<unsigned int>       : public NumberType<unsigned int> {};
template<> struct Type<long>               : public NumberType<long> {};
template<> struct Type<unsigned long>      : public NumberType<unsigned long> {};
template<> struct Type<long long>          : public NumberType<long long> {};
template<> struct Type<unsigned long long> : public NumberType<unsigned long long> {};
template<> struct Type<float>              : public NumberType<float> {};
template<> struct Type<double>             : public NumberType<double> {};
