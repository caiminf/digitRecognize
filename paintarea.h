#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QWidget>
#include <QSet>

namespace Ui {
class PaintArea;
}

class PaintArea : public QWidget
{
    Q_OBJECT
    
public:
    explicit PaintArea(QWidget *parent = 0);
    ~PaintArea();
    QSet<int> getSet();
    void clear();

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    
private:
    void drawLineTo(const QPoint &endPoint);
    void resizeImage(QImage *, const QSize &);

    Ui::PaintArea *ui;
    bool drawing;
    QImage image;
    QPoint lastPoint;
    QSet<int> set;
};

#endif // PAINTAREA_H
