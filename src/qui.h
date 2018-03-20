#pragma once

#ifdef NPY_NDARRAYOBJECT_H
#error <numpy/ndarrayobject.h> must be included via qui.h
#endif

#ifndef QUI_MAIN
#define NO_IMPORT_ARRAY
#endif

#define PY_ARRAY_UNIQUE_SYMBOL qui_ARRAY_API


#include <pybind11/pybind11.h>
#include <numpy/ndarrayobject.h>
#include <datetime.h>

#include <QMetaType>

namespace py = pybind11;

Q_DECLARE_METATYPE(py::object)
