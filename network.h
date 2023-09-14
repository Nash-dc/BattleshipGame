/**
 * @file network.h Header file for the newnetwork class
 * @author Mehin Ahmadova
 */

#ifndef NEWNETWORK_H
#define NEWNETWORK_H

#include <QWidget>
#include <QTcpSocket>
#include <QTcpServer>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QDataStream>
#include <QAbstractSocket>
#include <QMessageBox>
#include <QQueue>
#include <QPoint>
#include <QPair>


#include "battleshipgame.h"

/**
 * @brief a class for network communication for both server and clinet sides.
 */
class newnetwork : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief newnetwork Constructor
     *
     * @param parent. The parent QWidget. Defaults to nullptr.
     */
    newnetwork(QWidget *parent = nullptr);

    /**
     * @brief sets the BattelshipGame instance for interaction.
     *
     * @param selbstBoard a pointer to the BattleshipGame instance.
     */
    void setselbstBoard(BattleshipGame *selbstBoard);

    /**
     * @brief indicates whether the network is connected.
     */
    bool isConnected=false;

    /**
     * @brief x and y coordinates for send function.
     */
    QPair<uint8_t, uint8_t> xysendPair;

    /**
     * @brief x and y coordinates for receive function.
     */
    QPair<uint8_t, uint8_t>xyreceivePair;

    /**
     * @brief a mapping of coordinate pairs to integers for shots.
     */
    QMap<QPair<uint8_t, uint8_t>, int> schussMap;


    //The following variables and functions are used by controlCenter class.

    /**
     * @brief indicates whether updates are pending.
     */
    bool updateable = false;

    /**
     * @brief sends x & y coordinates over the network.
     */
    void send();

    /**
     * @brief sends the status code over the network.
     *
     * @param code The status code to send.
     */
    void sendStatusCode(int code);

    /**
     * @brief receives all kinds of inforamtion from the network.
     */
    void receive();

private:
    /**
     * @brief a queue for received x & y coordinates
     */
    QQueue<QPoint> receivedXYqueue;

    /**
     * @brief TCP socket for network communication.
     */
    QTcpSocket *socket = nullptr;

    /**
     * @brief TCP server for incoming connections.
     */
    QTcpServer *server = nullptr;

    /**
     * @brief a data stream for reading and writing network data.
     */
    QDataStream _stream;

    /**
     * @brief an input line for entering an IP address.
     */
    QLineEdit* ipAddressLineEdit = nullptr;

    /**
     * @brief an input line for entering a port number.
     */
    QLineEdit* portLineEdit = nullptr;

    /**
     * @brief a text field for chat messages.
     */
    QTextEdit* _chatTextEdit = nullptr;

    /**
     * @brief an input line for entering chat messages.
     */
    QLineEdit* _messageLineEdit = nullptr;

    /**
     * @brief a button for server side to start the server.
     */
    QPushButton* startServerButton = nullptr;

    /**
     * @brief a button for sevrer side to end the server and cut the connection.
     */
    QPushButton* endServerButton = nullptr;

    /**
     * @brief a button for client side to connect to a server.
     */
    QPushButton* connectClientButton = nullptr;

    /**
     * @brief a button for client side to disconnect from the server.
     */
    QPushButton* disconnectButton = nullptr;

    /**
     * @brief a button for sending 3 bytes data "/x10/x01/x00" (Ready).
     */
    QPushButton* sendButton = nullptr;

    /**
     * @brief indicates whether the user is client side.
     */
    bool isServer = false;

    /**
     * @brief a pointer to the BattleshipGame instance.
     */
    BattleshipGame *battleshipGame = nullptr;

    /**
     * @brief shows message in the message box.
     *
     * @param message The message to display.
     */
    void showMessage(const QString &message);

    /**
     * @brief x coordinate for communication initialized with -1.
     */
    uint8_t x = -1;

    /**
     * @brief y coordinate for commuication initialized with -1
     */
    uint8_t y = -1;

    /**
     * @brief status code for shots.
     */
    uint8_t statusCode;

private slots:
    /**
     * @brief handles the event when the "Start Server" button is clicked.
     */
    void startServerClicked();

    /**
     * @brief handles the event when the "End Server" button is clicked.
     */
    void endServerClicked();

    /**
     * @brief handles the event when the "Connect" button is clicked.
     */
    void connectClicked();

    /**
     * @brief handles the event when the "Disconnect" button is clicked.
     */
    void disconnectClicked();

    /**
     * @brief handles the event when the server radio button is toggled.
     *
     * @param checked Indicates whether the radio button is checked.
     */
    void serverRadioButtonToggled(bool checked);

    /**
     * @brief handles the event when the client radio button is toggled.
     *
     * @param checked Indicates whether the radio button is checked.
     */
    void clientRadioButtonToggled(bool checked);

    /**
     * @brief enables the connection when it is established.
     */
    void enableConnection();

    /**
     * @brief handles socket errors.
     *
     * @param socketError The socket error code.
     */
    void socketError(QAbstractSocket::SocketError socketError);

    /**
     * @brief handles a new client connection to the server.
     */
    void newClientConnection();

    /**
     * @brief sends chat data.
     */
    void sendChat();


    /**
     * @brief sends 3 bytes data "/x10/x01/x00" (Ready).
     */
    void sendBytes();

};

#endif // NEWNETWORK_H
