#ifndef TOPWIDGET_H
#define TOPWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLineEdit>

/**
 * @brief The TopWidget class - the class of mainMenu with checkBox(("server"/)("client" + ipAdress QLineEdit)), buttons ("Новая игра", "Выход")
 */
class TopWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief TopWidget - create mainMenuWidget
     * @param parent - the parent widget (MainWindow)
     */
    explicit TopWidget(QWidget *parent = 0);
    ~TopWidget();

    /**
     * @brief getAddress - getting entered ipAddres, when "client" was selected
     * @return - entered ipAdress
     */
    QString getAddress();
signals:
    /**
     * @brief newGameSignal - the signal of new game. sending to MainWindow
     * @param mode - the string with format: ("server"/"client")
     */
    newGameSignal(QString mode);

    /**
     * @brief exitSignal - the signal of exit game
     */
    exitSignal();

public slots:
    /**
     * @brief checkBoxActive - slot of changing active string of comboBox
     * @param mode - "server"/"client"
     */
    void checkBoxActive(QString mode);

    /**
     * @brief on_exitButton_clicked - the slot of exit button clicked. Sending signal to MainWindow
     */
    void on_exitButton_clicked();

    /**
     * @brief on_newGameButton_clicked - the slot of new game button clicked. Sending signal to MainWindow
     */
    void on_newGameButton_clicked();

private:
    QComboBox* modeCheckBox;
    QLineEdit* addressEdit;
};

#endif // TOPWIDGET_H
