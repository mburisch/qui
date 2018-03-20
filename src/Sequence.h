#pragma once

#include "qui.h"

template<class T> struct Converter;

class Sequence
{
public:
    static bool isValid(const py::object& o)
    {
        return PySequence_Check(o.ptr()) == 1;
    }


    static bool isValid(const py::object& o, int length)
    {
        if (isValid(o) == false)
            return false;

        if (size(o) != length)
            return false;

        return true;
    }


    static int size(const py::object& o)
    {
        return PySequence_Size(o.ptr());
    }


    template<class T> static T value(const py::object& o, Py_ssize_t index)
    {
        py::object v = py::reinterpret_steal<py::object>(PySequence_ITEM(o.ptr(), index));
        return Type<T>::fromPython(v);
    }


    template<class T, size_t N> static std::array<T, N> values(const py::object& o)
    {
        std::array<T, N> arr;
        for (int i = 0; i < N; i++)
            arr[i] = value<T>(o, i);

        return arr;
    }

};


template<class T, size_t N> std::array<T, N> extractValues(const py::object& o)
{
    if (NDArray::isValueArray(o, { N }))
        return NDArray::values<T, N>(o);
    else if (Sequence::isValid(o, N))
        return Sequence::values<T, N>(o);

    throw std::invalid_argument("Cannot extract values from object.");
}

