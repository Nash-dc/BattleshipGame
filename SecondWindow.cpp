#include "SecondWindow.h"

SecondWindow::SecondWindow(BattleshipGame *gameBoard,int number,int length2,
                           int length3, int length4, int length5, QWidget *parent)
    : QMainWindow(parent), gameWindow(gameBoard), shipNumber(number),
      length2ship (length2), length3ship(length3),
      length4ship(length4), length5ship(length5)
{
    setWindowTitle("Battleship Game - Start Ship placenment");

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QLabel *shipLengthLabel = new QLabel("Select Ship Length:", centralWidget);
    shipLengthComboBox = new QComboBox(centralWidget);
    shipLengthComboBox->addItem("Select Ship Length");
    shipLengthComboBox->addItem("2");
    shipLengthComboBox->addItem("3");
    shipLengthComboBox->addItem("4");
    shipLengthComboBox->addItem("5");

    QLabel *orientationLabel = new QLabel("Vertical Orientation:", centralWidget);
    verticalCheckBox = new QCheckBox(centralWidget);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->addWidget(shipLengthLabel);
    layout->addWidget(shipLengthComboBox);
    layout->addWidget(orientationLabel);
    layout->addWidget(verticalCheckBox);
//  layout->addWidget(gameWindow);

    startButton = new QPushButton("Place the ship", centralWidget);
    layout->addWidget(startButton);

    connect(startButton, &QPushButton::clicked, this, &SecondWindow::startPlace);
  }
SecondWindow::~SecondWindow(){
    //
}
void SecondWindow::startPlace(){
    if (gameWindow==nullptr) {
        gameWindow = new BattleshipGame(nullptr);//hier wird ein neues Spielfeld erzeugt
//      gameWindow->setshipPlacePhase(true);
    }
//    if (shipNumber==0){
//          gameWindow->setDisabled(false);
//          gameWindow->unlockBoard();
//          newnetwork *network=new newnetwork(nullptr);
//          network->setselbstBoard(gameWindow);
//          network->show();
//          this->close();
//          gameWindow->close();
//    }
    if(gameWindow){
        int selectedShipLength = shipLengthComboBox->currentText().toInt();
        bool isVertical = verticalCheckBox->isChecked();
        if (selectedShipLength >= 2 && selectedShipLength <= 5){
            switch(selectedShipLength){//schaue welche Länge ausgewählt worden ist
            case 2:
                if(length2ship<4){
                    calltheBoard(selectedShipLength,isVertical);//nach jedem Platzieren wird die Anzahl der noch zu platzierende Schiffe erhöht
                }
                else{
                    showMessage(QString("You have enough 2-length ships placed,please place other length ship"));
                }
                //hier der Code zu Schließen des Fensters
                break;
            case 3:
                if(length3ship<3){
                    calltheBoard(selectedShipLength,isVertical);
                }
                else{
                    showMessage(QString("You have enough 3-length ships placed,please place other length ship"));
                }
                break;

            case 4:
                if(length4ship<2){
                    calltheBoard(selectedShipLength,isVertical);
                }
                else{
                    showMessage(QString("You have enough 4-length ships placed,please place other length ship"));
                }
                break;
            case 5:
                if(length5ship<1){
                   calltheBoard(selectedShipLength,isVertical);
                }
                else{
                    showMessage(QString("You have enough 5-length ships placed,please place other length ship"));
                }
                break;
            default:
                break;
            }
        }
        else{
            showMessage(QString("Invalid number,please select a valid number"));
            this->show();
        }
        }
}

void SecondWindow::showMessage(const QString &message)
{
    QMessageBox::information(this, "Battleship Game Placement", message);
}

void SecondWindow::calltheBoard(int length,bool vertical){
    showMessage(QString("Place your ship by clicking the cell in board, the ship will always toward right or down")+QString(",%1 ships left to place").arg(shipNumber));
    gameWindow->show();
    gameWindow->setNextShip(length,vertical);//übergebe die Werte an Mahdi um Platzieren zu implementieren//aktualisiere die verbleibende Schiffanzahl
    gameWindow->setEnabled(true);
    this->close();
}
