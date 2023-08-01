#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString path = QDir(QCoreApplication::applicationDirPath()).filePath("Deals.db");//Задание пути по умолчанию для базы данных
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    // Установка имени базы данных
    db.setDatabaseName(path);

    // Открытие базы данных
    if (!db.open()) {
        qDebug() << "Failed to open database:";
        qDebug() << db.lastError().text();
       }

    // Создание таблицы в базе данных
    QSqlQuery query;
    query.prepare("CREATE TABLE IF NOT EXISTS deals("
                  "RowID INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "CryptoName TEXT, "
                  "Quantity REAL, "
                  "Buy REAL, "
                  "Sell REAL, "
                  "CurrentPrice REAL, "
                  "BrokerComission REAL, "
                  "Tax REAL, "
                  "PnL REAL)");
    if (!query.exec()) {
        qDebug() << "Failed to create table:";
        qDebug() << query.lastError().text();
        db.close();
      }






    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl("https://www.google.com/"));
    QNetworkReply* reply = manager.get(request);

    QObject::connect(reply, &QNetworkReply::finished, [&](){
        if(reply->error() != QNetworkReply::NoError) {
            qDebug() << "Ethernet connection not available!";
            QMessageBox::warning(nullptr, "Ethernet connection not available", "Please connect to Ethernet to use this application.");
            }
        qDebug() << "Ethernet connection available!";
        // start your app here

    });


    MainWindow w;
    w.show();
    int ret = a.exec();
    db.close(); // Close the database before exiting the application
    return ret;
}
