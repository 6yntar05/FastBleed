#include "ui/gui.hpp"
//#include <QApplication>
//#include <qapplication.h>
//#include <QMainWindow>
#include <iostream>
//#include <qmainwindow.h>

namespace ui {

/*
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    MainWindow *ui;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new MainWindow) {
    ui->setupUi();
}

MainWindow::~MainWindow()
{
    delete ui;
}
*/

gui_feedback::gui_feedback(int argc, char* argv[]) {
    std::cerr << "CALL "<<argc<<":"<<argv[0]<<"\n";
    //QApplication qapp(argc, argv);
    //MainWindow mwin;
    //mwin.show();
    //qapp.exec();
}

void gui_feedback::popup() {
    //
}

} // namespace ui