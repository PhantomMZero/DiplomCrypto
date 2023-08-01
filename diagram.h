#ifndef DIAGRAM_H
#define DIAGRAM_H

#include <QWidget>
#include <QtCharts>
#include <QChartView>
#include <QPieSeries>
#include <QVBoxLayout>

namespace Ui {
class Diagram;
}

class Diagram : public QWidget
{
    Q_OBJECT

public:
    explicit Diagram(QMap<QString, double> cryptoAssetsMap, QWidget *parent = nullptr);
    ~Diagram();

private:
    Ui::Diagram *ui;
};

#endif // DIAGRAM_H
