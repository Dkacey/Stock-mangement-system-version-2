#include "mainwindow.h"
#include "stockfactory.h"
#include "stocklist.h"
#include "datadialog.h"
#include "xmlwriter.h"
#include <QMessageBox>
#include <QStatusBar>
#include <QMenuBar>
#include <QDialog>
#include <QInputDialog>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QSpinBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent},
      stockAddAction{new QAction("&Add stock item", this)},
      listConfectioneryAction{new QAction("List &confectionery items", this)},
      listReadingMaterialAction{new QAction("List &reading material items", this)},
      writeXmlAction{new QAction("&Write to XML", this)},
      displayfield{new QPlainTextEdit}
{
    setupGui();

    stockList = StockList::getInstance();
}

MainWindow::~MainWindow()
{
    stockList->deleteInstance();
}

void MainWindow::addStock()
{
    DataDialog *dd{new DataDialog(this)};
    connect(dd, &DataDialog::dataEntered, this, &MainWindow::addStockData);

    if (dd->exec() == QDialog::Accepted)
    {
        statusBar()->showMessage("Data entered");
    }
    dd->deleteLater();
}

void MainWindow::addStockData(QStringList list)
{
    StockFactory sf;
    Stock *stock{sf.createStock(list.at(0), list.at(1), list.at(2).toInt(), list.at(3))};

    if (stock != nullptr)
    {
        displayfield->appendPlainText(stock->toString());
        stockList->addStock(stock);
    }
}

void MainWindow::listConfectionery()
{
    displayfield->clear();
    displayfield->appendPlainText("Confectionery items:");

    QList<Stock*> cList = stockList->returnList("Confectionery");
    foreach (Stock *s, cList)
        displayfield->appendPlainText(s->toString());
}

void MainWindow::listReadingMaterial()
{
    displayfield->clear();
    displayfield->appendPlainText("Reading material items:");

    QList<Stock*> rmList = stockList->returnList("ReadingMaterial");
    foreach (Stock *s, rmList)
        displayfield->appendPlainText(s->toString());

}

void MainWindow::writeXml()
{
    XMLWriter writer("myStock.xml");
    QList<Stock*> list = stockList->returnList("Confectionery");
    QList<Stock*> nextList = stockList->returnList("ReadingMaterial");
    list.append(nextList);
    if (list.size() > 0)
        writer.write(list);

    statusBar()->showMessage("XML writing complete");
}

void MainWindow::setupGui()
{
    setCentralWidget(displayfield);

    QMenu *stockMenu{menuBar()->addMenu("&Stock")};
    stockMenu->addAction(stockAddAction);
    stockAddAction->setStatusTip("Add a new stock item");
    connect(stockAddAction, &QAction::triggered, this, &MainWindow::addStock);

    QMenu *listMenu{menuBar()->addMenu("&List")};
    listMenu->addAction(listConfectioneryAction);
    listConfectioneryAction->setStatusTip("List all confectionery items");
    connect(listConfectioneryAction, &QAction::triggered, this, &MainWindow::listConfectionery);
    listMenu->addAction(listReadingMaterialAction);
    listReadingMaterialAction->setStatusTip("List all reading material items");
    connect(listReadingMaterialAction, &QAction::triggered, this, &MainWindow::listReadingMaterial);

    QMenu *XMLMenu{menuBar()->addMenu("&XML")};
    XMLMenu->addAction(writeXmlAction);
    writeXmlAction->setStatusTip("Write list to XML");
    connect(writeXmlAction, &QAction::triggered, this, &MainWindow::writeXml);

    statusBar()->showMessage("Ready");
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    int answer{QMessageBox::warning(this, "Exit", "Are you sure you want to exit?", QMessageBox::Yes | QMessageBox::No)};
    if (answer == QMessageBox::Yes)
        event->accept();
    else
        event->ignore();
}
