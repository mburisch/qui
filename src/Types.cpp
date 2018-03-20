#pragma once

#include "Types.h"

void initTypes(py::module& m)
{
    //py::implicitly_convertible<VariantHolder, QVariant>();

    py::class_<VariantHolder>(m, "Variant")
        .def(py::init<>())
        .def(py::init<const VariantHolder&>())
        .def("get", &VariantHolder::get)
        .def("clear", &VariantHolder::clear)
        .def_property_readonly("type", [](const VariantHolder& v) { return v.typeName(); })
        .def_property_readonly("is_valid", &VariantHolder::isValid)
        .def("__str__", &VariantHolder::str)
        ;

    QMetaType::registerConverter<ObjectHolder, py::object>(&ObjectHolder::convert<py::object>);

    Types::addConverter();
    Types::addHolder(m);
}
