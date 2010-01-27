/********************************************************************************
** Form generated from reading UI file 'UserInterface.ui'
**
** Created: Wed 27. Jan 02:45:38 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERINTERFACE_H
#define UI_USERINTERFACE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UserInterfaceClass
{
public:
    QAction *actionOpen;
    QAction *actionQuit;
    QWidget *centralWidget;
    QWidget *layoutWidget;
    QVBoxLayout *glWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;

    void setupUi(QMainWindow *UserInterfaceClass)
    {
        if (UserInterfaceClass->objectName().isEmpty())
            UserInterfaceClass->setObjectName(QString::fromUtf8("UserInterfaceClass"));
        UserInterfaceClass->resize(1280, 720);
        UserInterfaceClass->setMinimumSize(QSize(1280, 720));
        UserInterfaceClass->setMaximumSize(QSize(1280, 720));
        actionOpen = new QAction(UserInterfaceClass);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionQuit = new QAction(UserInterfaceClass);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        centralWidget = new QWidget(UserInterfaceClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        centralWidget->setMinimumSize(QSize(1280, 720));
        centralWidget->setMaximumSize(QSize(1280, 720));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 0, 1282, 1282));
        glWidget = new QVBoxLayout(layoutWidget);
        glWidget->setSpacing(0);
        glWidget->setContentsMargins(11, 11, 11, 11);
        glWidget->setObjectName(QString::fromUtf8("glWidget"));
        glWidget->setSizeConstraint(QLayout::SetDefaultConstraint);
        glWidget->setContentsMargins(0, 0, 0, 0);
        UserInterfaceClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(UserInterfaceClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1280, 18));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        UserInterfaceClass->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionQuit);

        retranslateUi(UserInterfaceClass);
        QObject::connect(actionQuit, SIGNAL(activated()), UserInterfaceClass, SLOT(close()));

        QMetaObject::connectSlotsByName(UserInterfaceClass);
    } // setupUi

    void retranslateUi(QMainWindow *UserInterfaceClass)
    {
        UserInterfaceClass->setWindowTitle(QApplication::translate("UserInterfaceClass", "ExplodedVolume", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("UserInterfaceClass", "Open...", 0, QApplication::UnicodeUTF8));
        actionQuit->setText(QApplication::translate("UserInterfaceClass", "Quit", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("UserInterfaceClass", "File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UserInterfaceClass: public Ui_UserInterfaceClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERINTERFACE_H
