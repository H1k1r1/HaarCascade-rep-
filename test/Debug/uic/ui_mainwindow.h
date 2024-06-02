/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *loadButton;
    QLabel *imageLabel;
    QProgressBar *progressBar;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QLabel *label;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1080, 880);
        MainWindow->setMinimumSize(QSize(0, 0));
        MainWindow->setBaseSize(QSize(0, 0));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        loadButton = new QPushButton(centralwidget);
        loadButton->setObjectName(QString::fromUtf8("loadButton"));
        loadButton->setGeometry(QRect(19, 10, 221, 41));
        imageLabel = new QLabel(centralwidget);
        imageLabel->setObjectName(QString::fromUtf8("imageLabel"));
        imageLabel->setGeometry(QRect(20, 60, 1031, 691));
        imageLabel->setFrameShape(QFrame::Box);
        imageLabel->setScaledContents(true);
        progressBar = new QProgressBar(centralwidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(300, 770, 501, 31));
        progressBar->setMinimum(0);
        progressBar->setMaximum(100);
        progressBar->setValue(0);
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(950, 10, 101, 31));
        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(830, 10, 91, 31));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(256, 20, 541, 31));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1080, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);
        QObject::connect(pushButton, SIGNAL(clicked()), MainWindow, SLOT(close()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), imageLabel, SLOT(clear()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), progressBar, SLOT(reset()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "HaarCascade", nullptr));
        loadButton->setText(QCoreApplication::translate("MainWindow", "Load Image", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Close", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "Reset", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "                         Special for HSE students", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
