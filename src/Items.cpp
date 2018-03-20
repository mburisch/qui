
#include "Items.h"
#include "Types.h"

#include <QSGImageNode>
#include <QSGTexture>
#include <QQuickWindow>
#include <QQuickItem>
#include <QQuickView>
#include <QQmlEngine>
#include <QPainter>


void initQuickItems(py::module& m)
{
    qmlRegisterType<ImageItem>("qui", 1, 0, "ImageView");
    qmlRegisterType<CanvasItem>("qui", 1, 0, "CanvasView");
}

//-----------------------------------------------------------------------------

ImageItem::ImageItem(QQuickItem* parent) :
    QQuickItem(parent),
    _smooth(false),
    _updateTexture(true)    // Force initial update of texture
{
    setFlag(QQuickItem::ItemHasContents, true);
}


void ImageItem::setImage(const QImage& image)
{
    QSize oldSize = _image.size();

    _image = image;
    _updateTexture = true;
    emit imageChanged();

    if (_image.size() != oldSize)
        emit imageSizeChanged();

    update();
}


const QImage& ImageItem::image() const
{
    return _image;
}


QSize ImageItem::imageSize() const
{
    return _image.size();
}


void ImageItem::setSmooth(bool smooth)
{
    if (smooth != _smooth)
    {
        _smooth = smooth;
        emit smoothChanged();
        update();
    }
}


bool ImageItem::smooth() const
{
    return _smooth;
}


QSGNode* ImageItem::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* updatePaintNodeData)
{
    QSGImageNode* node = static_cast<QSGImageNode*>(oldNode);

    if (node == nullptr)
    {
        node = window()->createImageNode();
        node->setOwnsTexture(true);
    }

    if (_smooth)
        node->setFiltering(QSGTexture::Linear);
    else
        node->setFiltering(QSGTexture::Nearest);

    if (_updateTexture)
    {
        QSGTexture* texture = window()->createTextureFromImage(_image);
        _updateTexture = false;
        node->setTexture(texture);
        node->setSourceRect(_image.rect());
    }

    node->setRect(boundingRect());

    return node;
}



//-----------------------------------------------------------------------------

CanvasItem::CanvasItem(QQuickItem* parent) :
    QQuickPaintedItem(parent)
{

}


py::object CanvasItem::callback() const
{
    return _callable;
}


void CanvasItem::setCallback(const py::object& callback)
{
    Require::callable(callback);

    _callable = callback;
    emit callbackChanged();
    update();
}


void CanvasItem::paint(QPainter* painter)
{
    if (_callable)
        _callable(painter);
}
