#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mlib.h"

double firstNum;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTextCursor cursor;
    cursor.movePosition(QTextCursor::End);
    ui->label->setTextCursor(cursor);


    // connect(ui->pushButton_zero,SIGNAL(pressed()),this,SLOT());
    // connect(ui->pushButton_1,SIGNAL(released()),this,SLOT(()));
    // connect(ui->pushButton_2,SIGNAL(released()),this,SLOT(()));
    // connect(ui->pushButton_3,SIGNAL(released()),this,SLOT(()));
    // connect(ui->pushButton_4,SIGNAL(released()),this,SLOT(()));
    // connect(ui->pushButton_5,SIGNAL(released()),this,SLOT(()));
    // connect(ui->pushButton_6,SIGNAL(released()),this,SLOT(()));
    // connect(ui->pushButton_7,SIGNAL(released()),this,SLOT(()));
    // connect(ui->pushButton_8,SIGNAL(released()),this,SLOT(()));
    // connect(ui->pushButton_9,SIGNAL(released()),this,SLOT(()));

    // connect(ui->pushButton_plus,SIGNAL(released()),this,SLOT(()));
    // connect(ui->pushButton_min,SIGNAL(released()),this,SLOT(()));
    // connect(ui->pushButton_mul,SIGNAL(released()),this,SLOT(()));
    // connect(ui->pushButton_div,SIGNAL(released()),this,SLOT(()));

    // connect(ui->pushButton_lzav,SIGNAL(released()),this,SLOT(()));
    // connect(ui->pushButton_rzav,SIGNAL(released()),this,SLOT(()));

    // connect(ui->pushButton_fac,SIGNAL(released()),this,SLOT(()));
    // connect(ui->pushButton_sqrt,SIGNAL(released()),this,SLOT(()));
    // connect(ui->pushButton_pow,SIGNAL(released()),this,SLOT(()));
    // connect(ui->pushButton_abs,SIGNAL(released()),this,SLOT(()));
    // connect(ui->pushButton_decimal,SIGNAL(released()),this,SLOT(()));
    // connect(ui->pushButton_C,SIGNAL(released()),this,SLOT(()));
    // connect(ui->pushButton_eq,SIGNAL(released()),this,SLOT(()));



}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pushButton_C_released()
{
    ui->pushButton_plus->setChecked(false);
    ui->pushButton_min->setChecked(false);
    ui->pushButton_mul->setChecked(false);
    ui->pushButton_div->setChecked(false);

    ui->label->setText("");
    ui->label_2->setText("0");
}



void MainWindow::on_pushButton_zero_pressed()
{
    QString newLabel;
    QPushButton * button = (QPushButton*) sender();

    newLabel = ui->label->toPlainText() + button->text();
    ui->label->setText(newLabel);
    ui->pushButton_plus->setChecked(true);
}



void MainWindow::on_pushButton_1_pressed()
{
        QString newLabel;
        QPushButton * button = (QPushButton*) sender();

        newLabel = ui->label->toPlainText() + button->text();
        ui->label->setText(newLabel);
        ui->pushButton_plus->setChecked(true);
}


void MainWindow::on_pushButton_2_pressed()
{
        QString newLabel;
        QPushButton * button = (QPushButton*) sender();

        newLabel = ui->label->toPlainText() + button->text();
        ui->label->setText(newLabel);
        ui->pushButton_plus->setChecked(true);
}

void MainWindow::on_pushButton_3_pressed()
{
        QString newLabel;
        QPushButton * button = (QPushButton*) sender();

        newLabel = ui->label->toPlainText() + button->text();
        ui->label->setText(newLabel);
        ui->pushButton_plus->setChecked(true);
}

void MainWindow::on_pushButton_4_pressed()
{
        QString newLabel;
        QPushButton * button = (QPushButton*) sender();

        newLabel = ui->label->toPlainText() + button->text();
        ui->label->setText(newLabel);
        ui->pushButton_plus->setChecked(true);
}

void MainWindow::on_pushButton_5_pressed()
{
        QString newLabel;
        QPushButton * button = (QPushButton*) sender();

        newLabel = ui->label->toPlainText() + button->text();
        ui->label->setText(newLabel);
        ui->pushButton_plus->setChecked(true);
}

void MainWindow::on_pushButton_6_pressed()
{
        QString newLabel;
        QPushButton * button = (QPushButton*) sender();

        newLabel = ui->label->toPlainText() + button->text();
        ui->label->setText(newLabel);
        ui->pushButton_plus->setChecked(true);
}



void MainWindow::on_pushButton_7_pressed()
{
        QString newLabel;
        QPushButton * button = (QPushButton*) sender();

        newLabel = ui->label->toPlainText() + button->text();
        ui->label->setText(newLabel);
        ui->pushButton_plus->setChecked(true);
}



void MainWindow::on_pushButton_8_pressed()
{
        QString newLabel;
        QPushButton * button = (QPushButton*) sender();

        newLabel = ui->label->toPlainText() + button->text();
        ui->label->setText(newLabel);
        ui->pushButton_plus->setChecked(true);
}


void MainWindow::on_pushButton_9_pressed()
{
        QString newLabel;
        QPushButton * button = (QPushButton*) sender();

        newLabel = ui->label->toPlainText() + button->text();
        ui->label->setText(newLabel);
        ui->pushButton_plus->setChecked(true);
}


/**
 * @brief function adds to string left paranthese
 * @return void
 */
void MainWindow::on_pushButton_rzav_pressed()
{
        QString newLabel;
        QPushButton * button = (QPushButton*) sender();

        newLabel = ui->label->toPlainText() + button->text();
        ui->label->setText(newLabel);
        ui->pushButton_plus->setChecked(true);
}

/**
 * @brief function adds to string right paranthese
 * @return void
 */
void MainWindow::on_pushButton_lzav_pressed()
{
        QString newLabel;
        QPushButton * button = (QPushButton*) sender();

        newLabel = ui->label->toPlainText() + button->text();
        ui->label->setText(newLabel);
        ui->pushButton_plus->setChecked(true);
}

/**
 * @brief function adds to string division slash
 * @return void
 */
void MainWindow::on_pushButton_div_pressed()
{
        QString newLabel;
        QPushButton * button = (QPushButton*) sender();

        newLabel = ui->label->toPlainText() + button->text();
        ui->label->setText(newLabel);
        ui->pushButton_plus->setChecked(true);
}

/**
 * @brief function adds to string multiplication sign
 * @return void
 */
void MainWindow::on_pushButton_mul_pressed()
{
        QString newLabel;
        QPushButton * button = (QPushButton*) sender();

        newLabel = ui->label->toPlainText() + button->text();
        ui->label->setText(newLabel);
        ui->pushButton_plus->setChecked(true);
}

/**
 * @brief function adds to string minus sign
 * @return void
 */
void MainWindow::on_pushButton_min_pressed()
{
        QString newLabel;
        QPushButton * button = (QPushButton*) sender();

        newLabel = ui->label->toPlainText() + button->text();
        ui->label->setText(newLabel);
        ui->pushButton_plus->setChecked(true);
}

/**
 * @brief function adds to string plus sign
 * @return void
 */
void MainWindow::on_pushButton_plus_pressed()
{
        QString newLabel;
        QPushButton * button = (QPushButton*) sender();

        newLabel = ui->label->toPlainText() + button->text();
        ui->label->setText(newLabel);
        ui->pushButton_plus->setChecked(true);
}
/**
 * @brief function adds to string factorial symbol
 * @return void
 */
void MainWindow::on_pushButton_fac_pressed()
{
    QString newLabel;
    //QPushButton * button = (QPushButton*) sender();

    newLabel = ui->label->toPlainText() + "!";
    ui->label->setText(newLabel);
    ui->pushButton_fac->setChecked(true);
}


void MainWindow::on_pushButton_fac_released()
{
    return;
    //QString newLabel;
    //QPushButton * button = (QPushButton*) sender();
    //QString newLabel;
    //QPushButton * button = (QPushButton*) sender();

    //newLabel = ui->label->text() + button->text() + "pixfs";
    //ui->label->setText(newLabel);
    //ui->pushButton_plus->setChecked(true);
    //newLabel = "pixfs";
    //ui->label->setText(newLabel);
    //ui->pushButton_plus->setChecked(true);
}

/**
 * @brief function adds to string sqrt operator
 * @return void
 */
void MainWindow::on_pushButton_sqrt_pressed()
{
    QString newLabel;
    //QPushButton * button = (QPushButton*) sender();

    newLabel = ui->label->toPlainText() + "^(1/";
    ui->label->setText(newLabel);
    ui->pushButton_sqrt->setChecked(true);
}

/**
 * @brief function adds to string exponent operator
 * @return void
 */
void MainWindow::on_pushButton_pow_pressed()
{
    QString newLabel;
    //QPushButton * button = (QPushButton*) sender();

    newLabel = ui->label->toPlainText() + "^";
    ui->label->setText(newLabel);
    ui->pushButton_pow->setChecked(true);
}

/**
 * @brief function adds to string symbol of absolute value
 * @return void
 */
void MainWindow::on_pushButton_abs_pressed()
{
    QString newLabel;
    //QPushButton * button = (QPushButton*) sender();

    newLabel = ui->label->toPlainText() + "|";
    ui->label->setText(newLabel);
    ui->pushButton_plus->setChecked(true);
}

/**
 * @brief function adds to string coma symbol
 * @return void
 */
void MainWindow::on_pushButton_decimal_pressed()
{
    QString newLabel;
    //QPushButton * button = (QPushButton*) sender();

    newLabel = ui->label->toPlainText() + ".";
    ui->label->setText(newLabel);
    ui->pushButton_plus->setChecked(true);
}

/**
 * @brief Function removes last character of QString
 * @return void
 */
void MainWindow::on_pushButton_back_pressed()
{
    QString newLabel;

      newLabel = ui->label->toPlainText();
      //ui->label->textCursor().deletePreviousChar();
      newLabel.remove(newLabel.length()-1,1);
      ui->label->setText(newLabel);
}
/**
 * @brief Function prints result or error on label
 * @return void
 */
void MainWindow::on_pushButton_eq_pressed()
{

    /*std::string s = (ui->label->toPlainText()).toStdString();
    double res;
    OPERATION_RESULT op = eval(s, &res);
    if(op == SUCCESS){
        ui->label->setText(QString::number(res));
        ui->label_2->setText(ui->label->toPlainText());
    }else if(op == ERR_MALFORMED_EXPRESSION){
        ui->label_2->setText("Malformed expression!");
    }else{
        ui->label_2->setText("Some operation has failed!");
    }*/
    ui->label->setText(ui->label_2->text());
}


void MainWindow::on_label_textChanged()
{
    std::string s = (ui->label->toPlainText()).toStdString();
    double res;
    //s = "10.2 + 1.8";
    OPERATION_RESULT op = eval(s, &res);
    if(op == SUCCESS){
        ui->label_2->setText(QString::number(res));
    }else if(op == ERR_MALFORMED_EXPRESSION){
        ui->label_2->setText("Malformed expression!");
    }else{
        ui->label_2->setText("Some operation has failed!");
    }
}

