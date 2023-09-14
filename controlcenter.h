#ifndef CONTROLCENTER_H
#define CONTROLCENTER_H

#include <QWidget>
#include <QVector>
#include <QMessageBox>
#include <QTimer>

#include "battleshipgame.h"
#include "network.h"

/**
 * @brief The controlCenter class represents the central control center of a Battleship game.
 *
 * This class is responsible for managing and coordinating game actions, handling user interactions,
 * and managing network communication.
 */
class controlCenter : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a controlCenter object.This constrcutor will create a new instance of network.
     *
     * @param parent The parent QWidget.
     */
    controlCenter(QWidget *parent = nullptr);

    /**
     * @brief Sets the main game window for the current player.
     *
     * @param gameWindow A pointer to the main BattleshipGame window for the current player.This point is given by SecondWindow.
     */
    void setselbstWindow(BattleshipGame *gameWindow);

    /**
     * @brief Sets the main game window for the opponent player.
     */
    void setgegnerWindow();

private:
    /**
     * @brief Displays a message in the control center.
     *
     * @param message The message to display.
     */
    void showMessage(const QString &message);

private slots:
    /**
     * @brief Slot for handling the "Show" button click event.After the show button is clicke the two board will be shown.
     */
    void onShowClicked();

    /**
     * @brief Slot for handling the cell clicked event.
     *
     * @param row The row of the clicked cell.
     * @param col The column of the clicked cell.
     */
    void handleCellClicked(int row, int col);

    /**
     * @brief Slot for handling received data from the network.
     */
    void handleReceiveddata();

private:
    BattleshipGame* selbstWindow;   /**< Pointer to the player's own game window. */
    BattleshipGame* gegnerWindow;   /**< Pointer to the opponent's game window. */
    newnetwork* network;            /**< Pointer to the network communication object. */
    QTimer* dataReadTimer;          /**< Timer for reading incoming data. */
    QMap<QPair<uint8_t, uint8_t>, int> schussMapCT; /**< Map to store shot information. */
    QPair<uint8_t, uint8_t> myPair; /**< Pair to store coordinates. */

};

#endif // CONTROLCENTER_H
