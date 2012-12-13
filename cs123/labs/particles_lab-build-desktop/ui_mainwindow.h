/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Tue Nov 27 19:29:33 2012
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDial>
#include <QtGui/QDockWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "particlewidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    ParticleWidget *widget;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QDial *velocityDial;
    QSlider *velocitySlider;
    QLabel *velocityLabel1;
    QLabel *velocityLabel2;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_3;
    QDial *forceDial;
    QSlider *forceSlider;
    QLabel *forceLabel1;
    QLabel *forceLabel2;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_5;
    QSlider *speedSlider;
    QLabel *speedLabel;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1199, 514);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        widget = new ParticleWidget(centralWidget);
        widget->setObjectName(QString::fromUtf8("widget"));

        gridLayout->addWidget(widget, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        dockWidget = new QDockWidget(MainWindow);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayout = new QVBoxLayout(dockWidgetContents);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(dockWidgetContents);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setSpacing(2);
        gridLayout_2->setContentsMargins(2, 2, 2, 2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        velocityDial = new QDial(groupBox);
        velocityDial->setObjectName(QString::fromUtf8("velocityDial"));
        velocityDial->setMaximum(360);
        velocityDial->setTracking(true);
        velocityDial->setWrapping(true);
        velocityDial->setNotchesVisible(true);

        gridLayout_2->addWidget(velocityDial, 0, 0, 1, 1);

        velocitySlider = new QSlider(groupBox);
        velocitySlider->setObjectName(QString::fromUtf8("velocitySlider"));
        velocitySlider->setMinimum(0);
        velocitySlider->setMaximum(300);
        velocitySlider->setValue(0);
        velocitySlider->setOrientation(Qt::Vertical);

        gridLayout_2->addWidget(velocitySlider, 0, 1, 1, 1);

        velocityLabel1 = new QLabel(groupBox);
        velocityLabel1->setObjectName(QString::fromUtf8("velocityLabel1"));

        gridLayout_2->addWidget(velocityLabel1, 1, 0, 1, 1);

        velocityLabel2 = new QLabel(groupBox);
        velocityLabel2->setObjectName(QString::fromUtf8("velocityLabel2"));

        gridLayout_2->addWidget(velocityLabel2, 2, 0, 1, 1);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(dockWidgetContents);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_3 = new QGridLayout(groupBox_2);
        gridLayout_3->setSpacing(2);
        gridLayout_3->setContentsMargins(2, 2, 2, 2);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        forceDial = new QDial(groupBox_2);
        forceDial->setObjectName(QString::fromUtf8("forceDial"));
        forceDial->setMaximum(360);
        forceDial->setWrapping(true);
        forceDial->setNotchesVisible(true);

        gridLayout_3->addWidget(forceDial, 0, 0, 1, 1);

        forceSlider = new QSlider(groupBox_2);
        forceSlider->setObjectName(QString::fromUtf8("forceSlider"));
        forceSlider->setMinimum(0);
        forceSlider->setMaximum(300);
        forceSlider->setValue(0);
        forceSlider->setOrientation(Qt::Vertical);

        gridLayout_3->addWidget(forceSlider, 0, 1, 1, 1);

        forceLabel1 = new QLabel(groupBox_2);
        forceLabel1->setObjectName(QString::fromUtf8("forceLabel1"));

        gridLayout_3->addWidget(forceLabel1, 1, 0, 1, 1);

        forceLabel2 = new QLabel(groupBox_2);
        forceLabel2->setObjectName(QString::fromUtf8("forceLabel2"));

        gridLayout_3->addWidget(forceLabel2, 2, 0, 1, 1);


        verticalLayout->addWidget(groupBox_2);

        groupBox_4 = new QGroupBox(dockWidgetContents);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        gridLayout_5 = new QGridLayout(groupBox_4);
        gridLayout_5->setSpacing(2);
        gridLayout_5->setContentsMargins(2, 2, 2, 2);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        speedSlider = new QSlider(groupBox_4);
        speedSlider->setObjectName(QString::fromUtf8("speedSlider"));
        speedSlider->setValue(50);
        speedSlider->setOrientation(Qt::Horizontal);

        gridLayout_5->addWidget(speedSlider, 0, 0, 1, 1);

        speedLabel = new QLabel(groupBox_4);
        speedLabel->setObjectName(QString::fromUtf8("speedLabel"));

        gridLayout_5->addWidget(speedLabel, 1, 0, 1, 1);


        verticalLayout->addWidget(groupBox_4);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalSpacer = new QSpacerItem(200, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(horizontalSpacer);

        dockWidget->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "[Lab06] Particles", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("MainWindow", "Velocity", 0, QApplication::UnicodeUTF8));
        velocityLabel1->setText(QApplication::translate("MainWindow", "TextLabel", 0, QApplication::UnicodeUTF8));
        velocityLabel2->setText(QApplication::translate("MainWindow", "TextLabel", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Force", 0, QApplication::UnicodeUTF8));
        forceLabel1->setText(QApplication::translate("MainWindow", "TextLabel", 0, QApplication::UnicodeUTF8));
        forceLabel2->setText(QApplication::translate("MainWindow", "TextLabel", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("MainWindow", "Speed", 0, QApplication::UnicodeUTF8));
        speedLabel->setText(QApplication::translate("MainWindow", "TextLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
