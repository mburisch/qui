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
    

#define APPLY_NUMBER_TYPES(TYPE_FUNC) \
    TYPE_FUNC(bool,               "bool"); \
    TYPE_FUNC(char,               "char"); \
    TYPE_FUNC(signed char,        "schar"); \
    TYPE_FUNC(unsigned char,      "uchar"); \
    TYPE_FUNC(short,              "short"); \
    TYPE_FUNC(unsigned short,     "ushort"); \
    TYPE_FUNC(int,                "int"); \
    TYPE_FUNC(unsigned int,       "uint"); \
    TYPE_FUNC(long,               "long"); \
    TYPE_FUNC(unsigned long,      "ulong"); \
    TYPE_FUNC(long long,          "longlong"); \
    TYPE_FUNC(unsigned long long, "ulonglong"); \
    TYPE_FUNC(float,              "float"); \
    TYPE_FUNC(double,             "double");

#define APPLY_TYPES(TYPE_FUNC) \
    TYPE_FUNC(QString,           "QString"); \
    TYPE_FUNC(QUrl,              "QUrl"); \
    TYPE_FUNC(QByteArray,        "QByteArray"); \
    TYPE_FUNC(QSize,             "QSize"); \
    TYPE_FUNC(QSizeF,            "QSizeF"); \
    TYPE_FUNC(QRect,             "QRect"); \
    TYPE_FUNC(QRectF,            "QRectF"); \
    TYPE_FUNC(QPoint,            "QPoint"); \
    TYPE_FUNC(QPointF,           "QPointF"); \
    TYPE_FUNC(QMatrix,           "QMatrix"); \
    TYPE_FUNC(QMatrix4x4,        "QMatrix4x4"); \
    TYPE_FUNC(QTransform,        "QTransform"); \
    TYPE_FUNC(QQuaternion,       "QQutaernion"); \
    TYPE_FUNC(QVector2D,         "QVector2D"); \
    TYPE_FUNC(QVector3D,         "QVector3D"); \
    TYPE_FUNC(QVector4D,         "QVector4D"); \
    TYPE_FUNC(QPolygon,          "QPolygon"); \
    TYPE_FUNC(QPolygonF,         "QPolygonF"); \
    TYPE_FUNC(QColor,            "QColor"); \
    TYPE_FUNC(QDateTime,         "QDateTime"); \
    TYPE_FUNC(QImage,            "QImage");\
    \
    TYPE_FUNC(QVector<QPoint>,    "QPointVector"); \
    TYPE_FUNC(QVector<QPointF>,   "QPointFVector"); \
    TYPE_FUNC(QVector<QVector2D>, "QVector2DVector"); \
    TYPE_FUNC(QVector<QVector3D>, "QVector3DVector"); \
    TYPE_FUNC(QVector<QVector4D>, "QVector4DVector"); \
    \
    TYPE_FUNC(QList<QString>, "QStringList_"); \
    TYPE_FUNC(QStringList,    "QStringList");

#define APPLY_COMPLEX_TYPES(TYPE_FUNC) \
    TYPE_FUNC(QVariant, "QVariant"); \
    TYPE_FUNC(QJSValue, "QJSValue");


    // Define type info
    template<class T> struct Info;
#define TYPE_FUNC(T, name_) template<> struct Info<T> { typedef T type; static constexpr const char* name = name_; }
    APPLY_TYPES(TYPE_FUNC);
    APPLY_COMPLEX_TYPES(TYPE_FUNC);
#undef TYPE_FUNC


    static void addConverter()
    {
#define TYPE_FUNC(T, name_) QMetaType::registerConverter<ObjectHolder, T>(&ObjectHolder::convert<T>);
        APPLY_NUMBER_TYPES(TYPE_FUNC);
        APPLY_TYPES(TYPE_FUNC);
        APPLY_COMPLEX_TYPES(TYPE_FUNC);
#undef TYPE_FUNC
    }

    static void addHolder(py::module& m)
    {
#define TYPE_FUNC(T, name_) m.def(name_, &VariantHolder::create<T>);
        APPLY_TYPES(TYPE_FUNC);
#undef TYPE_FUNC
    }

    // Helper templates to determine whether a type T is defined
    template<class T, class U = void> struct is_valid : std::false_type { };
    template<class T> struct is_valid<T, std::enable_if_t<std::is_same_v<T, typename Info<T>::type>>> : std::true_type {};
    template<class T> static constexpr bool is_valid_v = is_valid<T>::value;
};


namespace pybind11 
{
    namespace detail 
    {
        template <class T>
        struct type_caster<T, std::enable_if_t<Types::is_valid_v<T>>>
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

