#include "cryptocurrencys.h"
#include "ui_cryptocurrencys.h"


Cryptocurrencys::Cryptocurrencys(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Cryptocurrencys)
{
    reject=false;
    ui->setupUi(this);
    QUrl url("https://api.coingecko.com/api/v3/coins/markets?vs_currency=usd&order=market_cap_desc&page=1&sparkline=false");
    QNetworkRequest request(url);
    QNetworkAccessManager manager;
    QNetworkReply* reply = manager.get(request);


    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray array = document.array();


   for (const QJsonValue& value: array) {
        name = value.toObject().value("name").toString();
        QString symbol = value.toObject().value("symbol").toString();
        price = value.toObject().value("current_price").toDouble();

        QListWidgetItem* item = new QListWidgetItem(name + " (" + symbol + "): $" + QString::number(price, 'f', 2));
        ui->listWidget->addItem(item);
    }



}

void Cryptocurrencys::on_buttonBox_rejected()
{
    reject=true;
    QDialog::reject();
}


void Cryptocurrencys::on_buttonBox_accepted()
{
    QListWidgetItem *selectedItem = ui->listWidget->currentItem(); // get the currently selected item
       if (selectedItem) // make sure an item is selected
       {
           QString itemText = selectedItem->text(); // get the text of the selected item
           name = itemText.split("(").first().trimmed();
           name.remove(QChar('"'), Qt::CaseInsensitive);
           qDebug("Selected item name: %s", qUtf8Printable(name));
           QRegularExpression re("\\$([0-9]+\\.[0-9]{2})");
               QRegularExpressionMatch match = re.match(itemText);
               if (match.hasMatch()) {
                   price = match.captured(1).toDouble();
                   qDebug("Selected item price: %f", price);
               } else {
                   qDebug("Error: could not extract price from item text.");
               }

       }


}




void Cryptocurrencys::on_lineEdit_textChanged(const QString &searchText)
{
    for (int i = 0; i < ui->listWidget->count(); ++i) {
           QListWidgetItem* item = ui->listWidget->item(i);
           QString itemText = item->text();
           if (itemText.contains(searchText, Qt::CaseInsensitive)) {
               item->setHidden(false);
           } else {
               item->setHidden(true);
           }
    }
}






Cryptocurrencys::~Cryptocurrencys()
{

    delete ui;
}







