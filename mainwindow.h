#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QLabel>
#include <QMoveEvent>
#include "topwidget.h"
#include "inventoryitem.h"
#include "appleitem.h"
#include "inventory.h"

namespace Ui {
class MainWindow;
}

/**
 * @brief The MainWindow class - class of main window with all GUI
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief MainWindow - create window with topWidget, inventory, inventoryItem, buttons
     * @param parent - the parent widget
     */
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    /**
     * @brief on_mainMenuButton_clicked - the slot of button ("Главное меню")
     */
    void on_mainMenuButton_clicked();

    /**
     * @brief on_newGameButton_clicked - the slot of button ("Новая игра") from topWidget. Sending signal for inventory with string in format : (("server"/"client") + "\t" + ipAddress)
     * @param mode - the string in format : (("server"/"client") + "\t" + ipAddress)
     */
    void on_newGameButton_clicked(QString mode);

    /**
     * @brief on_exitButton_clicked - the slot of button ("Выход") from topWidget
     */
    void on_exitButton_clicked();
protected:

    /**
     * @brief moveEvent - override method for moving topWidget
     * @param event - the event of move
     */
    void moveEvent(QMoveEvent *event);

    /**
     * @brief closeEvent - override method for closing topWidget
     * @param event - the event of close
     */
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;

    Inventory* inventory;
    TopWidget* topWidget;
};

#endif // MAINWINDOW_H
