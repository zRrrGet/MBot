#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("MBot");
    w.setWindowIcon(QIcon(":/icon.ico"));
    w.show();

    return a.exec();
}
