#include "charts.h"
#include "ui_charts.h"

Charts::Charts(QString Id, QString Name, double Buy, double Sell, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Charts)
{
    QUrl url(QString("https://api.coingecko.com/api/v3/coins/%1/market_chart?vs_currency=usd&days=365").arg(Id));
    QNetworkRequest request(url);
    QNetworkAccessManager manager;
    QNetworkReply* reply = manager.get(request);

    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject object = document.object();
    QJsonArray pricesArray = object.value("prices").toArray();


    QLineSeries* BuyLine = new QLineSeries();
    BuyLine->setName("Buy");
    QLineSeries* SellLine = new QLineSeries();
    SellLine->setName("Sell");
    QLineSeries* series = new QLineSeries();
    for (const QJsonValue& value: pricesArray) {
        QDateTime timestamp = QDateTime::fromMSecsSinceEpoch(value.toArray()[0].toDouble());
        double price = value.toArray()[1].toDouble();
        BuyLine->append(timestamp.toMSecsSinceEpoch(), Buy);
        SellLine->append(timestamp.toMSecsSinceEpoch(), Sell);
        series->append(timestamp.toMSecsSinceEpoch(), price);
    }





    ui->setupUi(this);


    QChart* chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(BuyLine);
    chart->addSeries(SellLine);
    chart->addSeries(series);
    BuyLine->setColor(Qt::green);
    BuyLine->setPen(QPen(Qt::green, 2));

    SellLine->setColor(Qt::red);
    SellLine->setPen(QPen(Qt::red, 2));

    chart->setTitle(Name);

    QDateTimeAxis* axisX = new QDateTimeAxis;
    axisX->setTickCount(10);
    axisX->setFormat("d MMM yyyy");
    axisX->setTitleText("Date");
    chart->addAxis(axisX, Qt::AlignBottom);
    BuyLine->attachAxis(axisX);
    SellLine->attachAxis(axisX);
    series->attachAxis(axisX);

    QValueAxis* axisY = new QValueAxis;
    axisY->setLabelFormat("$%i");
    axisY->setTitleText("Price (USD)");
    chart->addAxis(axisY, Qt::AlignLeft);
    BuyLine->attachAxis(axisY);
    SellLine->attachAxis(axisY);
    series->attachAxis(axisY);

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setRubberBand(QChartView::HorizontalRubberBand);
    ui->chartLayout->addWidget(chartView); // add the chart to the UI layout
}

Charts::~Charts()
{
    delete ui;
}
