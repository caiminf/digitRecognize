#include "paintarea.h"
#include "ui_paintarea.h"
#include <QtGui>
#include <QtCore>

PaintArea::PaintArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaintArea)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StaticContents);
    drawing = false;
}

PaintArea::~PaintArea()
{
    delete ui;
}

QSet<int> PaintArea::getSet()
{
    return set;
}

void PaintArea::clear()
{
    image.fill(qRgb(255,255,255));
    set.clear();
    update();
}

void PaintArea::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QRect dirtyRect = e->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
}

void PaintArea::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton){
        lastPoint = e->pos();
        drawing = true;
    }
}

void PaintArea::mouseMoveEvent(QMouseEvent *e)
{
    if((e->buttons() & Qt::LeftButton) && drawing){
        drawLineTo(e->pos());
    }
}

void PaintArea::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton && drawing){
        drawLineTo(e->pos());
        drawing = false;
    }
}

void PaintArea::resizeEvent(QResizeEvent *e)
{
    if (width() > image.width() || height() > image.height()){
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(e);
}

void PaintArea::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;
    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

void PaintArea::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&image);
    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine,
                        Qt::RoundCap,Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    update(QRect(lastPoint, endPoint).normalized()
           .adjusted(-3, -3, 3, 3));
    int x = endPoint.x() / 10;
    int y = endPoint.y() / 10;
    int pos = x * 20 + y;
    if(pos<400 && pos >= 0){
        set.insert(pos);
    }
    lastPoint = endPoint;
}
