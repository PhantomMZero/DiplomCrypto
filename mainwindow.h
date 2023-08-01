#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QInputDialog>
#include <QMessageBox>
#include <QCoreApplication>
#include <QCloseEvent>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    void closeEvent(QCloseEvent *event) override;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_AddNewDealButton_clicked();

    void on_EditDealsButton_clicked();

    void on_action_triggered();

    void on_assetsChart_triggered();

    void on_DeleteDeal_clicked();


    void on_updatePrice_clicked();

    void on_tableWidget_cellDoubleClicked(int row, int column);

    void on_showChart_clicked();

    void on_AboutProgram_triggered();


private:
    QSet<int> rowsToRemove;
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
