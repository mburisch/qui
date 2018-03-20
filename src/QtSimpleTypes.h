#pragma once

#include "qui.h"

#include <QString>
#include <QUrl>
#include <QSize>
#include <QSizeF>
#include <QRect>
#include <QRectF>
#include <QPoint>
#include <QPointF>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix>
#include <QMatrix4x4>
#include <QTransform>
#include <QQuaternion>
#include <QColor>
#include <QDateTime>
#include <QImage>
#include <QVector>
#include <QList>
#include <QStringList>

#include "BaseTypes.h"
#include "ScalarTypes.h"
#include "NDArray.h"
#include "Sequence.h"

//-----------------------------------------------------------------------------
// QObject / QMetaObject
//-----------------------------------------------------------------------------


template<>
struct Type<QObject*>
{
    static py::object toPython(QObject* object)
    {
        return py::cast(object);
    }

    static QObject* fromPython(const py::object& o)
    {
        return py::cast<QObject*>(o);
    }

    static bool isValid(const py::object& o)
    {
        return py::isinstance<QObject*>(o);
    }
};


template<>
struct Type<QMetaObject*>
{
    static py::object toPython(const QMetaObject* object)
    {
        return py::cast(object);
    }

    static const QMetaObject* fromPython(const py::object& o)
    {
        return py::cast<QMetaObject*>(o);
    }

    static bool isValid(const py::object& o)
    {
        return py::isinstance<QMetaObject*>(o);
    }
};

template<> struct Type<const QMetaObject*> : public Type<QMetaObject*> {};


//-----------------------------------------------------------------------------
// QString / QUrl
//-----------------------------------------------------------------------------

template<>
struct Type<QString>
{
    static py::object toPython(const QString& s)
    {
        QByteArray utf8 = s.toUtf8();
        return py::reinterpret_steal<py::object>(PyUnicode_DecodeUTF8(utf8.data(), utf8.size(), nullptr));
    }

    static QString fromPython(const py::object& o)
    {
        Py_ssize_t size = 0;
        char* utf8 = PyUnicode_AsUTF8AndSize(o.ptr(), &size);
        return QString::fromUtf8(utf8, size);
    }

    static bool isValid(const py::object& o)
    {
        return PyUnicode_Check(o.ptr()) != 0;
    }
};



template<>
struct Type<QUrl>
{
    static py::object toPython(const QUrl& r)
    {
        return Type<QString>::toPython(r.url());
    }

    static QUrl fromPython(const py::object& o)
    {
        return QUrl(Type<QString>::fromPython(o));
    }

    static bool isValid(const py::object& o)
    {
        return Type<QString>::isValid(o);
    }
};




//-----------------------------------------------------------------------------
// QByteArray
//-----------------------------------------------------------------------------

template<>
struct Type<QByteArray>
{
    static py::object toPython(const QByteArray& s)
    {
        return py::reinterpret_steal<py::object>(PyBytes_FromStringAndSize(s.data(), s.size()));
    }

    static QByteArray fromPython(const py::object& o)
    {
        Py_buffer view;
        if (PyObject_GetBuffer(o.ptr(), &view, PyBUF_CONTIG_RO) == -1)
            throw std::runtime_error("Object is not a valid buffer");

        QByteArray data = QByteArray((const char*)view.buf, view.len);

        /*
        //if (PyObject_GetBuffer(o.ptr(), &view, PyBUF_STRIDED_RO) == -1)
        QByteArray data;
        if (PyBuffer_IsContiguous(&view, 'C') == 1)
        {
            data = QByteArray((const char*)view.buf, view.len);
        }
        else
        {
            data.resize(view.len);

        }
        */

        PyBuffer_Release(&view);

        return data;
    }

    static bool isValid(const py::object& o)
    {
        return PyObject_CheckBuffer(o.ptr()) != 0;
    }
};


//-----------------------------------------------------------------------------
// QSize / QSizeF
//-----------------------------------------------------------------------------

template<>
struct Type<QSize>
{
    static py::object toPython(const QSize& s)
    {
        return NDArray::fromValues<int>({ s.width(), s.height() });
    }

    static QSize fromPython(const py::object& o)
    {
        auto v = extractValues<int, 2>(o);
        return QSize(v[0], v[1]);
    }

    static bool isValid(const py::object& o)
    {
        return Sequence::isValid(o, 2);
    }
};


template<>
struct Type<QSizeF>
{
    static py::object toPython(const QSizeF& s)
    {
        return NDArray::fromValues<double>({ s.width(), s.height() });
    }

    static QSizeF fromPython(const py::object& o)
    {
        auto v = extractValues<double, 2>(o);
        return QSizeF(v[0], v[1]);
    }

    static bool isValid(const py::object& o)
    {
        return Sequence::isValid(o, 2);
    }
};

//-----------------------------------------------------------------------------
// QRect / QRectF
//-----------------------------------------------------------------------------

template<>
struct Type<QRect>
{
    static py::object toPython(const QRect& r)
    {
        return NDArray::fromValues<int>({ r.x(), r.y(), r.width(), r.height() });
    }

    static QRect fromPython(const py::object& o)
    {
        auto v = extractValues<int, 4>(o);
        return QRect(v[0], v[1], v[2], v[3]);
    }

    static bool isValid(const py::object& o)
    {
        return Sequence::isValid(o, 4);
    }
};

template<>
struct Type<QRectF>
{
    static py::object toPython(const QRectF& r)
    {
        return NDArray::fromValues<double>({ r.x(), r.y(), r.width(), r.height() });
    }

    static QRectF fromPython(const py::object& o)
    {
        auto v = extractValues<double, 4>(o);
        return QRectF(v[0], v[1], v[2], v[3]);
    }

    static bool isValid(const py::object& o)
    {
        return Sequence::isValid(o, 4);
    }
};


//-----------------------------------------------------------------------------
// QPoint / QPointF
//-----------------------------------------------------------------------------

template<>
struct Type<QPoint>
{
    static py::object toPython(const QPoint& p)
    {
        return NDArray::fromValues<int>({ p.x(), p.y() });
    }

    static QPoint fromPython(const py::object& o)
    {
        auto v = extractValues<int, 2>(o);
        return QPoint(v[0], v[1]);
    }

    static bool isValid(const py::object& o)
    {
        return Sequence::isValid(o, 2);
    }

    static QPoint fromValues(const int* values)
    {
        return QPoint(values[0], values[1]);
    }

    static void toValues(const QPoint& p, int* dst)
    {
        dst[0] = p.x();
        dst[1] = p.y();
    }
};


template<>
struct Type<QPointF>
{
    static py::object toPython(const QPointF& p)
    {
        return NDArray::fromValues<double>({ p.x(), p.y() });
    }

    static QPointF fromPython(const py::object& o)
    {
        auto v = extractValues<double, 2>(o);
        return QPointF(v[0], v[1]);
    }

    static bool isValid(const py::object& o)
    {
        return Sequence::isValid(o, 2);
    }


    static QPointF fromValues(const double* values)
    {
        return QPointF(values[0], values[1]);
    }

    static void toValues(const QPointF& p, double* dst)
    {
        dst[0] = p.x();
        dst[1] = p.y();
    }
};


//-----------------------------------------------------------------------------
// QVector2D / QVector3D / QVector4D
//-----------------------------------------------------------------------------

template<>
struct Type<QVector2D>
{
    static py::object toPython(const QVector2D& p)
    {
        return NDArray::fromValues<float>({ p.x(), p.y() });
    }

    static QVector2D fromPython(const py::object& o)
    {
        auto v = extractValues<float, 2>(o);
        return QVector2D(v[0], v[1]);
    }

    static bool isValid(const py::object& o)
    {
        return Sequence::isValid(o, 2);
    }

    static QVector2D fromValues(const float* values)
    {
        return QVector2D(values[0], values[1]);
    }

    static void toValues(const QVector2D& p, float* dst)
    {
        dst[0] = p.x();
        dst[1] = p.y();
    }
};

template<>
struct Type<QVector3D>
{
    static py::object toPython(const QVector3D& p)
    {
        return NDArray::fromValues<float>({ p.x(), p.y(), p.z() });
    }

    static QVector3D fromPython(const py::object& o)
    {
        auto v = extractValues<float, 3>(o);
        return QVector3D(v[0], v[1], v[2]);
    }

    static bool isValid(const py::object& o)
    {
        return Sequence::isValid(o, 3);
    }

    static QVector3D fromValues(const float* values)
    {
        return QVector3D(values[0], values[1], values[2]);
    }

    static void toValues(const QVector3D& p, float* dst)
    {
        dst[0] = p.x();
        dst[1] = p.y();
        dst[2] = p.z();
    }
};


template<>
struct Type<QVector4D>
{
    static py::object toPython(const QVector4D& p)
    {
        return NDArray::fromValues<float>({ p.x(), p.y(), p.z(), p.w() });
    }

    static QVector4D fromPython(const py::object& o)
    {
        auto v = extractValues<float, 4>(o);
        return QVector4D(v[0], v[1], v[2], v[3]);
    }

    static bool isValid(const py::object& o)
    {
        return Sequence::isValid(o, 4);
    }

    static QVector4D fromValues(const float* values)
    {
        return QVector4D(values[0], values[1], values[2], values[4]);
    }

    static void toValues(const QVector4D& p, float* dst)
    {
        dst[0] = p.x();
        dst[1] = p.y();
        dst[2] = p.z();
        dst[3] = p.w();
    }
};

//-----------------------------------------------------------------------------
// QMatrix / QMatrix4x4
//-----------------------------------------------------------------------------

template<>
struct Type<QMatrix>
{
    static py::object toPython(const QMatrix& m)
    {
        return NDArray::fromValues<double>({ 2, 3 }, { m.m11(), m.m12(), m.dx(), m.m21(), m.m22(), m.dy() });
    }

    static QMatrix fromPython(const py::object& o)
    {
        py::object arr = NDArray::ensureArray<double>(o, 2);
        const double* v = NDArray::data<double>(arr);
        return QMatrix(v[0], v[1], v[3], v[4], v[2], v[5]);
    }

    static bool isValid(const py::object& o)
    {
        return NDArray::isCompatible(o, { 2, 3 });
    }
};

template<>
struct Type<QMatrix4x4>
{
    static py::object toPython(const QMatrix4x4& m)
    {
        return NDArray::fromData<float>({ 4, 4 }, m.data());
    }

    static QMatrix4x4 fromPython(const py::object& o)
    {
        py::object arr = NDArray::ensureArray<float>(o, 2);
        const float* v = NDArray::data<float>(arr);
        return QMatrix4x4(v);
    }

    static bool isValid(const py::object& o)
    {
        return NDArray::isCompatible(o, { 4, 4 });
    }
};


//-----------------------------------------------------------------------------
// QTransform / QQuaternion
//-----------------------------------------------------------------------------

template<>
struct Type<QTransform>
{
    static py::object toPython(const QTransform& m)
    {
        return NDArray::fromValues<double>({ 3, 3 }, { m.m11(), m.m12(), m.m13(), m.m21(), m.m22(), m.m23(), m.m31(), m.m32(), m.m33() });
    }

    static QTransform fromPython(const py::object& o)
    {
        py::object arr = NDArray::ensureArray<double>(o, 2);
        const double* v = NDArray::data<double>(arr);
        return QTransform(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8]);
    }

    static bool isValid(const py::object& o)
    {
        return NDArray::isCompatible(o, { 3, 3 });
    }
};


template<>
struct Type<QQuaternion>
{
    static py::object toPython(const QQuaternion& p)
    {
        return NDArray::fromValues<float>({ p.scalar(), p.x(), p.y(), p.z() });
    }

    static QQuaternion fromPython(const py::object& o)
    {
        auto v = extractValues<float, 4>(o);
        return QQuaternion(v[0], v[1], v[2], v[3]);
    }

    static bool isValid(const py::object& o)
    {
        return Sequence::isValid(o, 4);
    }
};



//-----------------------------------------------------------------------------
// QVector<T>
//-----------------------------------------------------------------------------

template<class T, class ElementType, std::size_t N>
struct VectorType
{
    typedef Type<T> type;
    typedef ElementType element_type;
    static const std::size_t value_size = N;

    static py::object toPython(const QVector<T>& data)
    {
        py::object arr = NDArray::create<element_type>({ data.size(), static_cast<npy_int>(value_size) });

        for (int i = 0; i < data.size(); i++)
        {
            element_type* dst = NDArray::data<element_type>(arr, i);
            type::toValues(data[i], dst);
        }

        return arr;
    }

    static QVector<T> fromPython(const py::object& o)
    {
        py::object arr = NDArray::ensureArray<element_type>(o, 2);

        int count = NDArray::dim(arr, 0);
        QVector<T> data;
        data.reserve(count);

        for (int i = 0; i < count; i++)
        {
            const element_type* src = NDArray::data<element_type>(arr, i);
            data.append(type::fromValues(src));
        }

        return data;
    }

    static bool isValid(const py::object& o)
    {
        std::vector<npy_intp> dims;
        NPY_TYPES type;
        if (NDArray::getParams(o, dims, type) == false)
            return false;

        if (NumPy::isValueType(type) == false)
            return false;

        if (NDArray::checkDimensions(static_cast<int>(dims.size()), dims.data(), { -1, static_cast<npy_int>(value_size) }))
            return true;

        return false;
    }
};



template<> struct Type<QVector<QPoint>>    : public VectorType<QPoint, int, 2> {};
template<> struct Type<QVector<QPointF>>   : public VectorType<QPointF, double, 2> {};
template<> struct Type<QVector<QVector2D>> : public VectorType<QVector2D, float, 2> {};
template<> struct Type<QVector<QVector3D>> : public VectorType<QVector3D, float, 3> {};
template<> struct Type<QVector<QVector4D>> : public VectorType<QVector4D, float, 4> {};



//-----------------------------------------------------------------------------
// QPolygon / QPolygonF
//-----------------------------------------------------------------------------

template<>
struct Type<QPolygon>
{
    static py::object toPython(const QPolygon& p)
    {
        return Type<QVector<QPoint>>::toPython(p);
    }

    static QPolygon fromPython(const py::object& o)
    {
        return Type<QVector<QPoint>>::fromPython(o);
    }

    static bool isValid(const py::object& o)
    {
        return Type<QVector<QPoint>>::isValid(o);
    }
};

template<>
struct Type<QPolygonF>
{
    static py::object toPython(const QPolygonF& p)
    {
        return Type<QVector<QPointF>>::toPython(p);
    }

    static QPolygonF fromPython(const py::object& o)
    {
        return Type<QVector<QPointF>>::fromPython(o);
    }

    static bool isValid(const py::object& obj)
    {
        return Type<QVector<QPointF>>::isValid(obj);
    }
};


//-----------------------------------------------------------------------------
// QColor
//-----------------------------------------------------------------------------

template<>
struct Type<QColor>
{
    static py::object toPython(const QColor& c)
    {
        return NDArray::fromValues<double>({ c.redF(), c.greenF(), c.blueF(), c.alphaF() });
    }

    static QColor fromPython(const py::object& o)
    {
        if (Type<QString>::isValid(o))
        {
            // Check QString first as a string is also a sequence
            QString name = Type<QString>::fromPython(o);
            if (QColor::isValidColor(name) == false)
                throw std::invalid_argument("Invalid color name: " + name.toStdString());

            return QColor(name);

        }
        else if (Sequence::isValid(o, 3))
        {
            auto v = extractValues<double, 3>(o);
            return QColor::fromRgbF(v[0], v[1], v[2]);
        }
        else if (Sequence::isValid(o, 4))
        {
            auto v = extractValues<double, 4>(o);
            return QColor::fromRgbF(v[0], v[1], v[2], v[3]);
        }
        else if (Long::isValid(o))
        {
            unsigned int v = Long::fromPython<unsigned int>(o);
            return QColor(v);

        }
        
        throw std::invalid_argument("Cannot convert PyObject to QColor.");
    }

    static bool isValid(const py::object& o)
    {
        if (Sequence::isValid(o, 3) || Sequence::isValid(o, 4))
            return true;

        if (Long::isValid(o))
            return true;

        if (Type<QString>::isValid(o))
            return true;

        return false;
    }
};

//-----------------------------------------------------------------------------
// QDateTime
//-----------------------------------------------------------------------------

template<>
struct Type<QDateTime>
{
    static void ensureAPI()
    {
        // PyDateTimeAPI is defined as static, so make sure every translation unit has it defined
        // if it needs it
        if (PyDateTimeAPI == nullptr)
            PyDateTime_IMPORT;
    }

    static py::object toPython(const QDateTime& dt)
    {
        ensureAPI();
        QDate d = dt.date();
        QTime t = dt.time();
        py::object r = py::reinterpret_steal<py::object>(PyDateTime_FromDateAndTime(d.year(), d.month(), d.day(), t.hour(), t.minute(), t.second(), t.msec() * 1000));
        return r;
    }

    static QDateTime fromPython(const py::object& o)
    {
        ensureAPI();
        PyObject* obj = o.ptr();
        return QDateTime(QDate(PyDateTime_GET_YEAR(obj), PyDateTime_GET_MONTH(obj), PyDateTime_GET_DAY(obj)), 
                         QTime(PyDateTime_DATE_GET_HOUR(obj), PyDateTime_DATE_GET_MINUTE(obj), PyDateTime_DATE_GET_SECOND(obj), PyDateTime_DATE_GET_MICROSECOND(obj) / 1000));
    }

    static bool isValid(const py::object& o)
    {
        ensureAPI();
        return PyDateTime_Check(o.ptr()) != 0;
    }
};


//-----------------------------------------------------------------------------
// QImage
//-----------------------------------------------------------------------------

template<>
struct Type<QImage>
{
    static py::object toPython(const QImage& im)
    {
        int channels = 0;
        switch (im.format())
        {
            case QImage::Format_Invalid:    return py::none();
            case QImage::Format_RGB32:      channels = 4; break;
            case QImage::Format_ARGB32:     channels = 4; break;
            case QImage::Format_RGB888:     channels = 3; break;
            case QImage::Format_RGBX8888:   channels = 4; break;
            case QImage::Format_RGBA8888:   channels = 4; break;
            case QImage::Format_Grayscale8: channels = 1; break;
            default: 
                throw std::invalid_argument("Unsupported image format: " + std::to_string(im.format()));
        }

        py::object arr = channels == 1 ? 
                            NDArray::create<unsigned char>({ im.height(), im.width() }) :
                            NDArray::create<unsigned char>({ im.height(), im.width(), channels });


        for (int y = 0; y < im.height(); y++)
            memcpy(NDArray::data<unsigned char>(arr, y), im.scanLine(y), im.bytesPerLine());

        return arr;
    }


    static QImage fromPython(const py::object& o)
    {
        py::object arr = NDArray::ensureArray<unsigned char>(o, 2, 3);
        
        QImage im;
        if ((NDArray::ndim(arr) == 2) || ((NDArray::ndim(arr) == 3) && (NDArray::dim(arr, 2) == 1)))
            im = QImage(NDArray::dim(arr, 1), NDArray::dim(arr, 0), QImage::Format_Grayscale8);
        else if ((NDArray::ndim(arr) == 3) && (NDArray::dim(arr, 2) == 3))
            im = QImage(NDArray::dim(arr, 1), NDArray::dim(arr, 0), QImage::Format_RGB888);
        else if ((NDArray::ndim(arr) == 3) && (NDArray::dim(arr, 2) == 4))
            im = QImage(NDArray::dim(arr, 1), NDArray::dim(arr, 0), QImage::Format_ARGB32);
        else
            throw std::invalid_argument("Unsupported image format");

        for (int y = 0; y < im.height(); y++)
            memcpy(im.scanLine(y), NDArray::data<unsigned char>(arr, y), im.bytesPerLine());
        
        return im;
    }


    static bool isValid(const py::object& o)
    {
        std::vector<npy_intp> dims;
        NPY_TYPES type;
        if (NDArray::getParams(o, dims, type) == false)
            return false;

        if (NumPy::isValueType(type) == false)
            return false;

        if (NDArray::checkDimensions(static_cast<int>(dims.size()), dims.data(), { -1, -1 }))
            return true;

        if (NDArray::checkDimensions(static_cast<int>(dims.size()), dims.data(), { -1, -1, 1 }))
            return true;

        if (NDArray::checkDimensions(static_cast<int>(dims.size()), dims.data(), { -1, -1, 3 }))
            return true;

        if (NDArray::checkDimensions(static_cast<int>(dims.size()), dims.data(), { -1, -1, 4 }))
            return true;

        return false;
    }
};




//-----------------------------------------------------------------------------
// QList<T>
//-----------------------------------------------------------------------------

template<class T>
struct ListType
{
    static py::object toPython(const QList<T>& data)
    {
        py::list l;
        for (const auto& v : data)
            l.append(Type<T>::toPython(v));

        return l;
    }

    static QList<T> fromPython(const py::object& o)
    {
        QList<T> data;
        int count = Sequence::size(o);
        for (int i = 0; i < count; i++)
            data.append(Sequence::value<T>(o, i));

        return data;
    }

    static bool isValid(const py::object& o)
    {
        return Sequence::isValid(o);
    }
};


template<> struct Type<QList<QString>> : public ListType<QString> {};


//-----------------------------------------------------------------------------
// QStringList
//-----------------------------------------------------------------------------

template<>
struct Type<QStringList>
{
    static py::object toPython(const QStringList& data)
    {
        return Type<QList<QString>>::toPython(data);
    }

    static QStringList fromPython(const py::object& o)
    {
        return Type<QList<QString>>::fromPython(o);
    }

    static bool isValid(const py::object& o)
    {
        return Type<QList<QString>>::isValid(o);
    }
};


