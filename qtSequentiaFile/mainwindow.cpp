#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "SequentialController.h"
#include "Universitario.h"
#include "SequentialFile.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QInputDialog>
#include <QWidget>
#include <fstream>
#include <iostream>
#include <vector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    csvModel = new QStandardItemModel(this);
    csvModel->setColumnCount(3);
    csvModel->setHorizontalHeaderLabels(QStringList()<< "Codigo" << "Nombre" << "Carrera");
    ui->tableView->setModel(csvModel);
    ui->outputCodigo->setText("");
    ui->outputNombre->setText("");
    ui->outputCarrera->setText("");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setValueAt(int ix, int jx, const QString &value)
{
    if(!csvModel->item(ix, jx)){
        csvModel->setItem(ix, jx, new QStandardItem(value));
    } else{
        csvModel->item(ix, jx)->setText(value);
    }
}


void MainWindow::on_pushButton_ACTUALIZAR_clicked(){
    auto result = test.getSortedRecords();

    int ix=0;
    while (ix != (int)result.size()){
        csvModel->setRowCount(ix);
        auto line = result[ix];

        vector<QString> values;

        QString Q_codigo = QString::fromStdString(line.getCodigo());
        QString Q_nombre = QString::fromStdString(line.getNombre());
        QString Q_carrera = QString::fromStdString(line.getCarrera());

        values.push_back(Q_codigo);
        values.push_back(Q_nombre);
        values.push_back(Q_carrera);

        const int colCount = values.size();
        csvModel->setColumnCount(colCount);
        for(int jx=0; jx < colCount;++jx){
            //if(ix==-1){
                //csvModel->setHeaderData(jx, Qt::Horizontal, values.at(jx));
            //}
            setValueAt(ix, jx, values.at(jx));
        }
        ix++;

        for(int i=0; i<colCount;++i){
            values.pop_back();
        }
    }
}

void MainWindow::on_pushButton_INSERT_clicked()
{
    SequentialController sequentialController;
    QString inputCodigo = QInputDialog::getText(this,"CODIGO","Ingrese su codigo: ");
    ui->outputCodigo->setText(inputCodigo);
    QString inputNombre = QInputDialog::getText(this,"NOMBRE","Ingrese su nombre: ");
    ui->outputNombre->setText(inputNombre);
    QString inputCarrera = QInputDialog::getText(this,"CARRERA","Ingrese su carrera: ");
    ui->outputCarrera->setText(inputCarrera);

    // Cambiando QString to string
    std::string utf8_inputCodigo = inputCodigo.toUtf8().constData();
    std::string utf8_inputNombre = inputNombre.toUtf8().constData();
    std::string utf8_inputCarrera = inputCarrera.toUtf8().constData();
    sequentialController.addTest(utf8_inputCodigo, utf8_inputNombre, utf8_inputCarrera);
    MainWindow::on_pushButton_ACTUALIZAR_clicked();
}

void MainWindow::on_pushButton_SEARCH_clicked()
{
    SequentialController sequentialController;
    QString inputNombre = QInputDialog::getText(this,"NOMBRE","Ingrese su nombre: ");
    ui->outputNombre->setText(inputNombre);
    std::string utf8_inputNombre = inputNombre.toUtf8().constData();
    cout << "YEAH" << endl;
    auto result = sequentialController.searchTest(utf8_inputNombre);
    cout << "WORKSSSSS" << endl;

    int ix=0;
    while (ix != (int)result.size()){
        csvModel->setRowCount(ix);
        auto line = result[ix];

        vector<QString> values;

        QString Q_codigo = QString::fromStdString(line.getCodigo());
        QString Q_nombre = QString::fromStdString(line.getNombre());
        QString Q_carrera = QString::fromStdString(line.getCarrera());

        values.push_back(Q_codigo);
        values.push_back(Q_nombre);
        values.push_back(Q_carrera);

        const int colCount = values.size();
        csvModel->setColumnCount(colCount);
        for(int jx=0; jx < colCount;++jx){
            //if(ix==-1){
                //csvModel->setHeaderData(jx, Qt::Horizontal, values.at(jx));
            //}
            setValueAt(ix, jx, values.at(jx));
        }
        ix++;

        for(int i=0; i<colCount;++i){
            values.pop_back();
        }
    }
}

void MainWindow::on_pushButton_DELETE_clicked()
{
    SequentialController sequentialController;
    QString name = QInputDialog::getText(this,"NOMBRE","Ingrese un nombre: ");
    ui->outputNombre->setText(name);
    std::string utf8_deleteName = name.toUtf8().constData();
    sequentialController.deleteTest(utf8_deleteName);
    MainWindow::on_pushButton_ACTUALIZAR_clicked();
}

void MainWindow::on_pushButton_SEARCH_RANGE_clicked()
{
    SequentialController sequentialController;
    QString beginName = QInputDialog::getText(this,"NOMBRE","Ingrese su nombre: ");
    ui->outputNombre->setText(beginName);
    QString endName = QInputDialog::getText(this,"NOMBRE","Ingrese su nombre: ");
    ui->outputNombre->setText(endName);
    std::string utf8_beginName = beginName.toUtf8().constData();
    std::string utf8_endName = endName.toUtf8().constData();
    auto result = sequentialController.searchPerRange(utf8_beginName, utf8_endName);

    int ix=0;
    while (ix != (int)result.size()){
        csvModel->setRowCount(ix);
        auto line = result[ix];

        vector<QString> values;

        QString Q_codigo = QString::fromStdString(line.getCodigo());
        QString Q_nombre = QString::fromStdString(line.getNombre());
        QString Q_carrera = QString::fromStdString(line.getCarrera());

        values.push_back(Q_codigo);
        values.push_back(Q_nombre);
        values.push_back(Q_carrera);

        const int colCount = values.size();
        csvModel->setColumnCount(colCount);
        for(int jx=0; jx < colCount;++jx){
            //if(ix==-1){
                //csvModel->setHeaderData(jx, Qt::Horizontal, values.at(jx));
            //}
            setValueAt(ix, jx, values.at(jx));
        }
        ix++;

        for(int i=0; i<colCount;++i){
            values.pop_back();
        }
    }
}

void MainWindow::on_pushButton_INITIAL_DATA_clicked()
{
    SequentialController sequentialController;
    sequentialController.insertAllTest();
    MainWindow::on_pushButton_ACTUALIZAR_clicked();
}
