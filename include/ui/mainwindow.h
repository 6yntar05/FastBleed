#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QFocusEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QWidget empty_widget;
    QWidget root_triggers;
    QVBoxLayout triggers_layout{&root_triggers};
    std::vector<QWidget*> triggers_list;

    Ui::MainWindow *get_ui();
    QWidget *last_trigger = nullptr;

private:
    Ui::MainWindow *ui;

signals:
    void my_add_action_signal();
    void my_remove_action_signal();
    void my_add_trigger_signal();
    void my_remove_trigger_signal();

private slots:
    void my_add_action_slot();
    void on_add_action_clicked();
    void my_remove_action_slot();
    void on_remove_action_clicked();
    void my_add_trigger_slot();
    void on_add_trigger_clicked();
    void my_remove_trigger_slot();
    void on_remove_trigger_clicked();
};
#endif // MAINWINDOW_H
