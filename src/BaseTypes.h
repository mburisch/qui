#pragma once

#include "qui.h"
#include "Holder.h"


struct Require
{
    static void callable(const py::object& callable)
    {
        if (PyCallable_Check(callable.ptr()) == 0)
            throw std::invalid_argument("Object not callable");
    }
};


/*
struct Types
{
    template<class T> static constexpr const char* name()
    {
        return QMetaType::typeName(qMetaTypeId<T>());
    }
};
*/

//#define TYPE_NAME(T) static constexpr const char* name = Types::name<T>()



template<class T> struct Type;

template<>
struct Type<void>
{
    static py::object toPython()
    {
        return py::none();
    }
};


template<>
struct Type<PyObject*>
{
    static py::object toPython(PyObject* o)
    {
        return py::reinterpret_borrow<py::object>(o);
    }

    static PyObject* fromPython(const py::object& o)
    {
        return o.inc_ref().ptr();
    }

    static bool isValid(const py::object&)
    {
        return true;
    }
};


template<>
struct Type<py::object>
{
    static py::object toPython(const py::object& o)
    {
        return o;
    }

    static py::object fromPython(const py::object& o)
    {
        return o;
    }

    static bool isValid(const py::object&)
    {
        return true;
    }
};




template<>
struct Type<ObjectHolder>
{
    static  py::object toPython(const ObjectHolder& v)
    {
        return v.get();
    }


    static ObjectHolder fromPython(const py::object& o)
    {
        return ObjectHolder(o);
    }

    static bool isValid(const py::object&)
    {
        return true;
    }
};



