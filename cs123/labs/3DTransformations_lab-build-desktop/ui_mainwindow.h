/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Fri Sep 28 16:00:44 2012
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
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
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
    QCheckBox *showIntersectSpheres;
    QLabel *scoreLabel;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1084, 513);
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        centralWidget = new GLWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centralWidget->setFocusPolicy(Qt::StrongFocus);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1084, 25));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        MainWindow->setMenuBar(menuBar);
        settingsDock = new QDockWidget(MainWindow);
        settingsDock->setObjectName(QString::fromUtf8("settingsDock"));
        settingsDock->setMinimumSize(QSize(483, 472));
        settingsDock->setFeatures(QDockWidget::DockWidgetMovable);
        settingsDockContent = new QWidget();
        settingsDockContent->setObjectName(QString::fromUtf8("settingsDockContent"));
        settingsDockContent->setAutoFillBackground(false);
        settingsLayout = new QVBoxLayout(settingsDockContent);
        settingsLayout->setSpacing(6);
        settingsLayout->setContentsMargins(11, 11, 11, 11);
        settingsLayout->setObjectName(QString::fromUtf8("settingsLayout"));
        showIntersectSpheres = new QCheckBox(settingsDockContent);
        showIntersectSpheres->setObjectName(QString::fromUtf8("showIntersectSpheres"));
        showIntersectSpheres->setEnabled(true);
        showIntersectSpheres->setMinimumSize(QSize(207, 22));

        settingsLayout->addWidget(showIntersectSpheres);

        scoreLabel = new QLabel(settingsDockContent);
        scoreLabel->setObjectName(QString::fromUtf8("scoreLabel"));
        scoreLabel->setMinimumSize(QSize(465, 304));
        scoreLabel->setMaximumSize(QSize(465, 304));

        settingsLayout->addWidget(scoreLabel);

        label = new QLabel(settingsDockContent);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(465, 19));

        settingsLayout->addWidget(label);

        label_2 = new QLabel(settingsDockContent);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(465, 18));

        settingsLayout->addWidget(label_2);

        label_3 = new QLabel(settingsDockContent);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(465, 19));

        settingsLayout->addWidget(label_3);

        label_4 = new QLabel(settingsDockContent);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        settingsLayout->addWidget(label_4);

        settingsDock->setWidget(settingsDockContent);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), settingsDock);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionQuit);

        retranslateUi(MainWindow);
        QObject::connect(actionQuit, SIGNAL(triggered()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "[Lab02] 3D Transformations", 0, QApplication::UnicodeUTF8));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", 0, QApplication::UnicodeUTF8));
        actionQuit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        showIntersectSpheres->setText(QApplication::translate("MainWindow", "Intersection Sphere?", 0, QApplication::UnicodeUTF8));
        scoreLabel->setText(QApplication::translate("MainWindow", "Score: 0", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Click on the scene to enter first person mode", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Hit esc to exit first person mode", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Move with WASD and look around with the mouse", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Click to shoot in first person mode", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
