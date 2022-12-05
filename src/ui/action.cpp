#include "ui/action.h"
#include "ui_action.h"
#include <iostream>

action::action(MainWindow *main_window, QWidget *parent) :
    main_window(main_window),
    QWidget(parent),
    ui(new Ui::action)
{
    ui->setupUi(this);

    connect(this, &action::my_edit_action_signal, this, &action::my_edit_action_slot);
}

action::~action()
{
    delete ui;
}

void action::my_edit_action_slot()
{
    trigger *_t = nullptr;
    QString text = QInputDialog::getMultiLineText(this, tr(""), tr(""), this->ui->text_action->text());
    this->ui->text_action->setText(text);
    this->main_window->last_trigger->setFocus();
}

void action::on_edit_action_clicked()
{
    emit this->my_edit_action_signal();
}
