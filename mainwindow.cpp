#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "windows.h"
#include "dialog.h"
#include <QtWebKitWidgets/QWebView>
#include <QMessageBox>
#include <QUrlQuery>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFutureWatcher>
#include <ctime>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    on_off(false);
    ui->statusBar->showMessage("Ожидание входа...");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_LoginButton_clicked()
{
    log = new QWebView;
    log->setWindowTitle("Вход");
    log->setWindowIcon(QIcon(icon));
    connect(log,&QWebView::urlChanged,this,&MainWindow::urlchange);
    log->load(QUrl("https://oauth.vk.com/authorize?client_id=2685278&display=page&redirect_uri=https://oauth.vk.com/blank.html&scope=messages,offline&response_type=token&v=5.95&state=123456"));
    log->setFixedSize(log->page()->viewportSize());
    log->show();
}

void MainWindow::on_off(bool set) {
    ui->Delay->setEnabled(set);
    ui->MId->setEnabled(set);
    ui->StartButton->setEnabled(set);
    ui->Message->setEnabled(set);
    ui->attachments->setEnabled(set);
    ui->randomize->setEnabled(set);
}

void MainWindow::startOff(bool set) {
    ui->Delay->setEnabled(set);
    ui->MId->setEnabled(set);
    ui->Message->setEnabled(set);
    ui->attachments->setEnabled(set);
    ui->randomize->setEnabled(set);
    ui->LoginButton->setEnabled(set);
}

void MainWindow::urlchange(QUrl url) {
    QUrlQuery query(QUrl(url.toString(QUrl::None).replace('#','?')));
    if (query.hasQueryItem("access_token")) {
        token = query.queryItemValue("access_token");
        QMessageBox *test = new QMessageBox;
        test->setWindowTitle("Авторизация");
        test->setWindowIcon(QIcon(icon));
        if (token.size()>10) {
            test->setText("Авторизация прошла успешно");
            ui->statusBar->showMessage("Вход выполнен");
            ui->LoginButton->setText("Войти под другим аккаунтом");
            on_off(true);
        }
        else test->setText("Что-то пошло не так, напиши программисту");
        //ui->MId->setPlainText(token);
        test->exec();
        log->deleteLater();
    }
}

void MainWindow::g()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager;
    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::fin);
    QEventLoop loop;

    connect(manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);


    QString message = ui->Message->toPlainText();
    while (!future.isCanceled()) {
        if (!future.isPaused()) {
            if (isMassive && !GroupIds->isEmpty()) {
                peerid = GroupIds->last();
                GroupIds->erase(GroupIds->end());
            }
            else if (isMassive && iter!=0){
                offset+=1000;
//                QNetworkAccessManager *getnewids = new QNetworkAccessManager();
//                connect(getnewids,&QNetworkAccessManager::finished,this,&MainWindow::getMembers);
//                getnewids->get(QNetworkRequest(QUrl(
//                                                 "https://api.vk.com/method/groups.getMembers?group_id="+
//                                                 public_id+"&offset="+QString::number(offset)+"&count=1000&filter=can_write_private_message&v=5.101&access_token="+token)));
//                while (GroupIds->isEmpty()) Sleep(1000);
            }
            else if (isMassive){
                on_StartButton_clicked();
            }
                QByteArray data = "";
                //data.append("access_token="+token.toLocal8Bit()+"&");
                data.append("access_token="+token+"&");
                data.append("v=5.95&");
                data.append("attachment="+ui->attachments->toPlainText());
                srand(time(0));
                int random = rand();
                if (oldsetter.contains("method/messages.send") || isMassive) {
                    data.append("&peer_id="+peerid.toLocal8Bit());
                    data.append("&random_id=");
                    data.append(QString::number(random%2000000000));
                }
                else {
                    if (peerid.contains("_", Qt::CaseInsensitive) ) {
                        data.append("&owner_id=" + peerid.split("_")[0]);
                        data.append("&post_id=" + peerid.split("_")[1]);
                    }
                }

                data.append("&message=");
                if (ui->randomize->isChecked())
                    for (int i = 0;i<random%10;++i)
                        message.append(ui->Message->toPlainText());
                data.append(message);
                message = ui->Message->toPlainText();
                if (!future.isPaused()) manager->post(QNetworkRequest(QUrl(setter)),data);
                setter = oldsetter;
                captchasid = "";
                loop.exec();
                Sleep(ui->Delay->value());

        }
    }
    manager->deleteLater();

}


void MainWindow::on_StartButton_clicked()
{
    if (ui->StartButton->text() == startSign) {
        ui->statusBar->showMessage("Запуск потока");

        if (ui->Message->toPlainText().isEmpty()) {
            ui->statusBar->showMessage("Ошибка! Сообщение пустое");
            return;
        }
        QUrlQuery q(QUrl(ui->MId->toPlainText()));
        QString id = "";
        if (q.hasQueryItem("sel")) {
            id = q.queryItemValue("sel");
            setter = "https://api.vk.com/method/messages.send";
            oldsetter = "https://api.vk.com/method/messages.send";
        }
        else if (q.hasQueryItem("w")){
            id = q.queryItemValue("w");
            id.remove(0,4);
            if (ui->Delay->value() < 2000)
                ui->Delay->setValue(2000);

            setter = "https://api.vk.com/method/wall.createComment";
            oldsetter = "https://api.vk.com/method/wall.createComment";
        }
        else if (ui->MId->toPlainText().contains("vk.com/") && !ui->MId->toPlainText().contains("?")) {
            isMassive = true;
            public_id = QUrl(ui->MId->toPlainText().contains("https://")?ui->MId->toPlainText():ui->MId->toPlainText().insert(0,"https://")).path().remove(0,1);

            QNetworkAccessManager *manager = new QNetworkAccessManager();
            connect(manager,&QNetworkAccessManager::finished,this,&MainWindow::getMembers);
            manager->get(QNetworkRequest(QUrl(
                                             "https://api.vk.com/method/groups.getMembers?group_id="+
                                             public_id+"&offset="+QString::number(offset)+"&count=1000&filter=can_write_private_message&v=5.101&access_token="+token)));
        }
        else {
            ui->statusBar->showMessage("Ошибка! Неверная ссылка");
            return;
        }
        startOff(false);
        ui->StartButton->setText(stopSign);
        peerid = id;
        if (oldsetter.contains("method/messages.send")) {

            if (peerid.startsWith("c")) {
                peerid.remove("c");
                int numb = peerid.toInt() + 2000000000;
                peerid = QString::number(numb);
            }
        }
        future = QtConcurrent::run(this,&MainWindow::g);
    }
    else {
        isMassive = false;
        ui->StartButton->setText(startSign);
        ui->statusBar->showMessage("Остановка");
        startOff(true);
        future.cancel();
        cnt = 0;
    }
}


void MainWindow::fin(QNetworkReply *reply) {
    QByteArray a = reply->readAll();
    int b = a.indexOf("Captcha needed");
    qDebug() << a;
    if (b == 39 && !future.isPaused()) {
        ui->statusBar->showMessage("Капча! (" + QString::number(tries+1) + ")");
        if (tries==2) {
            tries = 0;
            future.setPaused(true);

            QString data = a.constData();
            int c = data.indexOf("captcha_sid",170);
            captchasid = QString::fromStdString(data.toStdString().substr(c+14,12));
            qDebug() << captchasid;
            if (ui->StartButton->text() == stopSign && setter.length()==oldsetter.length()) {
                cap = new Dialog(this, captchasid);
                cap->setWindowTitle("Введите капчу");
                cap->setWindowIcon(QIcon(icon));
                connect(cap,&QDialog::finished,this,&MainWindow::dialogClosed);
                cap->setModal(true);
               if (ui->StartButton->text() == stopSign && setter.length()==oldsetter.length()) cap->show();
            }
        }
        else ++tries;
    }
    else if (b != 39){
        ++cnt;
        ui->statusBar->showMessage("Отправлено!(" + QString::number(cnt) + ")");
    }
    else {
        ui->statusBar->showMessage("Ошибка! Сообщение слишком большое либо вы не имеете доступа для отправки сообщения");
    }
    reply->deleteLater();
}

void MainWindow::dialogClosed(int result) {
    if (result == 1){
        if (ui->StartButton->text() == stopSign)
        {
            cap->deleteLater();
            on_StartButton_clicked();
            ui->statusBar->showMessage("Остановка");
        }
    }
    else {
        setter=setter+"?captcha_sid="+captchasid+"&captcha_key="+cap->response;
        future.resume();
        cap->deleteLater();
    }
}


void MainWindow::on_MId_textChanged()
{
    if (ui->MId->toPlainText().contains("/im?")) ui->label->setText("Ссылка (диалог):");
    else if (ui->MId->toPlainText().contains("?w=wall")) ui->label->setText("Ссылка (пост):");
    else ui->label->setText("Ссылка (некорректная ссылка):");
}

void MainWindow::getMembers(QNetworkReply *reply) {
    QByteArray rep = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(rep);
    QJsonObject obj = doc.object();
    QJsonArray val = obj.value("response")["items"].toArray();
    for (int i = 0; i < val.size(); ++i) {
        GroupIds->push_back(val.at(i).toString());
    }
    if (iter==0)iter = obj.value("response")["count"].toInt();
    generalAmount = val.size();
    iter = iter-generalAmount;

    qDebug() << GroupIds->size();

}

