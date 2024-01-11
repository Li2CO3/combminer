#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QLabel>
#include <QMainWindow>
#include "calc.hpp"


#define NTHREAD 4


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Calc *c[NTHREAD]={NULL};
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString progress[NTHREAD];
    QLabel * label1[NTHREAD];
    QLabel * label2[NTHREAD];

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
