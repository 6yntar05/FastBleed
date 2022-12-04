#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "action.h"
#include "trigger.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->root_triggers.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->scroll_triggers->setWidget(&this->root_triggers);
    ui->scroll_actions->setWidget(&this->empty_widget);

    connect(this, &MainWindow::my_add_action_signal, this, &MainWindow::my_add_action_slot);
    connect(this, &MainWindow::my_remove_action_signal, this, &MainWindow::my_remove_action_slot);
    connect(this, &MainWindow::my_add_trigger_signal, this, &MainWindow::my_add_trigger_slot);
    connect(this, &MainWindow::my_remove_trigger_signal, this, &MainWindow::my_remove_trigger_slot);
}

MainWindow::~MainWindow()
{
    delete ui;
}

Ui::MainWindow *MainWindow::get_ui()
{
    return this->ui;
}

void add_element(QWidget *w, std::vector<QWidget*> &v, QVBoxLayout &l)
{
    v.push_back(w);
    l.addWidget(v.back());
}

void remove_element(std::vector<QWidget*> &v, QVBoxLayout &l)
{
    if (v.empty())
        return;

    l.removeWidget(v.back());
    v.pop_back();
}

void MainWindow::my_add_action_slot()
{
    for (QWidget *t : this->triggers_list)
    {
        trigger *_t = static_cast<trigger*>(t);
        if (_t->in_focus)
        {
            add_element(new action(this), _t->actions_list, _t->actions_layout);
            return;
        }
    }
}

void MainWindow::my_remove_action_slot()
{
    for (QWidget *t : this->triggers_list)
    {
        trigger *_t = static_cast<trigger*>(t);
        if (_t->in_focus)
        {
            remove_element(_t->actions_list, _t->actions_layout);
            return;
        }
    }
}

void MainWindow::on_add_action_clicked()
{
    emit this->my_add_action_signal();
}

void MainWindow::on_remove_action_clicked()
{
    emit this->my_remove_action_signal();
}

void MainWindow::my_add_trigger_slot()
{
    add_element(new trigger(this->triggers_list.size(), this), this->triggers_list, this->triggers_layout);
    this->ui->scroll_actions->takeWidget();
    this->ui->scroll_actions->setWidget(&this->empty_widget);
    for (QWidget *t : this->triggers_list)
    {
        trigger *_t = static_cast<trigger*>(t);
        if (_t->in_focus)
        {
            _t->clearFocus();
            break;
        }
    }
    this->triggers_list.back()->setFocus();
}

void MainWindow::my_remove_trigger_slot()
{
    remove_element(this->triggers_list, this->triggers_layout);
    this->ui->scroll_actions->takeWidget();
    this->ui->scroll_actions->setWidget(&this->empty_widget);
    for (QWidget *t : this->triggers_list)
    {
        trigger *_t = static_cast<trigger*>(t);
        if (_t->in_focus)
        {
            _t->clearFocus();
            return;
        }
    }
}

void MainWindow::on_add_trigger_clicked()
{
    emit this->my_add_trigger_signal();
}

void MainWindow::on_remove_trigger_clicked()
{
    emit this->my_remove_trigger_signal();
}

void trigger::focusInEvent(QFocusEvent *e)
{
    if (this->in_focus)
        return;
        
    this->in_focus = true;
    this->frame->setFrameShadow(QFrame::Sunken);
    this->main_window->get_ui()->scroll_actions->takeWidget();
    this->main_window->get_ui()->scroll_actions->setWidget(&this->root_actions);
}

void trigger::focusOutEvent(QFocusEvent *e)
{
    this->in_focus = false;
    this->frame->setFrameShadow(QFrame::Plain);
    this->main_window->last_trigger = this;
}