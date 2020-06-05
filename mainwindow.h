#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QEventLoop>
#include <QFuture>
#include <QFutureWatcher>
#include <QMainWindow>
#include <QtWebKitWidgets/QWebView>
#include "dialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QWebView *log;
    QString token = "ad41d1e48a04f41fe1c8afdd80d5c269f5536914a103d55a7d3b9d53613d0d453bb56341c329492dfd29f"; // already invalid
    QString icon = ":/icon.ico";
    int iter = 0;
    QString public_id = "";
    bool isMassive = false;
    int cnt = 0;
    int generalAmount = 0;
    int offset = 0;
    QFuture<void> future;
    QList<QString> *GroupIds = new QList<QString>;
    Dialog *cap;
    QString setter = "https://api.vk.com/method/messages.send";
    QString oldsetter = "https://api.vk.com/method/messages.send";
    QString peerid;
    QString captchasid = "";
    int tries = 0;
    QString startSign = "Start";
    QString stopSign = "Stop";
private slots:
    void on_LoginButton_clicked();

    void urlchange(QUrl url);
    void on_StartButton_clicked();

    void g();
    void fin(QNetworkReply *reply);
    void on_off(bool set);

    void dialogClosed(int result);


    void startOff(bool set);
    void on_MId_textChanged();

    void getMembers(QNetworkReply *reply);    
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
