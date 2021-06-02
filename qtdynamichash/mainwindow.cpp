#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "HashController.h"
#include "Alumno.h"
#include "Bucket.h"
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
    csvModel->setColumnCount(6);
    csvModel->setHorizontalHeaderLabels(QStringList()<< "Codigo" << "Nombre" << "Apellidos" << "Carrera" << "Ciclo" << "Mensualidad");
    ui->tableView->setModel(csvModel);
    ui->outputCodigo->setText("");
    ui->outputNombre->setText("");
    ui->outputApellidos->setText("");
    ui->outputCarrera->setText("");
    ui->outputCiclo->setText("");
    ui->outputMensualidad->setText("");
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

void MainWindow::on_pushButton_ACTUALIZAR_clicked()
{
    auto result = test.returnAllRecord();

    int ix=0;
    while (ix != (int)result.size()){
        csvModel->setRowCount(ix);
        auto line = result[ix];

        vector<QString> values;

        QString Q_codigo = QString::fromStdString(line.getCodigo());
        QString Q_nombre = QString::fromStdString(line.getNombre());
        QString Q_apellidos = QString::fromStdString(line.getApellidos());
        QString Q_carrera = QString::fromStdString(line.getCarrera());
        QString Q_ciclo = QString::fromStdString(to_string(line.getCiclo()));
        QString Q_mensualidad = QString::fromStdString(to_string(line.getMensualidad()));

        values.push_back(Q_codigo);
        values.push_back(Q_nombre);
        values.push_back(Q_apellidos);
        values.push_back(Q_carrera);
        values.push_back(Q_ciclo);
        values.push_back(Q_mensualidad);

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
    HashController hashController;
    QString inputCodigo = QInputDialog::getText(this,"CODIGO","Ingrese su codigo: ");
    ui->outputCodigo->setText(inputCodigo);
    QString inputNombre = QInputDialog::getText(this,"NOMBRE","Ingrese su nombre: ");
    ui->outputNombre->setText(inputNombre);
    QString inputApellidos = QInputDialog::getText(this,"APELLIDOS","Ingrese su apellido: ");
    ui->outputApellidos->setText(inputApellidos);
    QString inputCarrera = QInputDialog::getText(this,"CARRERA","Ingrese su carrera: ");
    ui->outputCarrera->setText(inputCarrera);
    int inputCiclo = QInputDialog::getInt(this,"CICLO","Ingrese su ciclo: ");
    ui->outputCiclo->setText(QString::number(inputCiclo));
    float inputMensualidad = QInputDialog::getDouble(this, "MENSUALIDAD","Ingrese su mensualidad: ");
    ui->outputMensualidad->setText(QString::number(inputMensualidad));

    // Cambiando QString to string
    std::string utf8_inputCodigo = inputCodigo.toUtf8().constData();
    std::string utf8_inputNombre = inputNombre.toUtf8().constData();
    std::string utf8_inputApellidos = inputApellidos.toUtf8().constData();
    std::string utf8_inputCarrera = inputCarrera.toUtf8().constData();
    hashController.insertTest(utf8_inputCodigo, utf8_inputNombre, utf8_inputApellidos, utf8_inputCarrera, inputCiclo, inputMensualidad);
    MainWindow::on_pushButton_ACTUALIZAR_clicked();
}

void MainWindow::on_pushButton_SEARCH_clicked()
{
    HashController hashController;
    QString inputCodigo = QInputDialog::getText(this,"CODIGO","Ingrese su codigo: ");
    ui->outputCodigo->setText(inputCodigo);
    std::string utf8_inputCodigo = inputCodigo.toUtf8().constData();
    auto result = hashController.searchTest(utf8_inputCodigo);

    int ix=0;
    while (ix != (int)result.size()){
        csvModel->setRowCount(ix);
        auto line = result[ix];

        vector<QString> values;

        QString Q_codigo = QString::fromStdString(line.getCodigo());
        QString Q_nombre = QString::fromStdString(line.getNombre());
        QString Q_apellidos = QString::fromStdString(line.getApellidos());
        QString Q_carrera = QString::fromStdString(line.getCarrera());
        QString Q_ciclo = QString::fromStdString(to_string(line.getCiclo()));
        QString Q_mensualidad = QString::fromStdString(to_string(line.getMensualidad()));

        values.push_back(Q_codigo);
        values.push_back(Q_nombre);
        values.push_back(Q_apellidos);
        values.push_back(Q_carrera);
        values.push_back(Q_ciclo);
        values.push_back(Q_mensualidad);

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
    HashController hashController;
    QString inputCodigo = QInputDialog::getText(this,"CODIGO","Ingrese su codigo: ");
    std::string utf8_inputCodigo = inputCodigo.toUtf8().constData();
    hashController.deleteTest(utf8_inputCodigo);
    MainWindow::on_pushButton_ACTUALIZAR_clicked();
}

void MainWindow::on_pushButton_SEARCH_RANGE_clicked()
{
    HashController hashController;
    QString beg_inputCodigo = QInputDialog::getText(this,"CODIGO","Ingrese su codigo inicial: ");
    std::string utf8_beg_inputCodigo = beg_inputCodigo.toUtf8().constData();
    QString end_inputCodigo = QInputDialog::getText(this,"CODIGO","Ingrese su codigo final: ");
    std::string utf8_end_inputCodigo = end_inputCodigo.toUtf8().constData();
    auto result = hashController.searchPerRangeTest(utf8_beg_inputCodigo, utf8_end_inputCodigo);

    int ix=0;
    while (ix != (int)result.size()){
        csvModel->setRowCount(ix);
        auto line = result[ix];

        vector<QString> values;

        QString Q_codigo = QString::fromStdString(line.getCodigo());
        QString Q_nombre = QString::fromStdString(line.getNombre());
        QString Q_apellidos = QString::fromStdString(line.getApellidos());
        QString Q_carrera = QString::fromStdString(line.getCarrera());
        QString Q_ciclo = QString::fromStdString(to_string(line.getCiclo()));
        QString Q_mensualidad = QString::fromStdString(to_string(line.getMensualidad()));

        values.push_back(Q_codigo);
        values.push_back(Q_nombre);
        values.push_back(Q_apellidos);
        values.push_back(Q_carrera);
        values.push_back(Q_ciclo);
        values.push_back(Q_mensualidad);

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
