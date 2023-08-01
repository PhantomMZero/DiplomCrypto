#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cryptocurrencys.h"
#include "charts.h"
#include "diagram.h"
#include "about.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
void PnLSumTaxAndComission(Ui::MainWindow* ui);
    ui->setupUi(this);
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(8);
    ui->tableWidget->setHorizontalHeaderLabels({"Crypto Name", "Quantity", "Buy", "Sell",
                                            "Current Price", "Broker Comission", "Tax", "PnL"});
    QSqlQuery query;
           query.exec("SELECT RowID, * FROM deals"); // Replace with your table name
           int row = 0;
           while (query.next())
           {
               // Extract data from the query
               QString cryptoName = query.value("CryptoName").toString();
               double quantity = query.value("Quantity").toDouble();
               double Buy = query.value("Buy").toDouble();
               double Sell = query.value("Sell").toDouble();
               double currentPrice = query.value("CurrentPrice").toDouble();
               double brokerCommission = query.value("BrokerComission").toDouble();
               double tax = query.value("Tax").toDouble();
               double pnl = query.value("PnL").toDouble();

               // Insert data into the table
               ui->tableWidget->insertRow(row);
               ui->tableWidget->setItem(row, 0, new QTableWidgetItem(cryptoName));
               ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(quantity)));
               ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(Buy)));
               ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(Sell)));
               ui->tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(currentPrice)));
               ui->tableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(brokerCommission)));
               ui->tableWidget->setItem(row, 6, new QTableWidgetItem(QString::number(tax)));
               ui->tableWidget->setItem(row, 7, new QTableWidgetItem(QString::number(pnl)));

               row++;
           }
           ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
           PnLSumTaxAndComission(ui);

}


void PnLSumTaxAndComission(Ui::MainWindow* ui){

   double allPnL=0.0;
   double BuyRow=0.0;
   double SellRow=0.0;
   double quantity =0.0;
   double sum=0.0;
   double BuyCommission=0.0;
   double SellCommission=0.0;
   double Tax=0;


   for (int row = 0; row < ui->tableWidget->rowCount(); row++) {


    quantity= ui->tableWidget->item(row, 1)->text().toDouble();
    BuyRow = ui->tableWidget->item(row, 2)->text().toDouble();
    SellRow = ui->tableWidget->item(row, 3)->text().toDouble();
    BuyCommission=BuyRow*0.001;
    SellCommission=SellRow*0.001;
    sum=(SellRow * quantity - BuyRow * quantity);
    Tax=(sum)*0.13;
    if (Tax<0){
     Tax=0;
    }
    if (quantity!=0&&BuyRow!=0){
        allPnL+=sum-BuyCommission-SellCommission-Tax;
        ui->tableWidget->setItem(row, 7, new QTableWidgetItem(QString::number(sum-BuyCommission-SellCommission-Tax)));
        if (SellRow!=0){
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(BuyCommission+SellCommission)));
        ui->tableWidget->setItem(row, 6, new QTableWidgetItem(QString::number(Tax)));
        }
        if (SellRow==0){
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(BuyCommission)));
        ui->tableWidget->setItem(row, 6, new QTableWidgetItem(QString::number(0)));
    }
    }
    else {
    ui->tableWidget->setItem(row, 7, new QTableWidgetItem(QString::number(0)));
    ui->tableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(0)));
    ui->tableWidget->setItem(row, 6, new QTableWidgetItem(QString::number(0)));
    }

   }
QString labelText = QString::number(allPnL) + " $";
ui->allPnL->setText(labelText);

}


void MainWindow::on_AddNewDealButton_clicked()
{


    Cryptocurrencys* c = new Cryptocurrencys();  // Create a new instance of the window on the heap
    c->show();
    while(c->isVisible()){    // Wait for the window to close before getting input from the user
           qApp->processEvents();
       }
    QString cryptoname=c->name;
    double price=c->price;
    if (c->reject){
        delete c;
        return;
    }
    delete c;    // Delete the window when it is no longer needed

    // Display input dialogs for each field
    double quantity = QInputDialog::getDouble(this, "Enter Quantity", "Quantity:");
    if (quantity == 0.0) {
           return; // User cancelled the dialog, exit the function
       }
    double Buy = QInputDialog::getDouble(this, "Enter Buy Price", "Buy Price:");
    if (Buy == 0.0) {
            return; // User cancelled the dialog, exit the function
        }
    double Sell = QInputDialog::getDouble(this, "Enter Sell Price", "Sell Price:");


    // Add a new row to the table widget
    int rowCount = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rowCount);

    QTableWidgetItem *cryptoNameItem = new QTableWidgetItem(cryptoname);
    QTableWidgetItem *quantityItem = new QTableWidgetItem(QString::number(quantity));
    QTableWidgetItem *BuyItem = new QTableWidgetItem(QString::number(Buy));
    QTableWidgetItem *SellItem = new QTableWidgetItem(QString::number(Sell));
    QTableWidgetItem *currentPriceItem = new QTableWidgetItem(QString::number(price));
    ui->tableWidget->setItem(rowCount, 0, cryptoNameItem);
    ui->tableWidget->setItem(rowCount, 1, quantityItem);
    ui->tableWidget->setItem(rowCount, 2, BuyItem);
    ui->tableWidget->setItem(rowCount, 3, SellItem);
    ui->tableWidget->setItem(rowCount, 4, currentPriceItem);

  PnLSumTaxAndComission(ui);


    // Insert the values into the database
    QSqlQuery query1;
    query1.prepare("INSERT INTO deals (CryptoName, Quantity, Buy, Sell, CurrentPrice, BrokerComission, Tax, PnL) "
                     "VALUES ( ?, ?, ?, ?, ?, ?, ?, ?)");
    query1.addBindValue(cryptoname);
    query1.addBindValue(quantity);
    query1.addBindValue(Buy);
    query1.addBindValue(Sell);
    query1.addBindValue(price);
    query1.addBindValue(0);
    query1.addBindValue(0);
    query1.addBindValue(0);
    if (query1.exec()) {
        qDebug() << "Row added to the database.";
    } else {
        qDebug() << "Failed to add row to the database:" << query1.lastError().text();
    }
}


void MainWindow::on_EditDealsButton_clicked()
{
       // Check the current edit triggers of the table
           QAbstractItemView::EditTriggers triggers = ui->tableWidget->editTriggers();

           // Toggle between locking and unlocking the table
           if (triggers == QAbstractItemView::NoEditTriggers) {
             ui->tableWidget->setEditTriggers(QAbstractItemView::AllEditTriggers);
             ui->EditDealsButton->setText("Заблокировать");
           }
           else {
             ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
             ui->EditDealsButton->setText("Редактировать");
           }
}


void MainWindow::on_DeleteDeal_clicked()
{
    QList<QTableWidgetItem*> selectedItems = ui->tableWidget->selectedItems();

    // Iterate over the selected items and remove the rows they belong to
    for(QTableWidgetItem* item : selectedItems) {
        int row = item->row();
        rowsToRemove.insert(row);


    }
    for(int row : qAsConst(rowsToRemove)) {
      ui->tableWidget->removeRow(row);
    }
}


void MainWindow::on_showChart_clicked()
{

    QList<QTableWidgetItem*> selectedItems = ui->tableWidget->selectedItems();
       if (selectedItems.isEmpty()) {
           // No item selected, show an error message or return
           return;
       }

       int selectedRow = selectedItems[0]->row();
       QTableWidgetItem* BuyItem = ui->tableWidget->item(selectedRow, 2);
          QTableWidgetItem* SellItem = ui->tableWidget->item(selectedRow, 3);

          if (!BuyItem || !SellItem) {
              // The required items are not present, return
              return;
          }

          double Buy = BuyItem->text().toDouble();
          double Sell = SellItem->text().toDouble();
          QString name = ui->tableWidget->item(selectedRow, 0)->text();

       // Set up the API endpoint and query parameters
       QUrl apiUrl("https://api.coingecko.com/api/v3/coins/markets");
       QUrlQuery query;
       query.addQueryItem("vs_currency", "usd");
       query.addQueryItem("order", "market_cap_desc");
       query.addQueryItem("page", "1");
       query.addQueryItem("sparkline", "false");
       apiUrl.setQuery(query);

       // Create a network request and send an HTTP GET request to the API
       QNetworkRequest request(apiUrl);
       QNetworkAccessManager manager;
       QNetworkReply* reply = manager.get(request);

       // Wait for the network reply to finish
       QEventLoop loop;
       connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
       loop.exec();

       if (reply->error() != QNetworkReply::NoError) {
           qWarning() << "Error fetching data:" << reply->errorString();
           reply->deleteLater();
           return;
       }

       // Parse the JSON response and extract the ID of the cryptocurrency
       QByteArray data = reply->readAll();
       QJsonDocument doc = QJsonDocument::fromJson(data);
       QJsonArray coins = doc.array();
       QString id;
       for (const auto& coin : coins) {
           QJsonObject coinObj = coin.toObject();
           QString coinName = coinObj.value("name").toString();
           if (coinName == name) {
               id = coinObj.value("id").toString();
               break;
           }
       }

       reply->deleteLater();

       if (!id.isEmpty()) {
           // Show the chart for the selected cryptocurrency
           Charts* charts = new Charts(id,name, Buy, Sell);
           charts->show();
           while(charts->isVisible()){
               qApp->processEvents();
           }
           delete charts;
       } else {
           // Could not find the ID for the selected cryptocurrency, show an error message or return
       }

}


void UpdateData(Ui::MainWindow* ui, QSet<int> rowsToRemove) {
    int rowCount = ui->tableWidget->rowCount();
    PnLSumTaxAndComission(ui);
    for (int row = 0; row < rowCount; ++row) {
        QString cryptoName = ui->tableWidget->item(row, 0)->text();
        double quantity = ui->tableWidget->item(row, 1)->text().toDouble();
        double Buy = ui->tableWidget->item(row, 2)->text().toDouble();
        double Sell = ui->tableWidget->item(row, 3)->text().toDouble();
        double currentPrice = ui->tableWidget->item(row, 4)->text().toDouble();
        double brokerComission = ui->tableWidget->item(row, 5)->text().toDouble();
        double tax = ui->tableWidget->item(row, 6)->text().toDouble();
        double pnl = ui->tableWidget->item(row, 7)->text().toDouble();

        // Update the database with new data for all rows
        QSqlQuery query;
        query.prepare("UPDATE deals SET CryptoName = ?, Quantity = ?, Buy = ?, Sell = ?, "
                      "CurrentPrice = ?, BrokerComission = ?, Tax = ?, PnL = ? WHERE RowID = ?");
        query.addBindValue(cryptoName);
        query.addBindValue(quantity);
        query.addBindValue(Buy);
        query.addBindValue(Sell);
        query.addBindValue(currentPrice);
        query.addBindValue(brokerComission);
        query.addBindValue(tax);
        query.addBindValue(pnl);
        query.addBindValue(row + 1); // Assuming RowID starts from 1 in the database

        if (!query.exec()) {
            // Handle query execution error
            qDebug() << query.lastError().text();
        }

    }
    if (!rowsToRemove.isEmpty())
    for (auto it = rowsToRemove.begin(); it != rowsToRemove.end(); ++it) {
        int row = *it;
        // Do something with the integer value

    QSqlQuery query;
    query.prepare("DELETE FROM deals WHERE RowID = ?");
    query.addBindValue(row + 1); // Assuming RowID starts from 1 in the database

    if (!query.exec()) {
    // Handle query execution error
    qDebug() << query.lastError().text();
    }
    QSqlQuery updateSeqQuery;
       updateSeqQuery.exec("UPDATE SQLITE_SEQUENCE SET SEQ = (SELECT MAX(RowID) FROM deals) WHERE NAME = 'deals';");
}
    QSqlQuery query("SELECT * FROM deals ORDER BY RowID");

    // Update the RowID values for each row
    int newID = 1;
    while (query.next()) {
        int oldID = query.value("RowID").toInt();
        if (oldID != newID) {
            // Update the RowID value for the current row
            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE deals SET RowID = ? WHERE RowID = ?");
            updateQuery.addBindValue(newID);
            updateQuery.addBindValue(oldID);
            if (!updateQuery.exec()) {
                // Handle query execution error
                qDebug() << updateQuery.lastError().text();
            }
        }
        newID++;
    }



}




void MainWindow::closeEvent(QCloseEvent *event) {
    // Ask the user if they want to save their data before closing
    int reply = QMessageBox::question(this, tr("Save data?"), tr("Do you want to save your data before closing?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    if (reply == QMessageBox::Cancel) {
        // If the user cancels, ignore the close event and return
        event->ignore();
        return;
    } else if (reply == QMessageBox::Yes) {
           UpdateData(ui, rowsToRemove);
       }

    // Call the default close event to close the application
    QMainWindow::closeEvent(event);
}


void MainWindow::on_updatePrice_clicked()
{
    QUrl url("https://api.coingecko.com/api/v3/coins/markets?vs_currency=usd&order=market_cap_desc&page=1&sparkline=false");
        QNetworkRequest request(url);
        QNetworkAccessManager manager;
        QNetworkReply* reply = manager.get(request);

        QEventLoop loop;
        connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();

        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QJsonArray array = document.array();

        int rowCount = ui->tableWidget->rowCount();

        for (const QJsonValue& value: array) {
            QString cryptoName = value.toObject().value("name").toString();
            double price = value.toObject().value("current_price").toDouble();

            for (int row = 0; row < rowCount; ++row) {
                       QString itemName = ui->tableWidget->item(row, 0)->text();
                       if (itemName == cryptoName) {
                           QTableWidgetItem* item = ui->tableWidget->item(row, 4);
                           if (item) {
                               item->setText(QString::number(price, 'f', 2));
                           }
                           break;
                       }
                   }


            QSqlQuery query;
            query.prepare("UPDATE deals SET CurrentPrice = ? WHERE CryptoName = ?");
            query.addBindValue(price);
            query.addBindValue(cryptoName);

            if (!query.exec()) {
                // Handle query execution error
                qDebug() << query.lastError().text();
            }

            query.clear();
        }
    PnLSumTaxAndComission(ui);

}


void MainWindow::on_tableWidget_cellDoubleClicked(int row, int column)
{
    Cryptocurrencys* c = new Cryptocurrencys();  // Create a new instance of the window on the heap
    c->show();
    while(c->isVisible()){    // Wait for the window to close before getting input from the user
           qApp->processEvents();
       }

    QString cryptoname=c->name;
    double price=c->price;
    if (c->reject){
        delete c;
        return;
    }
    delete c;

    if (row < 0 || row >= ui->tableWidget->rowCount())  // Check if a valid row is double-clicked
           return;

       // Get the two items in the row
       QTableWidgetItem *item1 = ui->tableWidget->item(row, 0);
       QTableWidgetItem *item2 = ui->tableWidget->item(row, 4);

       // Modify the values of the items
       item1->setText(cryptoname);
       item2->setText(QString::number(price));
}



void MainWindow::on_action_triggered()
{
     UpdateData(ui, rowsToRemove);
}

void MainWindow::on_AboutProgram_triggered()
{
   About* about = new About();
    about->show();
    while(about->isVisible()){
        qApp->processEvents();
    }
    delete about;
}



void MainWindow::on_assetsChart_triggered(){

    QMap<QString, double> cryptoAssetsMap;
    int rowCount = ui->tableWidget->rowCount();
    for (int row = 0; row < rowCount; ++row) {
    QString cryptoName = ui->tableWidget->item(row, 0)->text();
    double assets=(ui->tableWidget->item(row,1)->text().toDouble())*(ui->tableWidget->item(row, 4)->text().toDouble());
    if (cryptoAssetsMap.contains(cryptoName)) {
           double currentAssets = cryptoAssetsMap.value(cryptoName);
           cryptoAssetsMap.insert(cryptoName, currentAssets + assets);
       }
       else {
           cryptoAssetsMap.insert(cryptoName, assets);
       }
    }
    Diagram* diagram = new Diagram(cryptoAssetsMap);
    diagram->show();
    while(diagram->isVisible()){
        qApp->processEvents();
    }
    delete diagram;


}


    MainWindow::~MainWindow()
  {

      QSqlDatabase db = QSqlDatabase::database();

      delete ui;
  }























