#pragma once


#include "qui.h"

struct NumPy
{
    template<class T> static constexpr NPY_TYPES type();
    template<> static constexpr NPY_TYPES type<bool>()                  { return NPY_BOOL; }
    template<> static constexpr NPY_TYPES type<char>()                  { return NPY_BYTE; } // char -> byte
    template<> static constexpr NPY_TYPES type<signed char>()           { return NPY_BYTE; }
    template<> static constexpr NPY_TYPES type<unsigned char>()         { return NPY_UBYTE; }
    template<> static constexpr NPY_TYPES type<short>()                 { return NPY_SHORT; }
    template<> static constexpr NPY_TYPES type<unsigned short>()        { return NPY_USHORT; }
    template<> static constexpr NPY_TYPES type<int>()                   { return NPY_INT; }
    template<> static constexpr NPY_TYPES type<unsigned int>()          { return NPY_UINT; }
    template<> static constexpr NPY_TYPES type<long>()                  { return NPY_LONG; }
    template<> static constexpr NPY_TYPES type<unsigned long>()         { return NPY_ULONG; }
    template<> static constexpr NPY_TYPES type<long long>()             { return NPY_LONGLONG; }
    template<> static constexpr NPY_TYPES type<unsigned long long>()    { return NPY_ULONGLONG; }
    template<> static constexpr NPY_TYPES type<float>()                 { return NPY_FLOAT; }
    template<> static constexpr NPY_TYPES type<double>()                { return NPY_DOUBLE; }
    template<> static constexpr NPY_TYPES type<long double>()           { return NPY_LONGDOUBLE; }

    static bool isValueType(NPY_TYPES type)
    {
        switch (type)
        {
            case NPY_BOOL:          return true;
            //case NPY_CHAR:          return true;
            case NPY_BYTE:          return true;
            case NPY_UBYTE:         return true;
            case NPY_SHORT:         return true;
            case NPY_USHORT:        return true;
            case NPY_INT:           return true;
            case NPY_UINT:          return true;
            case NPY_LONG:          return true;
            case NPY_ULONG:         return true;
            case NPY_LONGLONG:      return true;
            case NPY_ULONGLONG:     return true;
            case NPY_FLOAT:         return true;
            case NPY_DOUBLE:        return true;
            case NPY_LONGDOUBLE:    return true;
            default:                return false;
        }
    }

    template<class Dst, class Src> static Dst cast(const void* ptr)
    {
        return static_cast<Dst>(*reinterpret_cast<std::add_const_t<Src>*>(ptr));
    }

    template<class Src, class Dst> static void copy(const void* src, size_t srcStride, void* dst, size_t dstStride, size_t count)
    {
        typedef std::remove_cv_t<std::decay_t<Src>> Src_t;
        typedef std::remove_cv_t<std::decay_t<Dst>> Dst_t;

        const char* pSrc = reinterpret_cast<const char*>(src);
        char* pDst = reinterpret_cast<char*>(dst);
                
        for (size_t i = 0; i < count; ++i)
        {
            const Src_t* s = reinterpret_cast<const Src_t*>(pSrc);
            Dst_t* d = reinterpret_cast<Dst_t*>(pDst);
            
            *d = static_cast<Dst_t>(*s);

            pSrc += srcStride;
            pDst += dstStride;
        }
    }


    template<class T> static T cast(NPY_TYPES type, const void* ptr)
    {
        switch (type)
        {
            case NPY_BOOL:          return cast<T, std::uint8_t>(ptr);
            //case NPY_CHAR:          return cast<T, char>(ptr);
            case NPY_BYTE:          return cast<T, signed char>(ptr);
            case NPY_UBYTE:         return cast<T, unsigned char>(ptr);
            case NPY_SHORT:         return cast<T, short>(ptr);
            case NPY_USHORT:        return cast<T, unsigned short>(ptr);
            case NPY_INT:           return cast<T, int>(ptr);
            case NPY_UINT:          return cast<T, unsigned int>(ptr);
            case NPY_LONG:          return cast<T, long>(ptr);
            case NPY_ULONG:         return cast<T, unsigned long>(ptr);
            case NPY_LONGLONG:      return cast<T, long long>(ptr);
            case NPY_ULONGLONG:     return cast<T, unsigned long long>(ptr);
            case NPY_FLOAT:         return cast<T, float>(ptr);
            case NPY_DOUBLE:        return cast<T, double>(ptr);
            case NPY_LONGDOUBLE:    return cast<T, long double>(ptr);
            default:                throwError(PyExc_ValueError, "Cannot convert to type.")
        }
    }


    template<class T, class U, size_t N> static std::array<T, N> cast(const void* ptr, size_t stride)
    {
        std::array<T, N> result;
        copy<U, T>(ptr, stride, result.data(), sizeof(T), N);
        return result;
    }


    template<class T, size_t N> static std::array<T, N> cast(NPY_TYPES type, void* ptr, int stride)
    {
        switch (type)
        {
            case NPY_BOOL:          return cast<T, std::uint8_t, N>(ptr, stride);
            //case NPY_CHAR:          return cast<T, char, N>(ptr, stride);
            case NPY_BYTE:          return cast<T, signed char, N>(ptr, stride);
            case NPY_UBYTE:         return cast<T, unsigned char, N>(ptr, stride);
            case NPY_SHORT:         return cast<T, short, N>(ptr, stride);
            case NPY_USHORT:        return cast<T, unsigned short, N>(ptr, stride);
            case NPY_INT:           return cast<T, int, N>(ptr, stride);
            case NPY_UINT:          return cast<T, unsigned int, N>(ptr, stride);
            case NPY_LONG:          return cast<T, long, N>(ptr, stride);
            case NPY_ULONG:         return cast<T, unsigned long, N>(ptr, stride);
            case NPY_LONGLONG:      return cast<T, long long, N>(ptr, stride);
            case NPY_ULONGLONG:     return cast<T, unsigned long long, N>(ptr, stride);
            case NPY_FLOAT:         return cast<T, float, N>(ptr, stride);
            case NPY_DOUBLE:        return cast<T, double, N>(ptr, stride);
            case NPY_LONGDOUBLE:    return cast<T, long double, N>(ptr, stride);
            default:                
                throw std::invalid_argument("Cannot convert to type.");
        }
    }


};


class NDArray
{
public:
    // Is array
    static bool isArray(const py::object& o)
    {
        return PyArray_Check(o.ptr()) != 0;
    }


    // Is array of values
    static bool isValueArray(const py::object& o)
    {
        if (isArray(o) == false)
            return false;

        if (NumPy::isValueType(dataType(o)) == false)
            return false;

        return true;
    }


    static bool isValueArray(const py::object& o, const std::initializer_list<npy_int>& dimensions)
    {
        if (isValueArray(o) == false)

        if (checkDimensions(o, dimensions) == false)
            return false;

        return true;
    }


    static bool checkDimensions(const py::object& o, const std::initializer_list<npy_int>& dimensions)
    {
        PyArrayObject* arr = reinterpret_cast<PyArrayObject*>(o.ptr());
        return checkDimensions(PyArray_NDIM(arr), PyArray_DIMS(arr), dimensions);
    }


    // Check dimensions (skip negative values)
    static bool checkDimensions(int ndim, const npy_intp* dims, const std::initializer_list<npy_int>& dimensions)
    {
        if (ndim != dimensions.size())
            return false;

        for (auto s : dimensions)
        {
            if (s != -1)
            {
                if (*dims != s)
                    return false;
            }
            ++dims;
        }

        return true;
    }


    // Is object compatible (i.e. can be converted) to an array with given dimensions (skip negative values).
    static bool isCompatible(const py::object& o, const std::initializer_list<npy_int>& dimensions)
    {
        std::vector<npy_intp> dims;
        NPY_TYPES type;
        if (getParams(o, dims, type) == false)
            return false;

        if (NumPy::isValueType(type) == false)
            return false;

        if (checkDimensions(static_cast<int>(dims.size()), dims.data(), dimensions) == false)
            return false;

        return true;
    }
    

    // Get dimension and type of "array"-like object
    static bool getParams(const py::object& o, std::vector<npy_intp>& dimensions, NPY_TYPES& type)
    {
        if (isArray(o))
        {
            PyArrayObject* arr = reinterpret_cast<PyArrayObject*>(o.ptr());
            
            int ndim = PyArray_NDIM(arr);
            const npy_intp* dims = PyArray_DIMS(arr);
            dimensions.reserve(ndim);
            for (int i = 0; i < ndim; i++)
                dimensions.push_back(dims[i]);

            type = dataType(o);

            return true;
        }
        else
        {
            PyArray_Descr* out_descr = nullptr;
            PyArrayObject* arr = nullptr;
            int ndim = 0;
            npy_intp dims[NPY_MAXDIMS];

            bool ok = false;
            if (PyArray_GetArrayParamsFromObject(o.ptr(), nullptr, false, &out_descr, &ndim, dims, &arr, nullptr) == 0)
            {
                const npy_intp* pdims = nullptr;
                if (arr != nullptr)
                {
                    type = static_cast<NPY_TYPES>(PyArray_TYPE(arr));
                    ndim = PyArray_NDIM(arr);
                    pdims = PyArray_DIMS(arr);
                }
                else
                {
                    type = static_cast<NPY_TYPES>(out_descr->type_num);
                    pdims = dims;
                }

                dimensions.reserve(ndim);
                for (int i = 0; i < ndim; i++)
                    dimensions.push_back(pdims[i]);
                
                ok = true;
            }

            Py_XDECREF(out_descr);
            Py_XDECREF(arr);

            return ok;
        }
    }

    static int ndim(const py::object& o)
    {
        PyArrayObject* arr = reinterpret_cast<PyArrayObject*>(o.ptr());
        return PyArray_NDIM(arr);
    }

    static const npy_intp* dims(const py::object& o)
    {
        PyArrayObject* arr = reinterpret_cast<PyArrayObject*>(o.ptr());
        return PyArray_DIMS(arr);
    }

    static const npy_intp* shape(const py::object& o)
    {
        PyArrayObject* arr = reinterpret_cast<PyArrayObject*>(o.ptr());
        return PyArray_SHAPE(arr);
    }

    static int dim(const py::object& o, int index)
    {
        PyArrayObject* arr = reinterpret_cast<PyArrayObject*>(o.ptr());
        return PyArray_DIM(arr, index);
    }

    static const npy_intp* strides(const py::object& o)
    {
        PyArrayObject* arr = reinterpret_cast<PyArrayObject*>(o.ptr());
        return PyArray_STRIDES(arr);
    }

    static npy_intp stride(const py::object& o, int index)
    {
        PyArrayObject* arr = reinterpret_cast<PyArrayObject*>(o.ptr());
        return PyArray_STRIDE(arr, index);
    }

    static NPY_TYPES dataType(const py::object& o)
    {
        PyArrayObject* a = reinterpret_cast<PyArrayObject*>(o.ptr());
        return static_cast<NPY_TYPES>(PyArray_TYPE(a));
    }

    static std::size_t elementCount(const py::object& o)
    {
        PyArrayObject* arr = reinterpret_cast<PyArrayObject*>(o.ptr());
        return PyArray_SIZE(arr);
    }


    template<class T> static py::object ensureArray(const py::object& o, int dimensions)
    {
        return ensureArray<T>(o, dimensions, dimensions);
    }

    // Make sure object is an array (read only) with given number of dimensions
    template<class T> static py::object ensureArray(const py::object& o, int min_dimensions, int max_dimensions)
    {
        py::object arr = py::reinterpret_steal<py::object>(PyArray_FromAny(o.ptr(), PyArray_DescrFromType(NumPy::type<T>()), min_dimensions, max_dimensions, NPY_ARRAY_CARRAY_RO | NPY_ARRAY_ENSUREARRAY | NPY_ARRAY_FORCECAST, nullptr));
        return arr;
    }
    

    template<class T> static py::object fromValues(const std::initializer_list<T>& values)
    {
        npy_intp dims = values.size();
        py::object arr = py::reinterpret_steal<py::object>(PyArray_SimpleNew(1, &dims, static_cast<int>(NumPy::type<T>())));
        std::copy_n(values.begin(), values.size(), data<T>(arr));
        return arr;
    }

    template<class T> static py::object create(const std::initializer_list<npy_intp>& size)
    {
        py::object arr = py::reinterpret_steal<py::object>(PyArray_SimpleNew(static_cast<int>(size.size()), const_cast<npy_intp*>(size.begin()), static_cast<int>(NumPy::type<T>())));
        return arr;
    }

    template<class T> static py::object fromValues(const std::initializer_list<npy_intp>& size, const std::initializer_list<T>& values)
    {
        py::object arr = py::reinterpret_steal<py::object>(PyArray_SimpleNew(static_cast<int>(size.size()), const_cast<npy_intp*>(size.begin()), static_cast<int>(NumPy::type<T>())));
        std::copy_n(values.begin(), values.size(), data<T>(arr));
        return arr;
    }

    template<class T> static py::object fromData(const std::initializer_list<npy_intp>& size, const T* values)
    {
        py::object arr = py::reinterpret_steal<py::object>(PyArray_SimpleNew(static_cast<int>(size.size()), const_cast<npy_intp*>(size.begin()), static_cast<int>(NumPy::type<T>())));
        std::copy_n(values, elementCount(arr), data<T>(arr));
        return arr;
    }

    
    template<class T> static T* data(const py::object& o)
    {
        PyArrayObject* arr = reinterpret_cast<PyArrayObject*>(o.ptr());
        return reinterpret_cast<T*>(PyArray_DATA(arr));
    }

    template<class T> static T* data(const py::object& o, int i)
    {
        PyArrayObject* arr = reinterpret_cast<PyArrayObject*>(o.ptr());
        void* ptr = PyArray_GETPTR1(arr, i);
        return reinterpret_cast<T*>(ptr);
    }

    template<class T> static T* data(const py::object& o, int i, int j)
    {
        PyArrayObject* arr = reinterpret_cast<PyArrayObject*>(o.ptr());
        void* ptr = PyArray_GETPTR2(arr, i, j);
        return reinterpret_cast<T*>(ptr);
    }

    template<class T> static T* data(const py::object& o, int i, int j, int k)
    {
        PyArrayObject* arr = reinterpret_cast<PyArrayObject*>(o.ptr());
        void* ptr = PyArray_GETPTR3(arr, i, j, k);
        return reinterpret_cast<T*>(ptr);
    }

    template<class T> static T value(const py::object& o, int i)
    {
        PyArrayObject* arr = reinterpret_cast<PyArrayObject*>(o.ptr());
        void* ptr = PyArray_GETPTR1(arr, i);
        return NumPy::cast<T>(dataType(object), ptr);
    }

    template<class T> static T value(const py::object& o, int i, int j)
    {
        PyArrayObject* arr = reinterpret_cast<PyArrayObject*>(o.ptr());
        void* ptr = PyArray_GETPTR2(arr, i, j);
        return NumPy::cast<T>(dataType(object), ptr);
    }

    template<class T> static T value(const py::object& o, int i, int j, int k)
    {
        PyArrayObject* arr = reinterpret_cast<PyArrayObject*>(o.ptr());
        void* ptr = PyArray_GETPTR3(arr, i, j, k);
        return NumPy::cast<T>(dataType(object), ptr);
    }

    template<class T, size_t N> static std::array<T, N> values(const py::object& o)
    {
        PyArrayObject* arr = reinterpret_cast<PyArrayObject*>(o.ptr());
        int ndims = PyArray_NDIM(arr);
        void* ptr = PyArray_DATA(arr);
        return NumPy::cast<T, N>(dataType(o), ptr, PyArray_STRIDE(arr, ndims - 1));
    }
};

