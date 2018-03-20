#pragma once

#include "qui.h"
#include <QVector>

class ApplicationArguments
{
public:
    ApplicationArguments();
    ApplicationArguments(const py::list& arguments);

    QVector<char*> argv;
    int argc;

private:
    void init(const QList<QString>& arguments);

private:
    QVector<QByteArray> _args;
};


