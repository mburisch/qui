#pragma once

#include "qui.h"

#include "BaseTypes.h"
#include "ScalarTypes.h"
#include "QtSimpleTypes.h"
#include "QtTypes.h"

#include "NDArray.h"
#include "Sequence.h"

#include <functional>

void initTypes(py::module& m);
void initQtTypes(py::module& m);
void initQMLTypes(py::module& m);
void initPaintTypes(py::module& m);



struct Types
{
// Macros to clarify meaning
#define HAS_CAST(x)     (x)
#define HAS_OBJECT(x)   (x)
#define HAS_VARIANT(x)  (x)

    // Define the types we use. This list needs to be evaluated during compile time (for py::type_caster specilizations)
    // and during run-time for registern QMetaType convertes and VariantHolder functions.
    // So use a macro which applies a "function" to the types.
#define APPLY_TO_TYPES(TYPE_FUNC) \
    TYPE_FUNC(bool,               "bool",      HAS_CAST(false), HAS_OBJECT(true), HAS_VARIANT(false)); \
    TYPE_FUNC(char,               "char",      HAS_CAST(false), HAS_OBJECT(true), HAS_VARIANT(false)); \
    TYPE_FUNC(signed char,        "schar",     HAS_CAST(false), HAS_OBJECT(true), HAS_VARIANT(false)); \
    TYPE_FUNC(unsigned char,      "uchar",     HAS_CAST(false), HAS_OBJECT(true), HAS_VARIANT(false)); \
    TYPE_FUNC(short,              "short",     HAS_CAST(false), HAS_OBJECT(true), HAS_VARIANT(false)); \
    TYPE_FUNC(unsigned short,     "ushort",    HAS_CAST(false), HAS_OBJECT(true), HAS_VARIANT(false)); \
    TYPE_FUNC(int,                "int",       HAS_CAST(false), HAS_OBJECT(true), HAS_VARIANT(false)); \
    TYPE_FUNC(unsigned int,       "uint",      HAS_CAST(false), HAS_OBJECT(true), HAS_VARIANT(false)); \
    TYPE_FUNC(long,               "long",      HAS_CAST(false), HAS_OBJECT(true), HAS_VARIANT(false)); \
    TYPE_FUNC(unsigned long,      "ulong",     HAS_CAST(false), HAS_OBJECT(true), HAS_VARIANT(false)); \
    TYPE_FUNC(long long,          "longlong",  HAS_CAST(false), HAS_OBJECT(true), HAS_VARIANT(false)); \
    TYPE_FUNC(unsigned long long, "ulonglong", HAS_CAST(false), HAS_OBJECT(true), HAS_VARIANT(false)); \
    TYPE_FUNC(float,              "float",     HAS_CAST(false), HAS_OBJECT(true), HAS_VARIANT(false)); \
    TYPE_FUNC(double,             "double",    HAS_CAST(false), HAS_OBJECT(true), HAS_VARIANT(false)); \
    \
    TYPE_FUNC(QString,           "QString",     HAS_CAST(true), HAS_OBJECT(true), HAS_VARIANT(true)); \
    TYPE_FUNC(QUrl,              "QUrl",        HAS_CAST(true), HAS_OBJECT(true), HAS_VARIANT(true)); \
    TYPE_FUNC(QByteArray,        "QByteArray",  HAS_CAST(true), HAS_OBJECT(true), HAS_VARIANT(true)); \
    TYPE_FUNC(QSize,             "QSize",       HAS_CAST(true), HAS_OBJECT(true), HAS_VARIANT(true)); \
    TYPE_FUNC(QSizeF,            "QSizeF",      HAS_CAST(true), HAS_OBJECT(true), HAS_VARIANT(true)); \
    TYPE_FUNC(QRect,             "QRect",       HAS_CAST(true), HAS_OBJECT(true), HAS_VARIANT(true)); \
    TYPE_FUNC(QRectF,            "QRectF",      HAS_CAST(true), HAS_OBJECT(true), HAS_VARIANT(true)); \
    TYPE_FUNC(QPoint,            "QPoint",      HAS_CAST(true), HAS_OBJECT(true), HAS_VARIANT(true)); \
    TYPE_FUNC(QPointF,           "QPointF",     HAS_CAST(true), HAS_OBJECT(true), HAS_VARIANT(true)); \
    TYPE_FUNC(QMatrix,           "QMatrix",     HAS_CAST(true), HAS_OBJECT(true), HAS_VARIANT(true)); \
    TYPE_FUNC(QMatrix4x4,        "QMatrix4x4",  HAS_CAST(true), HAS_OBJECT(true), HAS_VARIANT(true)); \
    TYPE_FUNC(QTransform,        "QTransform",  HAS_CAST(true), HAS_OBJECT(true), HAS_VARIANT(true)); \
    TYPE_FUNC(QQuaternion,       "QQutaernion", HAS_CAST(true), HAS_OBJECT(true), HAS_VARIANT(true)); \
    TYPE_FUNC(QVector2D,         "QVector2D",   HAS_CAST(true), HAS_OBJECT(true), HAS_VARIANT(true)); \
    TYPE_FUNC(QVector3D,         "QVector3D",   HAS_CAST(true), HAS_OBJECT(true), HAS_VARIANT(true)); \
    TYPE_FUNC(QVector4D,         "QVector4D",   HAS_CAST(true), HAS_OBJECT(true), HAS_VARIANT(true)); \
    TYPE_FUNC(QPolygon,          "QPolygon",    HAS_CAST(true), HAS_OBJECT(true), HAS_VARIANT(true)); \
    TYPE_FUNC(QPolygonF,         "QPolygonF",   HAS_CAST(true), HAS_OBJECT(true), HAS_VARIANT(true)); \
    TYPE_FUNC(QColor,            "QColor",      HAS_CAST(true), HAS_OBJECT(true), HAS_VARIANT(true)); \
    TYPE_FUNC(QDateTime,         "QDateTime",   HAS_CAST(true), HAS_OBJECT(true), HAS_VARIANT(true)); \
    TYPE_FUNC(QImage,            "QImage",      HAS_CAST(true), HAS_OBJECT(true), HAS_VARIANT(true));\
    \
    TYPE_FUNC(QVector<QPoint>,    "QPointVector",     HAS_CAST(true), HAS_OBJECT(true), HAS_VARIANT(true)); \
    TYPE_FUNC(QVector<QPointF>,   "QPointFVector",    HAS_CAST(true), HAS_OBJECT(true), HAS_VARIANT(true)); \
    TYPE_FUNC(QVector<QVector2D>, "QVector2DVector",  HAS_CAST(true), HAS_OBJECT(true), HAS_VARIANT(true)); \
    TYPE_FUNC(QVector<QVector3D>, "QVector3DVector",  HAS_CAST(true), HAS_OBJECT(true), HAS_VARIANT(true)); \
    TYPE_FUNC(QVector<QVector4D>, "QVector4DVector",  HAS_CAST(true), HAS_OBJECT(true), HAS_VARIANT(true)); \
    \
    TYPE_FUNC(QList<QString>, "QStringList_",  HAS_CAST(true), HAS_OBJECT(true), HAS_VARIANT(true)); \
    TYPE_FUNC(QStringList,    "QStringList",   HAS_CAST(true), HAS_OBJECT(true), HAS_VARIANT(true)); \
    \
    TYPE_FUNC(QJSValue, "QJSValue",      HAS_CAST(true), HAS_OBJECT(true), HAS_VARIANT(true));  \
    \
    TYPE_FUNC(QVariant, "QVariant",      HAS_CAST(true), HAS_OBJECT(true), HAS_VARIANT(false)); 
    

    // Define type info
    template<class T> struct Info;
#define MAKE_TYPE_INFO(T, name_, has_cast_, has_object_, has_variant_) template<> struct Info<T> { typedef T type; static constexpr const char* name = name_; static constexpr bool has_cast = has_cast_; }
    APPLY_TO_TYPES(MAKE_TYPE_INFO);
#undef MAKE_TYPE_INFO


    static void addConverter()
    {
#define REGISTER_CONVERTER(T, name_, has_cast_, has_object_, has_variant_) if (has_object_) QMetaType::registerConverter<ObjectHolder, T>(&ObjectHolder::convert<T>)
        APPLY_TO_TYPES(REGISTER_CONVERTER);
#undef REGISTER_CONVERTER
    }

    static void addHolder(py::module& m)
    {
#define DEF_HOLDER(T, name_, has_cast_, has_object_, has_variant_) if (has_variant_) m.def(name_, &VariantHolder::create<T>)
        APPLY_TO_TYPES(DEF_HOLDER);
#undef DEF_HOLDER
    }

    // Helper templates to determine whether a type T is defined for py::type_caster specilization
    template<class T, class U = void> struct has_cast : std::false_type { };
    template<class T> struct has_cast<T, std::enable_if_t<Info<T>::has_cast>> : std::true_type {};
    template<class T> static constexpr bool has_cast_v = has_cast<T>::value;

// Undefine type macros
#undef HAS_CAST
#undef HAS_OBJECT
#undef HAS_VARIANT

#undef APPLY_TO_TYPES
};



namespace pybind11 
{
    namespace detail 
    {
        template <class T>
        struct type_caster<T, std::enable_if_t<Types::has_cast_v<T>>>
        {
            PYBIND11_TYPE_CASTER(T, _(Types::Info<T>::name));

            bool load(py::handle src, bool implicit)
            {
                py::object o = py::reinterpret_borrow<py::object>(src);

                if (Type<T>::isValid(o) == false)
                    return false;

                value = Type<T>::fromPython(o);
                return true;
            }

            static py::handle cast(const T& src, py::return_value_policy policy, py::handle parent)
            {
                py::object o = Type<T>::toPython(src);
                return o.release();
            }
        };
    }
}

