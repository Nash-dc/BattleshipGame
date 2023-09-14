#ifndef BATTLESHIPGAME_H
#define BATTLESHIPGAME_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QVector>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class BattleshipGame;
}
QT_END_NAMESPACE

class BattleshipGame : public QWidget
{
    Q_OBJECT

public:
    BattleshipGame(QWidget *parent);

    ~BattleshipGame();

    // The following functions will be used by SecondWindow

    void setNextShip(int length, bool isVertical);

    void setSchiffNumer2place(int number);

    void lockBoard();

    void unlockBoard();

    bool isShipPlaced();
    void setzeroBoard();


    QVector<QVector<bool>>& getOccupied();


private slots:
    void onCellClicked();

public:
    void initializeGame();

    void placeShipsManually();

    bool isGameOver() const;

    QPushButton *createCellButton();

    void setCellOccupied(int row, int col);

    void freeCell(int row, int col);


    bool isCellOccupied(int row, int col) const;


    void updateCellAppearance(int row, int col);


    void updateMissCellAppearance(int row, int col);
    void updateHit(int row, int col);

    bool setFire(int row,int col);

    void showMessage(const QString &message);
    bool possibleToPlace_R(int row, int col);
    bool possibleToPlace_L(int row, int col);
    bool possibleToPlace_U(int row, int col);
    bool possibleToPlace_D(int row, int col);
    bool checkPlaceSuccessful();
    void callControlCenter();
    void setshipPlacePhase(bool isPlacePhase);

private:
    int shipLength;
    bool vertical;
    bool placeSucced;
    int shipsToPlace;// Number of ships left to place
    int size2,size3,size4,size5;
    bool shipPlacePhase = true;

    static const int boardSize = 10;
    QVector<QVector<QPushButton *>> board;
    QVector<QVector<bool>> occupied;

signals:
    void cellClicked(int row, int col);
};
#endif // BATTLESHIPGAME_H

