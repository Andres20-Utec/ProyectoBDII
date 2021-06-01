/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTableView *tableView;
    QPushButton *pushButton_INSERT;
    QPushButton *pushButton_SEARCH;
    QPushButton *pushButton_DELETE;
    QPushButton *pushButton_ACTUALIZAR;
    QPushButton *pushButton_SEARCH_RANGE;
    QLabel *outputCodigo;
    QLabel *outputNombre;
    QLabel *outputCarrera;
    QPushButton *pushButton_INITIAL_DATA;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1021, 788);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        tableView = new QTableView(centralwidget);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setGeometry(QRect(10, 320, 1001, 421));
        pushButton_INSERT = new QPushButton(centralwidget);
        pushButton_INSERT->setObjectName(QString::fromUtf8("pushButton_INSERT"));
        pushButton_INSERT->setGeometry(QRect(20, 20, 161, 71));
        pushButton_SEARCH = new QPushButton(centralwidget);
        pushButton_SEARCH->setObjectName(QString::fromUtf8("pushButton_SEARCH"));
        pushButton_SEARCH->setGeometry(QRect(20, 120, 161, 71));
        pushButton_DELETE = new QPushButton(centralwidget);
        pushButton_DELETE->setObjectName(QString::fromUtf8("pushButton_DELETE"));
        pushButton_DELETE->setGeometry(QRect(20, 230, 161, 71));
        pushButton_ACTUALIZAR = new QPushButton(centralwidget);
        pushButton_ACTUALIZAR->setObjectName(QString::fromUtf8("pushButton_ACTUALIZAR"));
        pushButton_ACTUALIZAR->setGeometry(QRect(840, 230, 161, 71));
        pushButton_SEARCH_RANGE = new QPushButton(centralwidget);
        pushButton_SEARCH_RANGE->setObjectName(QString::fromUtf8("pushButton_SEARCH_RANGE"));
        pushButton_SEARCH_RANGE->setGeometry(QRect(230, 20, 161, 71));
        outputCodigo = new QLabel(centralwidget);
        outputCodigo->setObjectName(QString::fromUtf8("outputCodigo"));
        outputCodigo->setGeometry(QRect(840, 100, 67, 17));
        outputNombre = new QLabel(centralwidget);
        outputNombre->setObjectName(QString::fromUtf8("outputNombre"));
        outputNombre->setGeometry(QRect(840, 140, 67, 17));
        outputCarrera = new QLabel(centralwidget);
        outputCarrera->setObjectName(QString::fromUtf8("outputCarrera"));
        outputCarrera->setGeometry(QRect(840, 180, 67, 17));
        pushButton_INITIAL_DATA = new QPushButton(centralwidget);
        pushButton_INITIAL_DATA->setObjectName(QString::fromUtf8("pushButton_INITIAL_DATA"));
        pushButton_INITIAL_DATA->setGeometry(QRect(240, 120, 151, 71));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1021, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButton_INSERT->setText(QCoreApplication::translate("MainWindow", "INSERT", nullptr));
        pushButton_SEARCH->setText(QCoreApplication::translate("MainWindow", "SEARCH", nullptr));
        pushButton_DELETE->setText(QCoreApplication::translate("MainWindow", "DELETE", nullptr));
        pushButton_ACTUALIZAR->setText(QCoreApplication::translate("MainWindow", "ACTUALIZAR", nullptr));
        pushButton_SEARCH_RANGE->setText(QCoreApplication::translate("MainWindow", "SEARCH_RANGE", nullptr));
        outputCodigo->setText(QCoreApplication::translate("MainWindow", "Codigo", nullptr));
        outputNombre->setText(QCoreApplication::translate("MainWindow", "Nombre", nullptr));
        outputCarrera->setText(QCoreApplication::translate("MainWindow", "Carrera", nullptr));
        pushButton_INITIAL_DATA->setText(QCoreApplication::translate("MainWindow", "INITIAL_DATA", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
