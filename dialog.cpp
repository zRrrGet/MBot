#include "dialog.h"
#include "ui_dialog.h"
#include <QNetworkAccessManager>
#include <QFuture>

Dialog::Dialog(QWidget *parent, QString imgurl) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    imgurl = "https://api.vk.com/captcha.php?sid="+imgurl+"&s=1";
    ui->setupUi(this);
    QNetworkAccessManager *manager = new QNetworkAccessManager;
    connect(manager,&QNetworkAccessManager::finished,this,&Dialog::replyfinish);
    manager->get(QNetworkRequest(QUrl(imgurl)));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::replyfinish(QNetworkReply *reply) {
    QPixmap pm;
    pm.loadFromData(reply->readAll());
    ui->CaptchaImage->setPixmap(pm);
}

void Dialog::on_pushButton_clicked()
{
    if (ui->CaptchaInput->toPlainText() == ""){
        ui->CaptchaInput->setFocus();
    }
    else {
        response = ui->CaptchaInput->toPlainText();
        this->close();
    }
}

void Dialog::on_cancel_clicked()
{
    this->done(1);
}
