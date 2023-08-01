#ifndef CHARTS_H
#define CHARTS_H

#include <QWidget>
#include <QtCharts>
#include <QChartView>
#include <QLineSeries>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>

namespace Ui {
class Charts;
}

class Charts : public QWidget
{
    Q_OBJECT

public:
    explicit Charts(QString Id, QString Name, double Buy, double Close, QWidget *parent = nullptr);
    ~Charts();

private:
    Ui::Charts *ui;
};

#endif // CHARTS_H
