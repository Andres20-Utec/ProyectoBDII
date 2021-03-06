#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_INSERT_clicked();

    void on_pushButton_SEARCH_clicked();

    void on_pushButton_DELETE_clicked();

    void on_pushButton_ACTUALIZAR_clicked();

    void on_pushButton_SEARCH_RANGE_clicked();

private:
    Ui::MainWindow *ui;
    void setValueAt(int ix, int jx, const QString &value);
    QStandardItemModel *csvModel;
};
#endif // MAINWINDOW_H
