#include "ui/trigger.h"
#include "ui_trigger.h"

trigger::trigger(int index, MainWindow *main_window, QWidget *parent) :
    main_window(main_window),
    QWidget(parent),
    ui(new Ui::trigger)
{
    ui->setupUi(this);

    this->frame = ui->frame;
    this->frame->setFrameShadow(QFrame::Plain);
    ui->key_number->setText(QString::fromStdString(std::to_string(index)));

    this->root_actions.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}

trigger::~trigger()
{
    delete ui;
}
