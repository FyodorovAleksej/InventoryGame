#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    this->topWidget = new TopWidget(this);
    connect(topWidget, SIGNAL(newGameSignal(QString)), this, SLOT(on_newGameButton_clicked(QString)));
    connect(topWidget, SIGNAL(exitSignal()), this, SLOT(on_exitButton_clicked()));

    QFrame *frame = new QFrame;
    QHBoxLayout *frameLayout = new QHBoxLayout(frame);
    this->inventory = new Inventory(3,3, this);
    this->inventory->setAcceptDrops(true);

    QVBoxLayout *rightLayout = new QVBoxLayout(frame);

    QPushButton *button = new QPushButton("Главное меню");
    connect (button, SIGNAL(clicked(bool)), this, SLOT(on_mainMenuButton_clicked()));
    InventoryItem* item = new AppleItem(this);
    item->setPixmap(QPixmap(item->getPicturePath()));

    rightLayout->addWidget(item);
    rightLayout->addWidget(button);

    frameLayout->addWidget(this->inventory);
    frameLayout->addLayout(rightLayout);
    setCentralWidget(frame);

    this->setMinimumSize(500, 500);
    setAcceptDrops(true);
    on_mainMenuButton_clicked();
    this->move(0,0);
}

MainWindow::~MainWindow()
{
    delete topWidget;
    delete inventory;
}

void MainWindow::on_mainMenuButton_clicked()
{
        QRect parentRect = this->geometry();
        topWidget->resize(parentRect.width(), 100);
        topWidget->move(parentRect.left(), parentRect.top() + 25);
        topWidget->show();
        this->setEnabled(false);
        topWidget->setEnabled(true);

}

void MainWindow::on_newGameButton_clicked(QString mode)
{
    this->inventory->newGame(mode + "\t" + topWidget->getAddress());
    topWidget->close();
    this->setEnabled(true);
}

void MainWindow::on_exitButton_clicked()
{
    this->close();
}

void MainWindow::moveEvent(QMoveEvent *event)
{
    QRect parentRect = this->geometry();
    topWidget->resize(parentRect.width(), 100);
    topWidget->move(parentRect.left(), parentRect.top() + 25);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    this->topWidget->close();
}
