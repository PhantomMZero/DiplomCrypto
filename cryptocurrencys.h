#ifndef CRYPTOCURRENCYS_H
#define CRYPTOCURRENCYS_H

#include <QDialog>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariantMap>
#include <QList>
#include <QCloseEvent>

namespace Ui {
class Cryptocurrencys;
}

class Cryptocurrencys : public QDialog
{
    Q_OBJECT



public:
    explicit Cryptocurrencys(QWidget *parent = nullptr);
    QString name;
    double price;
    bool reject;
    ~Cryptocurrencys();

private slots:
    void on_buttonBox_accepted();


    void on_lineEdit_textChanged(const QString &arg1);

    void on_buttonBox_rejected();

private:
    Ui::Cryptocurrencys *ui;

};

#endif // CRYPTOCURRENCYS_H
