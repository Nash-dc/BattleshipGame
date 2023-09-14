#include "battleshipgame.h"
#include "SecondWindow.h"
#include "controlcenter.h"


BattleshipGame::BattleshipGame(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Schiff versenken");

    QGridLayout *gridLayout = new QGridLayout(this);

    for (int i = 0; i < boardSize; ++i) {
        QVector<QPushButton *> row;//speichert die Zeigern auf den Buttons in einer Reihe
        QVector<bool> occupiedRow;//speichert den Zustand der Buttons einer Reihe setfire

        //erstellt eine Reihe und initialisiert deren Elemente
        for (int j = 0; j < boardSize; ++j) {
            QPushButton *cellButton = createCellButton();
            gridLayout->addWidget(cellButton, i, j);
            row.push_back(cellButton);//fügen des cellButton in die row Vektor
            occupiedRow.push_back(false);//fügen des Zustands des cellButtons in die ZustandsvektorVektor
        }
        board.push_back(row);//initialisiert den zweidimensionalen Vektor
        occupied.push_back(occupiedRow);//initialisiert den zweidimensionalen Vektor(die neu erstellten Reihen werden hier gespeichert)
    }
    shipsToPlace=10;
    size2=0;
    size3=0;
    size4=0;
    size5=0;
    initializeGame();// Initialize the game board and start manual ship placement.
}

BattleshipGame::~BattleshipGame()
{
    // Clean up any dynamically allocated resources.
}

QPushButton *BattleshipGame::createCellButton()
{
    QPushButton *button = new QPushButton(this);
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(button, &QPushButton::clicked, this, &BattleshipGame::onCellClicked);
    return button;
}

void BattleshipGame::initializeGame()
{
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            board[i][j]->setText("");
            occupied[i][j] = false;
        }
    }

//    placeShipsManually();
}

//void BattleshipGame::placeShipsManually()
//{
//    if (shipsToPlace > 0) {
//        showMessage(QString("Place your %1-length ship by clicking on a cell").arg(shipSizes.first())+QString(",%1 ships left to place").arg(shipsToPlace));
//    } else {
//        showMessage("All ships placed. Click on cells to start the game.");
//        isMyturn = true;
//        return;
//    }
//}

bool BattleshipGame::isGameOver() const
{
    // Check if all ships have been sunk.
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (isCellOccupied(i, j)) {
                return false;
            }
        }
    }
    return true;
}

void BattleshipGame::onCellClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    placeSucced = false;
    if (clickedButton) {
        int row = -1;
        int col = -1;

        for (int i = 0; i < boardSize; ++i) {
            for (int j = 0; j < boardSize; ++j) {
                if (board[i][j] == clickedButton) {
                    row = i;
                    col = j;
                    break;
                }
            }
            if (row != -1 && col != -1)
                break;
        }
        bool isVert =vertical;
        if (row != -1 && col != -1) {

//..................................................................................................................................

            if (shipsToPlace!=0&&shipPlacePhase) {//hier beginnt der Code mit dem Platzieren

                if (!isCellOccupied(row, col)) {//wenn der Button noch verfügbar ist
                    int size = shipLength;//hier wird size mit dem gewähltem Wert vom Benutzer initialiesiert

                    switch(size){
                        case 2:
                            size2=size2+1;
                            break;
                        case 3:
                            size3=size3+1;
                            break;
                        case 4:
                            size4=size4+1;
                            break;
                        case 5:
                            size5=size5+1;
                            break;
                        default:break;
                }

                    //wenn alle moglich Ausrichtung gehen nicht, dann bitte die folgende Function aufrufen;
                    //showMessage(QString("This palce is not avalible, please select a new one"))

                if(!isVert){//wenn horizontal ausgewählt wurde
                    if(possibleToPlace_R(row,col)){//überprüft die rechten Buttons auf Verfügbarkeit und platziert anschließend die Schiffe
                        for(int i=0; i<size;i++){
                            setCellOccupied(row, col+i);
                            updateCellAppearance(row, col+i);
                            board[row][col+i]->setEnabled(false);
                        }
                        shipsToPlace=shipsToPlace-1;
                        if(shipsToPlace!=0){
                            SecondWindow *secondWindow = new SecondWindow(this,shipsToPlace,size2, size3, size4, size5, nullptr);
                            secondWindow->show();
                        }else{
                            callControlCenter();
                        }
                        //hier der Code zum Anzeigen des Fensters
                        placeSucced=true;
                    }else if(possibleToPlace_L(row,col)) {//hier schaut man nach links
                        for(int i=0; i<size;i++){
                            setCellOccupied(row, col-i);
                            updateCellAppearance(row, col-i);
                            board[row][col-i]->setEnabled(false);
                        }
                        shipsToPlace=shipsToPlace-1;
                        if(shipsToPlace!=0){
                            SecondWindow *secondWindow = new SecondWindow(this,shipsToPlace,size2, size3, size4, size5, nullptr);
                            secondWindow->show();
                        }else{
                            callControlCenter();
                        }
                        placeSucced=true;
                        }else{
                            showMessage(QString("This button can't be choosed, please select a new one "));
                        switch(size){
                        case 2:
                            size2=size2-1;
                            break;
                        case 3:
                            size3=size3-1;
                            break;
                        case 4:
                            size4=size4-1;
                            break;
                        case 5:
                            size5=size5-1;
                            break;
                        default:break;}
                        }
                        // else zeige einen Hinweis, dass ein neuer Button ausgewählt wetden soll und bereits angeklickten Button nicht mehr anklickbar
                } else {//hier kommt else für vertikale platzierung
                    if(possibleToPlace_D(row,col)){//überprüft die unteren Buttons auf Verfügbarkeit und platziert anschließend die Schiffe
                        for(int i=0; i<size;i++){
                            setCellOccupied(row+i, col);
                            updateCellAppearance(row+i, col);
                            board[row+i][col]->setEnabled(false);
                        }
                        shipsToPlace=shipsToPlace-1;
                        if(shipsToPlace!=0){
                            SecondWindow *secondWindow = new SecondWindow(this,shipsToPlace,size2, size3, size4, size5, nullptr);
                            secondWindow->show();
                        }else{
                            callControlCenter();
                        }
                        placeSucced=true;
                    }else if(possibleToPlace_U(row,col)) {//hier schaut man nach oben
                        for(int i=0; i<size;i++){
                            setCellOccupied(row-i, col);
                            updateCellAppearance(row-i, col);
                            board[row-i][col]->setEnabled(false);
                            }
                        shipsToPlace=shipsToPlace-1;
                        if(shipsToPlace!=0){
                            SecondWindow *secondWindow = new SecondWindow(this,shipsToPlace,size2, size3, size4, size5, nullptr);
                            secondWindow->show();
                        }else{
                            callControlCenter();
                        }
                        placeSucced=true;
                        }
                        else{
                        showMessage(QString("This button can't be choosed, please select a new one "));
                        switch(size){
                        case 2:
                            size2=size2-1;
                            break;
                        case 3:
                            size3=size3-1;
                            break;
                        case 4:
                            size4=size4-1;
                            break;
                        case 5:
                            size5=size5-1;
                            break;
                        default:break;}
                        }
                        // else zeige einen Hinweis, dass ein neuer Button ausgewählt wetden soll und bereits angeklickten Button nicht mehr anklickbar
                }
                }//hier kommt der else Code, wenn der Button schon gewählt worden ist
//......................................................................................................................................
            }else {//hier beginnt der Code für Schießen(diese Klammer gehört zum if (shipsPlacephase = true))
                emit cellClicked(row, col);
//                if (isCellOccupied(row, col)) {
//                    // It's a hit!
//                    clickedButton->setText("X");
//                    updateCellAppearance(row, col);

//                    freeCell(row, col);
//                    if (isGameOver()) {
//                        showMessage(QString("Game Over! You won!"));
//                        this->close();
//                        // Handle game over condition.
//                    }
//                }
//                else {
//                    // It's a miss!
//                    clickedButton->setText("O");
//                    updateMissCellAppearance(row, col);
//                }
            }
        }

       if(shipsToPlace>0 && placeSucced){//Wenn noch Schiffe verfügbar und entweder oben oder unten schon Plätze verfügbar sind, dann mach die Buttons unanklickbar
            this->setDisabled(true);//Block the window in case of missclick
       }
    }
}

bool BattleshipGame::possibleToPlace_R(int row, int col) {
    bool status = true;
        for(int i=0;i<shipLength;++i){
            //check if array out of bounds
            if (col+i<=9){
                status = status && !isCellOccupied(row,col+i);
            }
            else{
                return false;
            }
        }
        return status;

    }


 bool BattleshipGame::possibleToPlace_L(int row, int col){
     bool status = true;
         for(int i=0;i<shipLength;++i){
             //check if array out of bounds
             if (col-i>=0){
                 status = status && !isCellOccupied(row,col-i);
             }
             else{
                 return false;
             }
         }
         return status;
}

bool BattleshipGame::possibleToPlace_D(int row, int col){
        bool status = true;
        for(int i=0;i<shipLength;++i){
            //check if array out of bounds
            if (row+i<=9){
                status = status && !isCellOccupied(row+i,col);
            }
            else{
                return false;
            }
        }
        return status;

}

bool BattleshipGame::possibleToPlace_U(int row, int col){

        bool status = true;
        for(int i=0;i<shipLength;++i){
            //check if array out of bounds
            if (row-i>=0){
                status = status && !isCellOccupied(row-i,col);
            }
            else{
                return false;
            }
        }
        return status;
}
void BattleshipGame::setCellOccupied(int row, int col)
{
    occupied[row][col] = true;
}

void BattleshipGame::freeCell(int row, int col)
{
    occupied[row][col] = false;
}

bool BattleshipGame::isCellOccupied(int row, int col) const
{
    return occupied[row][col];
}

void BattleshipGame::updateCellAppearance(int row, int col)
{
    // Customize the appearance of the cell based on its state (hit or miss).
    // You can change the button color, enable/disable it, or set an icon/image.
    QPushButton *cellButton = board[row][col];
    cellButton->setStyleSheet("background-color: red; color: white;");
    cellButton->setEnabled(false); // Disable the button after a hit.
}

void BattleshipGame::updateHit(int row, int col){
    QPushButton *cellButton = board[row][col];
    cellButton->setText("X");
    cellButton->setStyleSheet("background-color: red; color: white;");
    cellButton->setEnabled(false);
}

void BattleshipGame::updateMissCellAppearance(int row, int col)
{
    // Customize the appearance of the cell based on its state (hit or miss).
    // You can change the button color, enable/disable it, or set an icon/image.
    QPushButton *cellButton = board[row][col];
    cellButton->setStyleSheet("background-color: blue; color: white;");
    cellButton->setEnabled(false); // Disable the button after a hit.
}

void BattleshipGame::showMessage(const QString &message)
{
    QMessageBox::information(this, "Battleship Game", message);
}

void BattleshipGame::lockBoard(){
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
               QPushButton *cellButton = board[i][j];
               cellButton->setEnabled(false);
            }
        }
}

bool BattleshipGame::isShipPlaced(){
    return placeSucced;
}

void BattleshipGame::setNextShip(int length,bool isVertical){//übernimmt die Länge und die Orientierung vom Combobox
    shipLength = length;
    vertical = isVertical;
}

void BattleshipGame::setSchiffNumer2place(int number){
    shipsToPlace=number;
}

void BattleshipGame::unlockBoard(){
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
               QPushButton *cellButton = board[i][j];
               cellButton->setEnabled(true);
            }
        }
}

void BattleshipGame::setzeroBoard(){
    for (int i = 0; i < 10; i++) {
        QVector<bool> occupiedRow;
        for (int j = 0; j < 10; j++) {
            occupiedRow.push_back(false);
        }
        occupied.push_back(occupiedRow);
    }
}

bool BattleshipGame::checkPlaceSuccessful(){
    return placeSucced;
}

//void BattleshipGame::setshipPlacePhase(bool placement){
//    shipPlacePhase=placement;
//}

bool BattleshipGame::setFire(int row,int col){
    if(occupied[row][col]){
        updateCellAppearance(row,col);
        return true;
    }
    else{
        updateMissCellAppearance(row,col);
        return false;
    }
}

QVector<QVector<bool>>& BattleshipGame::getOccupied(){
    return occupied;
}

void BattleshipGame::callControlCenter()
{
    this->setDisabled(false);
    this->unlockBoard();
    //                          this->setshipPlacePhase(false);

    controlCenter *gameController = new controlCenter(nullptr);
    gameController->setselbstWindow(this);
    gameController->show();
    this->close();
}
void BattleshipGame::setshipPlacePhase(bool isPlacePhase){
    shipPlacePhase = isPlacePhase;
}

