#include "imageconverter.h"

#include <QImage>
#include <QPainter>
#include <QSvgRenderer>

ImageConverter::ImageConverter(QObject *parent) : QObject(parent)
{
}

void ImageConverter::saveAsPng(QString input, QString pngname)
{
  QSvgRenderer renderer(input);
  QImage image(256, 256, QImage::Format_ARGB32);
  QPainter painter(&image);
  renderer.render(&painter);
  image.save(pngname);
}
