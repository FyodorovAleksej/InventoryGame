#include "topwidget.h"

TopWidget::TopWidget(QWidget *parent): QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setAttribute(Qt::WA_UnderMouse);
    QHBoxLayout* horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->setMargin(0);

    modeCheckBox = new QComboBox();
    modeCheckBox->setCurrentText("choose mode");
    modeCheckBox->addItem("server");
    modeCheckBox->addItem("client");

    connect(modeCheckBox, SIGNAL(activated(QString)), this, SLOT(checkBoxActive(QString)));

    addressEdit = new QLineEdit("localhost");
    QPushButton* newGameButton = new QPushButton("Новая игра");
    connect(newGameButton, SIGNAL(clicked(bool)), this, SLOT(on_newGameButton_clicked()));

    QPushButton* exitButton = new QPushButton("Выход");
    connect(exitButton, SIGNAL(clicked(bool)), this, SLOT(on_exitButton_clicked()));

    horizontalLayout->addWidget(this->modeCheckBox);
    horizontalLayout->addWidget(this->addressEdit);
    this->addressEdit->setVisible(false);
    horizontalLayout->addWidget(newGameButton);
    horizontalLayout->addWidget(exitButton);
}

TopWidget::~TopWidget()
{
    delete this->modeCheckBox;
}

void TopWidget::checkBoxActive(QString mode)
{
    if (mode == "client")
    {
        this->addressEdit->setVisible(true);
    }
    if (mode == "server")
    {
        this->addressEdit->setVisible(false);
    }
}

void TopWidget::on_exitButton_clicked()
{
    emit this->exitSignal();
}

void TopWidget::on_newGameButton_clicked()
{
    emit this->newGameSignal(this->modeCheckBox->currentText());
}

QString TopWidget::getAddress()
{
    if (this->addressEdit->isVisible())
    {
        return addressEdit->text();
    }
    return QString();
}
