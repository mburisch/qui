#pragma once

#include "qui.h"

#include <QQuickImageProvider>
#include <QQuickItem>
#include <QQuickPaintedItem>


void initQuickItems(py::module& m);


class ImageItem : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
    Q_PROPERTY(QSize  imageSize READ imageSize NOTIFY imageSizeChanged)
    Q_PROPERTY(bool   smooth READ smooth WRITE setSmooth NOTIFY smoothChanged)

public:
    ImageItem(QQuickItem* parent = nullptr);

    const QImage& image() const;
    void setImage(const QImage& image);

    QSize imageSize() const;

    bool smooth() const;
    void setSmooth(bool smooth);

    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* updatePaintNodeData) override;

Q_SIGNALS:
    void imageChanged();
    void imageSizeChanged();
    void smoothChanged();

private:
    QImage _image;
    bool _smooth;
    bool _updateTexture;
};



class CanvasItem : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(py::object painter READ callback WRITE setCallback NOTIFY callbackChanged)

public:
    CanvasItem(QQuickItem* parent = nullptr);

    py::object callback() const;
    void setCallback(const py::object& callback);

    void paint(QPainter* painter) override;


Q_SIGNALS:
    void callbackChanged();

private:
    py::object _callable;
};