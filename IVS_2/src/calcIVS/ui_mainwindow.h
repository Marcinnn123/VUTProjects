/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *label_2;
    QPushButton *pushButton_back;
    QPushButton *pushButton_rzav;
    QPushButton *pushButton_lzav;
    QPushButton *pushButton_div;
    QPushButton *pushButton_C;
    QPushButton *pushButton_9;
    QPushButton *pushButton_mul;
    QPushButton *pushButton_8;
    QPushButton *pushButton_7;
    QPushButton *pushButton_abs;
    QPushButton *pushButton_6;
    QPushButton *pushButton_min;
    QPushButton *pushButton_5;
    QPushButton *pushButton_4;
    QPushButton *pushButton_sqrt;
    QPushButton *pushButton_3;
    QPushButton *pushButton_plus;
    QPushButton *pushButton_2;
    QPushButton *pushButton_1;
    QPushButton *pushButton_pow;
    QPushButton *pushButton_decimal;
    QPushButton *pushButton_eq;
    QPushButton *pushButton_zero;
    QPushButton *pushButton_fac;
    QTextEdit *label;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(600, 610);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(600, 610));
        MainWindow->setMaximumSize(QSize(600, 610));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(0, 120, 600, 90));
        QFont font;
        font.setPointSize(30);
        font.setBold(true);
        label_2->setFont(font);
        label_2->setCursor(QCursor(Qt::IBeamCursor));
        label_2->setMouseTracking(true);
        label_2->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	color: red;\n"
"}"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_2->setMargin(24);
        pushButton_back = new QPushButton(centralwidget);
        pushButton_back->setObjectName(QString::fromUtf8("pushButton_back"));
        pushButton_back->setGeometry(QRect(500, 210, 100, 100));
        QFont font1;
        font1.setPointSize(18);
        font1.setBold(true);
        pushButton_back->setFont(font1);
        pushButton_back->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: #f4ebd0;\n"
"border: 1px solid grey \n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #DEDAB0;\n"
"}\n"
""));
        pushButton_rzav = new QPushButton(centralwidget);
        pushButton_rzav->setObjectName(QString::fromUtf8("pushButton_rzav"));
        pushButton_rzav->setGeometry(QRect(500, 310, 100, 100));
        pushButton_rzav->setFont(font1);
        pushButton_rzav->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: #f4ebd0;\n"
"border: 1px solid grey \n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #DEDAB0;\n"
"}\n"
""));
        pushButton_lzav = new QPushButton(centralwidget);
        pushButton_lzav->setObjectName(QString::fromUtf8("pushButton_lzav"));
        pushButton_lzav->setGeometry(QRect(500, 410, 100, 100));
        pushButton_lzav->setFont(font1);
        pushButton_lzav->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: #f4ebd0;\n"
"border: 1px solid grey;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #DEDAB0;\n"
"}\n"
""));
        pushButton_div = new QPushButton(centralwidget);
        pushButton_div->setObjectName(QString::fromUtf8("pushButton_div"));
        pushButton_div->setGeometry(QRect(400, 210, 100, 100));
        pushButton_div->setFont(font1);
        pushButton_div->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: #f4ebd0;\n"
"border: 1px solid grey \n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #DEDAB0;\n"
"}\n"
""));
        pushButton_C = new QPushButton(centralwidget);
        pushButton_C->setObjectName(QString::fromUtf8("pushButton_C"));
        pushButton_C->setGeometry(QRect(0, 210, 100, 100));
        pushButton_C->setFont(font1);
        pushButton_C->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: #DC143C;\n"
"border: 1px solid grey;\n"
"\n"
"width: 20%;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #A52A2A;\n"
"}\n"
""));
        pushButton_9 = new QPushButton(centralwidget);
        pushButton_9->setObjectName(QString::fromUtf8("pushButton_9"));
        pushButton_9->setGeometry(QRect(300, 210, 100, 100));
        pushButton_9->setFont(font1);
        pushButton_9->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: #f4ebd0;\n"
"border: 1px solid grey \n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #DEDAB0;\n"
"}\n"
""));
        pushButton_mul = new QPushButton(centralwidget);
        pushButton_mul->setObjectName(QString::fromUtf8("pushButton_mul"));
        pushButton_mul->setGeometry(QRect(400, 310, 100, 100));
        pushButton_mul->setFont(font1);
        pushButton_mul->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: #f4ebd0;\n"
"border: 1px solid grey \n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #DEDAB0;\n"
"}\n"
""));
        pushButton_8 = new QPushButton(centralwidget);
        pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_8->setGeometry(QRect(200, 210, 100, 100));
        pushButton_8->setFont(font1);
        pushButton_8->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: #f4ebd0;\n"
"border: 1px solid grey \n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #DEDAB0;\n"
"}\n"
""));
        pushButton_7 = new QPushButton(centralwidget);
        pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));
        pushButton_7->setGeometry(QRect(100, 210, 100, 100));
        pushButton_7->setFont(font1);
        pushButton_7->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: #f4ebd0;\n"
"border: 1px solid grey;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #DEDAB0;\n"
"}\n"
""));
        pushButton_abs = new QPushButton(centralwidget);
        pushButton_abs->setObjectName(QString::fromUtf8("pushButton_abs"));
        pushButton_abs->setGeometry(QRect(500, 510, 100, 100));
        pushButton_abs->setFont(font1);
        pushButton_abs->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: #f4ebd0;\n"
"border: 1px solid grey \n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #DEDAB0;\n"
"}\n"
""));
        pushButton_6 = new QPushButton(centralwidget);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        pushButton_6->setGeometry(QRect(300, 310, 100, 100));
        pushButton_6->setFont(font1);
        pushButton_6->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: #f4ebd0;\n"
"border: 1px solid grey \n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #DEDAB0;\n"
"}\n"
""));
        pushButton_min = new QPushButton(centralwidget);
        pushButton_min->setObjectName(QString::fromUtf8("pushButton_min"));
        pushButton_min->setGeometry(QRect(400, 410, 100, 100));
        pushButton_min->setFont(font1);
        pushButton_min->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: #f4ebd0;\n"
"border: 1px solid grey \n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #DEDAB0;\n"
"}\n"
""));
        pushButton_5 = new QPushButton(centralwidget);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setGeometry(QRect(200, 310, 100, 100));
        pushButton_5->setFont(font1);
        pushButton_5->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: #f4ebd0;\n"
"border: 1px solid grey \n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #DEDAB0;\n"
"}\n"
""));
        pushButton_4 = new QPushButton(centralwidget);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(100, 310, 100, 100));
        pushButton_4->setFont(font1);
        pushButton_4->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: #f4ebd0;\n"
"border: 1px solid grey \n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #DEDAB0;\n"
"}\n"
""));
        pushButton_sqrt = new QPushButton(centralwidget);
        pushButton_sqrt->setObjectName(QString::fromUtf8("pushButton_sqrt"));
        pushButton_sqrt->setGeometry(QRect(0, 410, 100, 100));
        pushButton_sqrt->setFont(font1);
        pushButton_sqrt->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: #f4ebd0;\n"
"border: 1px solid grey \n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #DEDAB0;\n"
"}\n"
""));
        pushButton_3 = new QPushButton(centralwidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(300, 410, 100, 100));
        pushButton_3->setFont(font1);
        pushButton_3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: #f4ebd0;\n"
"border: 1px solid grey \n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #DEDAB0;\n"
"}\n"
""));
        pushButton_plus = new QPushButton(centralwidget);
        pushButton_plus->setObjectName(QString::fromUtf8("pushButton_plus"));
        pushButton_plus->setGeometry(QRect(400, 510, 100, 100));
        pushButton_plus->setFont(font1);
        pushButton_plus->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: #f4ebd0;\n"
"border: 1px solid grey \n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #DEDAB0;\n"
"}\n"
""));
        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(200, 410, 100, 100));
        pushButton_2->setFont(font1);
        pushButton_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: #f4ebd0;\n"
"border: 1px solid grey \n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #DEDAB0;\n"
"}\n"
""));
        pushButton_1 = new QPushButton(centralwidget);
        pushButton_1->setObjectName(QString::fromUtf8("pushButton_1"));
        pushButton_1->setGeometry(QRect(100, 410, 100, 100));
        pushButton_1->setFont(font1);
        pushButton_1->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: #f4ebd0;\n"
"border: 1px solid grey \n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #DEDAB0;\n"
"}\n"
""));
        pushButton_pow = new QPushButton(centralwidget);
        pushButton_pow->setObjectName(QString::fromUtf8("pushButton_pow"));
        pushButton_pow->setGeometry(QRect(0, 510, 100, 100));
        pushButton_pow->setFont(font1);
        pushButton_pow->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: #f4ebd0;\n"
"border: 1px solid grey \n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #DEDAB0;\n"
"}\n"
""));
        pushButton_decimal = new QPushButton(centralwidget);
        pushButton_decimal->setObjectName(QString::fromUtf8("pushButton_decimal"));
        pushButton_decimal->setGeometry(QRect(100, 510, 100, 100));
        pushButton_decimal->setFont(font1);
        pushButton_decimal->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: #f4ebd0;\n"
"border: 1px solid grey \n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #DEDAB0;\n"
"}\n"
""));
        pushButton_eq = new QPushButton(centralwidget);
        pushButton_eq->setObjectName(QString::fromUtf8("pushButton_eq"));
        pushButton_eq->setGeometry(QRect(300, 510, 100, 100));
        pushButton_eq->setFont(font1);
        pushButton_eq->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"  background-color: #87cefa;\n"
"  border: 1px solid gray;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #1e90ff;\n"
"}"));
        pushButton_zero = new QPushButton(centralwidget);
        pushButton_zero->setObjectName(QString::fromUtf8("pushButton_zero"));
        pushButton_zero->setGeometry(QRect(200, 510, 100, 100));
        pushButton_zero->setFont(font1);
        pushButton_zero->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: #f4ebd0;\n"
"border: 1px solid grey \n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #DEDAB0;\n"
"}\n"
""));
        pushButton_fac = new QPushButton(centralwidget);
        pushButton_fac->setObjectName(QString::fromUtf8("pushButton_fac"));
        pushButton_fac->setGeometry(QRect(0, 310, 100, 100));
        pushButton_fac->setFont(font1);
        pushButton_fac->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: #f4ebd0;\n"
"border: 1px solid grey \n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #DEDAB0;\n"
"}\n"
""));
        pushButton_fac->setIconSize(QSize(16, 16));
        label = new QTextEdit(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 0, 600, 120));
        QFont font2;
        font2.setPointSize(34);
        font2.setBold(true);
        label->setFont(font2);
        label->viewport()->setProperty("cursor", QVariant(QCursor(Qt::IBeamCursor)));
        label->setAutoFillBackground(true);
        MainWindow->setCentralWidget(centralwidget);
        pushButton_4->raise();
        label_2->raise();
        pushButton_back->raise();
        pushButton_rzav->raise();
        pushButton_lzav->raise();
        pushButton_div->raise();
        pushButton_C->raise();
        pushButton_9->raise();
        pushButton_mul->raise();
        pushButton_8->raise();
        pushButton_7->raise();
        pushButton_abs->raise();
        pushButton_6->raise();
        pushButton_min->raise();
        pushButton_5->raise();
        pushButton_sqrt->raise();
        pushButton_3->raise();
        pushButton_plus->raise();
        pushButton_2->raise();
        pushButton_1->raise();
        pushButton_pow->raise();
        pushButton_decimal->raise();
        pushButton_eq->raise();
        pushButton_zero->raise();
        pushButton_fac->raise();
        label->raise();

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "0", nullptr));
        pushButton_back->setText(QApplication::translate("MainWindow", "\342\214\253", nullptr));
        pushButton_rzav->setText(QApplication::translate("MainWindow", "(", nullptr));
        pushButton_lzav->setText(QApplication::translate("MainWindow", ")", nullptr));
        pushButton_div->setText(QApplication::translate("MainWindow", "/", nullptr));
        pushButton_C->setText(QApplication::translate("MainWindow", "C", nullptr));
        pushButton_9->setText(QApplication::translate("MainWindow", "9", nullptr));
        pushButton_mul->setText(QApplication::translate("MainWindow", "*", nullptr));
        pushButton_8->setText(QApplication::translate("MainWindow", "8", nullptr));
        pushButton_7->setText(QApplication::translate("MainWindow", "7", nullptr));
        pushButton_abs->setText(QApplication::translate("MainWindow", "|x|", nullptr));
        pushButton_6->setText(QApplication::translate("MainWindow", "6", nullptr));
        pushButton_min->setText(QApplication::translate("MainWindow", "-", nullptr));
        pushButton_5->setText(QApplication::translate("MainWindow", "5", nullptr));
        pushButton_4->setText(QApplication::translate("MainWindow", "4", nullptr));
        pushButton_sqrt->setText(QApplication::translate("MainWindow", "x^(1/y)", nullptr));
        pushButton_3->setText(QApplication::translate("MainWindow", "3", nullptr));
        pushButton_plus->setText(QApplication::translate("MainWindow", "+", nullptr));
        pushButton_2->setText(QApplication::translate("MainWindow", "2", nullptr));
        pushButton_1->setText(QApplication::translate("MainWindow", "1", nullptr));
        pushButton_pow->setText(QApplication::translate("MainWindow", "x^y", nullptr));
        pushButton_decimal->setText(QApplication::translate("MainWindow", ".", nullptr));
        pushButton_eq->setText(QApplication::translate("MainWindow", "=", nullptr));
        pushButton_zero->setText(QApplication::translate("MainWindow", "0", nullptr));
        pushButton_fac->setText(QApplication::translate("MainWindow", "x!", nullptr));
        label->setHtml(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:34pt; font-weight:700; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
