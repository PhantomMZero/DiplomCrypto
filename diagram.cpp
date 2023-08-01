#include "diagram.h"
#include "ui_diagram.h"

Diagram::Diagram(QMap<QString, double> cryptoAssetsMap, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Diagram)
{
    ui->setupUi(this);

    QPieSeries *series = new QPieSeries();

       // Add the crypto assets to the pie series
       QMapIterator<QString, double> i(cryptoAssetsMap);
       while (i.hasNext()) {
           i.next();

           series->append(i.key(), i.value());
       }
       for (QPieSlice *slice : series->slices()) {
             QString sliceName = slice->label();
             slice->setLabel(QString("%1 %2%").arg(sliceName).arg(100*slice->percentage(), 0, 'f', 1));
          }

       series->setLabelsVisible(true);
       series->setLabelsPosition(QPieSlice::LabelInsideHorizontal);


       QChart *diagram= new QChart();
       diagram->addSeries(series);
       diagram->setTitle("Crypto Assets");
       // Create a new chart view and add the pie series to it
       QChartView *chartView = new QChartView(diagram);
       chartView->setRenderHint(QPainter::Antialiasing);
       // Set the chart view as the main widget for the Diagram class
       QVBoxLayout *layout = new QVBoxLayout;
       layout->addWidget(chartView);
       setLayout(layout);


}

Diagram::~Diagram()
{
    delete ui;
}
