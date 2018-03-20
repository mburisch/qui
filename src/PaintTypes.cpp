
#include "Types.h"

#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QPointF>


void initPaintTypes(py::module& m)
{
    py::enum_<Qt::PenStyle>(m, "PenStyle")
        .value("NoPen", Qt::NoPen)
        .value("SolidLine", Qt::SolidLine)
        .value("DashLine", Qt::DashLine)
        .value("DotLine", Qt::DotLine)
        .value("DashDotLine", Qt::DashDotLine)
        .value("DashDotDotLine", Qt::DashDotDotLine)
        ;

    py::enum_<Qt::BrushStyle>(m, "BrushStyle")
        .value("NoBrush", Qt::NoBrush)
        .value("SolidPattern", Qt::SolidPattern)
        ;
    
    
    py::class_<QBrush>(m, "QBrush")
        .def(py::init<>())
        .def(py::init<Qt::BrushStyle>())
        .def(py::init<const QColor&>())
        .def(py::init<const QColor&, Qt::BrushStyle>())
        .def_property("color", &QBrush::color, py::overload_cast<const QColor&>(&QBrush::setColor))
        .def_property("style", &QBrush::style, &QBrush::setStyle)
        ;

    py::class_<QPen>(m, "QPen")
        .def(py::init<>())
        .def(py::init<const QColor&>())
        .def(py::init<Qt::PenStyle>())
        .def_property("color", &QPen::color, &QPen::setColor)
        .def_property("style", &QPen::style, &QPen::setStyle)
        .def_property("width", &QPen::widthF, &QPen::setWidthF)
        ;


    py::class_<QPainter>(m, "QPainter")
        .def("begin_native_painting", &QPainter::beginNativePainting)
        .def("end_native_painting", &QPainter::endNativePainting)
        .def("draw_ellipse", py::overload_cast<const QRectF&>(&QPainter::drawEllipse))
        .def("draw_line", py::overload_cast<const QPointF&, const QPointF&>(&QPainter::drawLine))
        .def("draw_image", py::overload_cast<const QRectF&, const QImage&>(&QPainter::drawImage))
        .def("draw_image", py::overload_cast<const QPointF&, const QImage&>(&QPainter::drawImage))
        .def("draw_polyline", py::overload_cast<const QPolygonF&>(&QPainter::drawPolyline))
        .def("draw_text", py::overload_cast<const QPointF&, const QString&>(&QPainter::drawText))
        .def("draw_rect", py::overload_cast<const QRectF&>(&QPainter::drawRect))
        .def("fill_rect", py::overload_cast<const QRectF&, const QBrush&>(&QPainter::fillRect))
        .def_property("pen", &QPainter::pen, py::overload_cast<const QPen&>(&QPainter::setPen))
        .def("set_pen", py::overload_cast<const QColor&>(&QPainter::setPen))
        .def_property("brush", &QPainter::brush, py::overload_cast<const QBrush&>(&QPainter::setBrush))
        .def_property("opacity", &QPainter::opacity, &QPainter::setOpacity)
        .def("reset_transform", &QPainter::resetTransform)
        .def("translate", py::overload_cast<const QPointF&>(&QPainter::translate))
        .def("scale", &QPainter::scale)
        .def("rotate", &QPainter::rotate)
        .def("shear", &QPainter::shear)
        .def_property("transform", &QPainter::transform, &QPainter::setTransform)
        ;
}