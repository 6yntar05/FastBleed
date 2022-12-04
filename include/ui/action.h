#ifndef ACTION_H
#define ACTION_H

#include <QWidget>
#include <QInputDialog>
#include "mainwindow.h"
#include "trigger.h"

namespace Ui {
class action;
}

class action : public QWidget
{
    Q_OBJECT

public:
    explicit action(MainWindow *main_window, QWidget *parent = nullptr);
    ~action();

    MainWindow *main_window;

private:
    Ui::action *ui;

signals:
    void my_edit_action_signal();

private slots:
    void my_edit_action_slot();
    void on_edit_action_clicked();
};

#endif // ACTION_H
