#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:

    void on_pushButton_C_released();

    void on_pushButton_zero_pressed();
    void on_pushButton_1_pressed();
    void on_pushButton_2_pressed();
    void on_pushButton_3_pressed();
    void on_pushButton_4_pressed();
    void on_pushButton_5_pressed();
    void on_pushButton_6_pressed();
    void on_pushButton_7_pressed();
    void on_pushButton_8_pressed();
    void on_pushButton_9_pressed();
    void on_pushButton_rzav_pressed();
    void on_pushButton_lzav_pressed();
    void on_pushButton_div_pressed();
    void on_pushButton_mul_pressed();
    void on_pushButton_min_pressed();
    void on_pushButton_plus_pressed();
    void on_pushButton_fac_pressed();
    void on_pushButton_fac_released();
    void on_pushButton_sqrt_pressed();
    void on_pushButton_pow_pressed();
    void on_pushButton_abs_pressed();
    void on_pushButton_decimal_pressed();
    void on_pushButton_back_pressed();
    void on_pushButton_eq_pressed();
    void on_label_textChanged();
};
#endif // MAINWINDOW_H
