#include "controlcenter.h"

controlCenter::controlCenter(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    this->resize(800, 600);
    setWindowTitle("Game Controller");
    QPushButton *button = new QPushButton("Click here to show two boards and call the network center",this);
    layout->addWidget(button);
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(button, &QPushButton::clicked, this, &controlCenter::onShowClicked);

    setgegnerWindow();
    connect(gegnerWindow, &BattleshipGame::cellClicked, this, &controlCenter::handleCellClicked);
    network = new newnetwork(nullptr);
    network -> show();
    dataReadTimer = new QTimer(this);
    connect(dataReadTimer, &QTimer::timeout, this, &controlCenter::handleReceiveddata);
    dataReadTimer->start(1000);

}


void controlCenter::setselbstWindow(BattleshipGame *gameWindow){
    selbstWindow = gameWindow;
    selbstWindow->setWindowTitle("Mein Board");
    selbstWindow->setDisabled(true);
}

void controlCenter::setgegnerWindow(){
    gegnerWindow = new BattleshipGame(nullptr);
    gegnerWindow->setWindowTitle("Gegner Board");
    gegnerWindow->setzeroBoard();
    gegnerWindow->setshipPlacePhase(false);
//    gegnerWindow->setDisabled(true);
}

void controlCenter::onShowClicked(){
    if(selbstWindow){
        selbstWindow->show();
    }
    if(gegnerWindow){
        gegnerWindow->show();
    }
//    network = new newnetwork(nullptr);
//    network -> show();
    this->hide();
}

void controlCenter::handleCellClicked(int row, int col)
{
    if(!network->isConnected){
        return;
    }
    uint8_t x=row;
    uint8_t y=col;
    network->xysendPair.first = x;
    network->xysendPair.second = y;
    myPair.first = x;
    myPair.second = y;
    network->schussMap[myPair]=-1;
    schussMapCT[myPair]=-1;
    network->send();
    gegnerWindow->setDisabled(true);

}

void controlCenter::handleReceiveddata(){
    if(network->schussMap[network->xyreceivePair] == -1){
        int x= network->xyreceivePair.first;
        int y= network->xyreceivePair.second;

        if(selbstWindow->setFire(x,y)){
            network->sendStatusCode(1);
            selbstWindow->updateHit(x,y);
            selbstWindow->freeCell(x,y);
            qDebug() << "statuscode sent";
            network->schussMap[network->xyreceivePair] = -2;
            if(selbstWindow->isGameOver()){
                showMessage("Game Over , you lose");
                selbstWindow->close();
                gegnerWindow->close();
            }
            gegnerWindow->setDisabled(false);
          }
        else{
            network->sendStatusCode(0);
            qDebug() << "statuscode sent";
            network->schussMap[network->xyreceivePair] = -2;
            gegnerWindow->setDisabled(false);
        }


        }
    if(network->updateable){
        int x= myPair.first;
        int y= myPair.second;
        int i =network->schussMap[myPair];
        if(i==0){
            gegnerWindow->updateMissCellAppearance(x,y);
            network->updateable=false;
            qDebug() << "Gegner Board updated";

        }
        else{
            gegnerWindow->updateHit(x,y);
            network->updateable=false;
            qDebug() << "Gegner Board updated";

        }
    }
    else{
        return;
    }

}

void controlCenter::showMessage(const QString &message)
{
    QMessageBox::information(this, "Battleship Game", message);
}
