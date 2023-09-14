#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

#include "network.h"

class SecondWindow : public QMainWindow
{
    Q_OBJECT

public:
    SecondWindow( BattleshipGame* gameBoard, int number,int length2, int length3, int length4, int length5,QWidget *parent = nullptr);
    ~SecondWindow();

private:
    void showMessage(const QString &message);
    void calltheBoard(int length,bool vertical);


private slots:
    void startPlace();

private:
    QComboBox *shipLengthComboBox;
    QCheckBox *verticalCheckBox;
    QPushButton *startButton;
    BattleshipGame *gameWindow;

private:
    int shipNumber;
    bool isMyturn;
    bool isInternet;
    int length2ship,length3ship, length4ship, length5ship;

};

#endif // SECONDWINDOW_H
