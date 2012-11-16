/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Fri Oct 5 16:38:41 2012
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
#include <QtGui/QCheckBox>
#include <QtGui/QDockWidget>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QRadioButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "glwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionQuit;
    GLWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QDockWidget *settingsDock;
    QWidget *settingsDockContent;
    QVBoxLayout *settingsLayout;
    QGroupBox *fillModeGroupBox;
    QRadioButton *fillModePoints;
    QRadioButton *fillModeShaded;
    QRadioButton *fillModeWireframe;
    QGroupBox *lightingGroupBox;
    QCheckBox *lightingEnabled;
    QGroupBox *shadingGroupBox;
    QRadioButton *shadingModeFlat;
    QRadioButton *shadingModeSmooth;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(600, 400);
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        centralWidget = new GLWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 25));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        MainWindow->setMenuBar(menuBar);
        settingsDock = new QDockWidget(MainWindow);
        settingsDock->setObjectName(QString::fromUtf8("settingsDock"));
        settingsDock->setMinimumSize(QSize(140, 211));
        settingsDock->setFeatures(QDockWidget::DockWidgetMovable);
        settingsDockContent = new QWidget();
        settingsDockContent->setObjectName(QString::fromUtf8("settingsDockContent"));
        settingsDockContent->setAutoFillBackground(false);
        settingsLayout = new QVBoxLayout(settingsDockContent);
        settingsLayout->setSpacing(6);
        settingsLayout->setContentsMargins(11, 11, 11, 11);
        settingsLayout->setObjectName(QString::fromUtf8("settingsLayout"));
        fillModeGroupBox = new QGroupBox(settingsDockContent);
        fillModeGroupBox->setObjectName(QString::fromUtf8("fillModeGroupBox"));
        fillModePoints = new QRadioButton(fillModeGroupBox);
        fillModePoints->setObjectName(QString::fromUtf8("fillModePoints"));
        fillModePoints->setGeometry(QRect(0, 30, 140, 22));
        fillModePoints->setChecked(true);
        fillModeShaded = new QRadioButton(fillModeGroupBox);
        fillModeShaded->setObjectName(QString::fromUtf8("fillModeShaded"));
        fillModeShaded->setGeometry(QRect(0, 90, 140, 22));
        fillModeWireframe = new QRadioButton(fillModeGroupBox);
        fillModeWireframe->setObjectName(QString::fromUtf8("fillModeWireframe"));
        fillModeWireframe->setGeometry(QRect(0, 60, 140, 22));

        settingsLayout->addWidget(fillModeGroupBox);

        lightingGroupBox = new QGroupBox(settingsDockContent);
        lightingGroupBox->setObjectName(QString::fromUtf8("lightingGroupBox"));
        lightingGroupBox->setMinimumSize(QSize(0, 0));
        lightingGroupBox->setMaximumSize(QSize(16777215, 60));
        lightingEnabled = new QCheckBox(lightingGroupBox);
        lightingEnabled->setObjectName(QString::fromUtf8("lightingEnabled"));
        lightingEnabled->setEnabled(true);
        lightingEnabled->setGeometry(QRect(0, 30, 140, 22));

        settingsLayout->addWidget(lightingGroupBox);

        shadingGroupBox = new QGroupBox(settingsDockContent);
        shadingGroupBox->setObjectName(QString::fromUtf8("shadingGroupBox"));
        shadingGroupBox->setEnabled(false);
        shadingModeFlat = new QRadioButton(shadingGroupBox);
        shadingModeFlat->setObjectName(QString::fromUtf8("shadingModeFlat"));
        shadingModeFlat->setGeometry(QRect(0, 30, 140, 22));
        shadingModeFlat->setChecked(true);
        shadingModeSmooth = new QRadioButton(shadingGroupBox);
        shadingModeSmooth->setObjectName(QString::fromUtf8("shadingModeSmooth"));
        shadingModeSmooth->setGeometry(QRect(0, 60, 140, 22));

        settingsLayout->addWidget(shadingGroupBox);

        settingsDock->setWidget(settingsDockContent);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), settingsDock);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionQuit);

        retranslateUi(MainWindow);
        QObject::connect(lightingEnabled, SIGNAL(toggled(bool)), shadingGroupBox, SLOT(setEnabled(bool)));
        QObject::connect(actionQuit, SIGNAL(triggered()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "[Lab04] OpenGL II", 0, QApplication::UnicodeUTF8));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", 0, QApplication::UnicodeUTF8));
        actionQuit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        fillModeGroupBox->setTitle(QApplication::translate("MainWindow", "Fill Mode", 0, QApplication::UnicodeUTF8));
        fillModePoints->setText(QApplication::translate("MainWindow", "Points", 0, QApplication::UnicodeUTF8));
        fillModeShaded->setText(QApplication::translate("MainWindow", "Shaded", 0, QApplication::UnicodeUTF8));
        fillModeWireframe->setText(QApplication::translate("MainWindow", "Wireframe", 0, QApplication::UnicodeUTF8));
        lightingGroupBox->setTitle(QApplication::translate("MainWindow", "Lighting", 0, QApplication::UnicodeUTF8));
        lightingEnabled->setText(QApplication::translate("MainWindow", "Use Lighting", 0, QApplication::UnicodeUTF8));
        shadingGroupBox->setTitle(QApplication::translate("MainWindow", "Shading", 0, QApplication::UnicodeUTF8));
        shadingModeFlat->setText(QApplication::translate("MainWindow", "Flat", 0, QApplication::UnicodeUTF8));
        shadingModeSmooth->setText(QApplication::translate("MainWindow", "Smooth", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
