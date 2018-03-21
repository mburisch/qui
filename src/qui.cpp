#define QUI_MAIN

#include "qui.h"
#include "Types.h"
#include "Items.h"

int initNumPy()
{
    import_array();
    return 1;
}


PYBIND11_MODULE(qui_, m)
{
    m.doc() = "Qt QML bindings";

    initNumPy();

    initTypes(m);
    initQtTypes(m);
    initQMLTypes(m);
    initPaintTypes(m);
    initQuickItems(m);
}
