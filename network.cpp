#include "network.h"
#include <QDebug>

newnetwork::newnetwork(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    QRadioButton* serverRadioButton = new QRadioButton("Server", this);
    QRadioButton* clientRadioButton = new QRadioButton("Client", this);

    QHBoxLayout* radioLayout = new QHBoxLayout;
    radioLayout->addWidget(serverRadioButton);
    radioLayout->addWidget(clientRadioButton);

    ipAddressLineEdit = new QLineEdit(this);
    portLineEdit = new QLineEdit(this);
    ipAddressLineEdit->setPlaceholderText("IP Address");
    portLineEdit->setPlaceholderText("Port");


    startServerButton = new QPushButton("Start Server", this);
    connectClientButton = new QPushButton("Connect", this);
    endServerButton = new QPushButton ("End Server", this);
    disconnectButton = new QPushButton("Disconnect", this);
    sendButton = new QPushButton("Ready");

    QVBoxLayout* inputLayout = new QVBoxLayout;
    inputLayout->addWidget(ipAddressLineEdit);
    inputLayout->addWidget(portLineEdit);

    QHBoxLayout* buttonLayout1 = new QHBoxLayout;
    buttonLayout1->addWidget(startServerButton);
    buttonLayout1->addWidget(connectClientButton);

    QHBoxLayout* buttonLayout2 = new QHBoxLayout;
    buttonLayout2->addWidget(endServerButton);
    buttonLayout2->addWidget(disconnectButton);

    _chatTextEdit = new QTextEdit(this);
    _messageLineEdit = new QLineEdit(this);

    layout->addLayout(radioLayout);
    layout->addLayout(inputLayout);
    layout->addLayout(buttonLayout1);
    layout->addWidget(_chatTextEdit);
    layout->addWidget(_messageLineEdit);
    layout->addLayout (buttonLayout2);
    layout->addWidget(sendButton);

    ipAddressLineEdit->hide();
    portLineEdit->hide();
    startServerButton->hide();
    connectClientButton->hide();
    endServerButton->hide();
    disconnectButton->hide();
    sendButton->show();

    connect(serverRadioButton, &QRadioButton::toggled, this, &newnetwork::serverRadioButtonToggled);
    connect(clientRadioButton, &QRadioButton::toggled, this, &newnetwork::clientRadioButtonToggled);


    _stream.setByteOrder(QDataStream::BigEndian);

    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &newnetwork::newClientConnection);

    connect(startServerButton, &QPushButton::clicked, this, &newnetwork::startServerClicked);
    connect(connectClientButton, &QPushButton::clicked, this, &newnetwork::connectClicked);
    connect(endServerButton, &QPushButton::clicked, this, &newnetwork::endServerClicked);
    connect(disconnectButton, &QPushButton::clicked, this, &newnetwork::disconnectClicked);
    connect(sendButton, &QPushButton::clicked, this, &newnetwork::sendBytes);

    socket = nullptr;

}

void newnetwork::startServerClicked()
{
    if (socket) {
        socket->disconnectFromHost();
        socket->deleteLater();
        socket = nullptr;
    }

    quint16 portServer = portLineEdit->text().toUShort();

    if (portServer < 1024){
        _chatTextEdit->setPlainText("Error: Invalid port number.");
        return ;
    }

    if (server->listen(QHostAddress::Any, portServer)) {
        qDebug() << "Server is listening on port " << portServer;
        _chatTextEdit->setPlainText("Server started successfully.");
    } else {
        qDebug() << "Failed to start the server.";
        _chatTextEdit->setPlainText("Failed to start the server.");
    }
}

void newnetwork::connectClicked()
{
    if (socket) {
        socket->disconnectFromHost();
        socket->deleteLater();
        socket = nullptr;
    }

    QString host = ipAddressLineEdit->text();
    quint16 port = portLineEdit->text().toUShort();

    socket = new QTcpSocket(this);
    _chatTextEdit->setPlainText("Connecting to Server");

    qDebug() << "Connecting to " << host << " on port " << port << "\n";
    socket->connectToHost(host, port);

    connect(socket, &QTcpSocket::connected, this, &newnetwork::enableConnection);
    connect(socket, &QTcpSocket::disconnected, this, &newnetwork::disconnectClicked);
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
}

void newnetwork::serverRadioButtonToggled(bool checked)
{
    isServer = checked;
    if (checked) {
        portLineEdit->show();
        startServerButton->show();
        endServerButton->show();
        ipAddressLineEdit->hide();
        connectClientButton->hide();
        disconnectButton->hide();
    }
}

void newnetwork::clientRadioButtonToggled(bool checked)
{
    isServer = !checked;
    if (checked) {
        ipAddressLineEdit->show();
        portLineEdit->show();
        connectClientButton->show();
        disconnectButton->show();
        startServerButton->hide();
        endServerButton->hide();
    }
}

void newnetwork::newClientConnection()
{
    if (socket) {
        socket->disconnectFromHost();
        socket->deleteLater();
        socket = nullptr;
    }

    isConnected=true;
    _chatTextEdit->setPlainText("Client connected.");
    endServerButton->setEnabled(true);

    socket = server->nextPendingConnection();

    connect(socket, &QTcpSocket::readyRead, this, &newnetwork::receive);
    connect(_messageLineEdit, &QLineEdit::returnPressed, this, &newnetwork::sendChat);

}

void newnetwork::enableConnection()
{
    if (!socket) {
        return;
    }

    isConnected=true;
    _chatTextEdit->setPlainText("Connected.");
    disconnectButton->setEnabled(true);
    _stream.setDevice(socket);

    connect(socket, &QTcpSocket::readyRead, this, &newnetwork::receive);
    connect(_messageLineEdit, &QLineEdit::returnPressed, this, &newnetwork::sendChat);
}

void newnetwork::endServerClicked()
{
    if (server->isListening()) {
        if (socket) {
            socket->disconnectFromHost();
            socket->deleteLater();
            socket = nullptr;
        }
        server->close();
        qDebug() << "Server closed.";
        _chatTextEdit->setPlainText("Server closed.");
    }
}

void newnetwork::disconnectClicked()
{
    if(!socket){
        return;
    }
    _chatTextEdit->setPlainText("Disconnected");
    if (socket) {
        socket->disconnectFromHost();
        socket->deleteLater();
        socket = nullptr;
    }
    disconnectButton->setEnabled(false);
}

void newnetwork::socketError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Socket error: " << socketError;
    _chatTextEdit->setPlainText("Socket error: " + socket->errorString());
    if (socket) {
        socket->disconnectFromHost();
        socket->deleteLater();
        socket = nullptr;
    }
    disconnectButton->setEnabled(false);
}

void newnetwork::sendStatusCode(int code){
    if (!socket)
    {
        return;
    }
    QByteArray messageData;
        messageData.append(0x10);
        //messageData.append(0x01);
        uint8_t statuscode = code;
        messageData.append(statuscode);

        socket->write(messageData);
        socket->flush();

        qDebug() << "Message sent: " << messageData;
}


void newnetwork::send()
{
    if (!socket)
    {
        return;
    }

    QByteArray messageData;

    messageData.append(0x03);
    //messageData.append(0x02);
    messageData.append(xysendPair.first);
    messageData.append(xysendPair.second);

    socket->write(messageData);
    socket->flush();
    updateable=false;

    qDebug() << "Message sent: " << messageData;

}

void newnetwork::receive()
{
    if (!socket)
    {
        return;
    }

    QByteArray receivedData = socket->readAll();
    if (receivedData.isEmpty()) {
        return;
    }

    uint8_t cmd = static_cast<uint8_t>(receivedData.at(0));
    qDebug() << "cmd: " << cmd;
    if (cmd == 0x03 && receivedData.size() == 3) {
        x = static_cast<uint8_t>(receivedData.at(1));
        y = static_cast<uint8_t>(receivedData.at(2));
        qDebug() << "received Data: " << receivedData;
        qDebug() << "cmd: " << cmd;
        qDebug() << "x:" << x;
        qDebug() << "y:" << y;
        xyreceivePair.first = x;
        xyreceivePair.second = y;
        schussMap[xyreceivePair] = -1;
    }
    if(cmd==0x81){
        QString chatMessage = receivedData.mid(1);
        _chatTextEdit->append("Gegner: "+chatMessage);
    }
    if(cmd==0x10){
        int statuscode = static_cast<uint8_t>(receivedData.at(1));
        qDebug()<<"statuscode:"<<statuscode;
        if(statuscode == 1){
            schussMap[xysendPair] = 1;
        }
        else{
            schussMap[xysendPair] = 0;
        }
        updateable=true;
    }

}

void newnetwork::setselbstBoard(BattleshipGame *selbstBoard){
    battleshipGame = selbstBoard;
}

void newnetwork::showMessage(const QString &message)
{
    QMessageBox::information(this, "Network", message);
}

void newnetwork::sendChat()
{
    if (!socket) {
        return;
    }

    QString message = _messageLineEdit->text();

    if (!message.isEmpty()) {
        QByteArray chatData;
        chatData.append(0x81);
        chatData.append(message.toUtf8());
        socket->write(chatData);
        socket->flush();
        _chatTextEdit->append("Ich: " + message);
        _messageLineEdit->clear();
    }
}

void newnetwork::sendBytes()
{
    QByteArray byte("\0x10\0x01\0x00", 3);
    socket->write(byte);
    socket->flush();
    qDebug() << "Ready sent";
}


