#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    paintArea = new PaintArea(this);
    this->setWindowTitle("Digits Recognition");

    prob.l = TRAIN_DATA_CNT;
    prob.y = new double[TRAIN_DATA_CNT];
    prob.x = new svm_node * [TRAIN_DATA_CNT];
    currentNum = 0;
    cnt = 0;
    isTrained = false;

    param.svm_type = C_SVC;
    param.kernel_type = RBF;
    param.degree = 3;
    param.gamma = double(1)/double(FEATURE_CNT);	// 1/num_features
    param.coef0 = 0;
    param.nu = 0.5;
    param.cache_size = 100;
    param.C = 1;
    param.eps = 1e-3;
    param.p = 0.1;
    param.shrinking = 1;
    param.probability = 0;
    param.nr_weight = 0;
    param.weight_label = NULL;
    param.weight = NULL;

    fp = fopen("inputData", "w");
}

MainWindow::~MainWindow()
{
    delete ui;
    fclose(fp);
    svm_free_model_content(model);
    delete paintArea;
}

void MainWindow::on_pushButton_clicked()
{
    if(isTrained){
        QList<int> list = paintArea->getSet().values();
        qSort(list.begin(), list.end());
        int size = list.length();
        struct svm_node *x = new svm_node[FEATURE_CNT + 1];
        for(int i=0;i<FEATURE_CNT;i++){
            x[i].index = i + 1;
            x[i].value = 0;
        }
        for(int i=0;i<size;i++){
            x[list[i]].value = 1;
        }
        x[FEATURE_CNT].index = -1;
        svm_save_model("digitRec.model", model);
        double pred = svm_predict(model, x);
        qDebug()<<"prediction "<<pred;
        ui->label_2->setText(QString::number(pred));
        paintArea->clear();
    }
    else{
        if(cnt<INPUT_CNT){
            int index = currentNum * INPUT_CNT + cnt;
            setProb(currentNum, index);
            paintArea->clear();
            cnt++;
            if(cnt == INPUT_CNT){
                currentNum++;
                if(currentNum==10){
                    const char * error_msg = svm_check_parameter(&prob, &param);
                    if(error_msg){
                        qDebug()<<"parameter error";
                    }
                    qDebug()<<"training";
                    model = svm_train(&prob, &param);
                    qDebug()<<"trained";
                    isTrained = true;
                    ui->label->setText("your input digit is:");
                    ui->label_2->setText(" ");
                }
                else{
                    ui->label_2->setText(QString::number(currentNum));
                    cnt = 0;
                }
            }
        }
    }
}

void MainWindow::setProb(double digit, int index)
{
    QList<int> list = paintArea->getSet().values();
    qSort(list.begin(), list.end());
    int size = list.size();
    prob.x[index] = new svm_node[FEATURE_CNT + 1];
    for(int i=0;i<FEATURE_CNT;i++){
        prob.x[index][i].index = i + 1;
        prob.x[index][i].value = 0;
    }
    for(int i=0;i<size;i++){
        prob.x[index][list[i]].value = 1;
    }
    prob.x[index][FEATURE_CNT].index = -1;

    prob.y[index] = digit;
#ifdef DEBUG
    qDebug()<<"current_index = "<<index;
    qDebug()<<"y = "<<prob.y[index];
    for(int i=0;i<=FEATURE_CNT;i++){
        qDebug() << prob.x[index][i].index << ','
                 <<prob.x[index][i].value;
    }
#endif
    fprintf(fp, "%lf", prob.y[index]);
    for(int i=0;i<FEATURE_CNT;i++){
        fprintf(fp, " %d:%lf", prob.x[index][i].index,
                prob.x[index][i].value);
    }
    fprintf(fp, "\n");
}
