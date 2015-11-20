#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "paintarea.h"
#include <QtGui>
#include <QtCore>
#include "svm.h"
#include "digitRec.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>
#include <locale.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    PaintArea *paintArea;
    int currentNum;
    int cnt;
    struct svm_problem prob;
    struct svm_model *model;
    struct svm_parameter param;
    bool isTrained;

    FILE *fp;

    void setProb(double digit, int index);
};

#endif // MAINWINDOW_H
