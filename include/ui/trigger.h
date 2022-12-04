#ifndef TRIGGER_H
#define TRIGGER_H

#include <QWidget>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QFrame>
#include "mainwindow.h"

namespace Ui {
class trigger;
}

class trigger : public QWidget
{
    Q_OBJECT

public:
    explicit trigger(int index, MainWindow *main_window, QWidget *parent = nullptr);
    ~trigger();

    QWidget root_actions;
    QVBoxLayout actions_layout{&root_actions};
    std::vector<QWidget*> actions_list;
    MainWindow *main_window;
    QFrame *frame = nullptr;

    bool in_focus = false;

private:
    Ui::trigger *ui;

private slots:
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);
};

#endif // TRIGGER_H
