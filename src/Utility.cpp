
#include "Utility.h"
#include "Types.h"


ApplicationArguments::ApplicationArguments()
{
    QList<QString> args = { "qui" };
    init(args);
}


ApplicationArguments::ApplicationArguments(const py::list& arguments)
{
    QList<QString> args;
    if (arguments.size() > 0)
    {
        for (int i = 0; i < py::len(arguments); i++)
            QString s = Type<QString>::fromPython(arguments[i]);
    }
    else
        args.append("qui");

    init(args);
}


void ApplicationArguments::init(const QList<QString>& arguments)
{
    for (auto& s : arguments)
        _args.append(s.toUtf8());

    for (auto& s : _args)
        argv.append(s.data());

    argc = argv.size();
}
