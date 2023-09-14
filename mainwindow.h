#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>

#include "SecondWindow.h"

/**
 * @brief The MainWindow class represents the main window of the application.
 *
 * This class serves as the main entry point of the application and provides a user interface
 * for interacting with the program's features.
 */
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a MainWindow object.
     *
     * @param parent The parent QWidget.
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destroys the MainWindow object.
     */
    ~MainWindow();

private:
    Ui::MainWindow *ui; /**< Pointer to the user interface elements. */

private slots:
    /**
     * @brief Slot for handling the "Schiffe Platzieren" button click event.
     *
     * This slot is called when the user clicks the "Schiffe Platzieren" button, initiating
     * the ship placement process.
     */
    void on_schiffeplatzieren_clicked();
};

#endif // MAINWINDOW_H
