#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QNetworkReply>
#include <QFuture>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr, QString imgurl =  "");
    QString response;
    ~Dialog();

private slots:
    void on_pushButton_clicked();

    void replyfinish(QNetworkReply *reply);
    void on_cancel_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
