#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    QPixmap backgroundImage(":new/prefix1/background.png");
    ui->background->setPixmap(backgroundImage);
    //passt das Bild an MainFenster
    ui->background->setScaledContents(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_schiffeplatzieren_clicked(){
//    ui->stackedWidget->setCurrentIndex(1);
    SecondWindow *secondWindow = new SecondWindow(nullptr,10,0,0,0,0,nullptr);
    secondWindow->show();
    this->close();
}

//void MainWindow::on_back_clicked(){
//    ui->stackedWidget->setCurrentIndex(0);
//}
