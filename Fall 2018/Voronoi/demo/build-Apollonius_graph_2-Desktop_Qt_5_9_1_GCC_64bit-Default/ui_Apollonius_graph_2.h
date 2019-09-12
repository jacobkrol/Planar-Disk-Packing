/********************************************************************************
** Form generated from reading UI file 'Apollonius_graph_2.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_APOLLONIUS_GRAPH_2_H
#define UI_APOLLONIUS_GRAPH_2_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Apollonius_graph_2
{
public:
    QAction *actionAbout;
    QAction *actionAboutCGAL;
    QAction *actionQuit;
    QAction *actionInsertRandomPoints;
    QAction *actionClear;
    QAction *actionLoadPoints;
    QAction *actionSavePoints;
    QAction *actionRecenter;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QGraphicsView *graphicsView;
    QStatusBar *statusbar;
    QToolBar *fileToolBar;
    QToolBar *toolBar;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuTools;

    void setupUi(QMainWindow *Apollonius_graph_2)
    {
        if (Apollonius_graph_2->objectName().isEmpty())
            Apollonius_graph_2->setObjectName(QStringLiteral("Apollonius_graph_2"));
        Apollonius_graph_2->resize(751, 430);
        QIcon icon;
        icon.addFile(QStringLiteral(":/cgal/logos/cgal_icon"), QSize(), QIcon::Normal, QIcon::Off);
        Apollonius_graph_2->setWindowIcon(icon);
        actionAbout = new QAction(Apollonius_graph_2);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionAboutCGAL = new QAction(Apollonius_graph_2);
        actionAboutCGAL->setObjectName(QStringLiteral("actionAboutCGAL"));
        actionQuit = new QAction(Apollonius_graph_2);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        actionInsertRandomPoints = new QAction(Apollonius_graph_2);
        actionInsertRandomPoints->setObjectName(QStringLiteral("actionInsertRandomPoints"));
        actionClear = new QAction(Apollonius_graph_2);
        actionClear->setObjectName(QStringLiteral("actionClear"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/cgal/fileToolbar/fileNew.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionClear->setIcon(icon1);
        actionLoadPoints = new QAction(Apollonius_graph_2);
        actionLoadPoints->setObjectName(QStringLiteral("actionLoadPoints"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/cgal/fileToolbar/fileOpen.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLoadPoints->setIcon(icon2);
        actionSavePoints = new QAction(Apollonius_graph_2);
        actionSavePoints->setObjectName(QStringLiteral("actionSavePoints"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/cgal/fileToolbar/fileSave.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSavePoints->setIcon(icon3);
        actionRecenter = new QAction(Apollonius_graph_2);
        actionRecenter->setObjectName(QStringLiteral("actionRecenter"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/cgal/Input/zoom-best-fit"), QSize(), QIcon::Normal, QIcon::Off);
        actionRecenter->setIcon(icon4);
        centralwidget = new QWidget(Apollonius_graph_2);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setFocusPolicy(Qt::StrongFocus);
        graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        graphicsView->setTransformationAnchor(QGraphicsView::NoAnchor);

        gridLayout->addWidget(graphicsView, 0, 0, 1, 1);

        Apollonius_graph_2->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(Apollonius_graph_2);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        Apollonius_graph_2->setStatusBar(statusbar);
        fileToolBar = new QToolBar(Apollonius_graph_2);
        fileToolBar->setObjectName(QStringLiteral("fileToolBar"));
        Apollonius_graph_2->addToolBar(Qt::TopToolBarArea, fileToolBar);
        toolBar = new QToolBar(Apollonius_graph_2);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        Apollonius_graph_2->addToolBar(Qt::TopToolBarArea, toolBar);
        menubar = new QMenuBar(Apollonius_graph_2);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 751, 19));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuTools = new QMenu(menubar);
        menuTools->setObjectName(QStringLiteral("menuTools"));
        Apollonius_graph_2->setMenuBar(menubar);

        fileToolBar->addAction(actionClear);
        fileToolBar->addAction(actionLoadPoints);
        fileToolBar->addAction(actionSavePoints);
        toolBar->addAction(actionRecenter);
        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menubar->addAction(menuTools->menuAction());
        menuFile->addSeparator();
        menuFile->addAction(actionClear);
        menuFile->addAction(actionLoadPoints);
        menuFile->addAction(actionSavePoints);
        menuFile->addSeparator();
        menuFile->addAction(actionQuit);
        menuEdit->addAction(actionInsertRandomPoints);
        menuTools->addSeparator();
        menuTools->addAction(actionRecenter);

        retranslateUi(Apollonius_graph_2);

        QMetaObject::connectSlotsByName(Apollonius_graph_2);
    } // setupUi

    void retranslateUi(QMainWindow *Apollonius_graph_2)
    {
        Apollonius_graph_2->setWindowTitle(QApplication::translate("Apollonius_graph_2", "CGAL Apollonius Graph", Q_NULLPTR));
        actionAbout->setText(QApplication::translate("Apollonius_graph_2", "&About", Q_NULLPTR));
        actionAboutCGAL->setText(QApplication::translate("Apollonius_graph_2", "About &CGAL", Q_NULLPTR));
        actionQuit->setText(QApplication::translate("Apollonius_graph_2", "&Quit", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionQuit->setShortcut(QApplication::translate("Apollonius_graph_2", "Ctrl+Q", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionInsertRandomPoints->setText(QApplication::translate("Apollonius_graph_2", "&Insert random points", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionInsertRandomPoints->setShortcut(QApplication::translate("Apollonius_graph_2", "Ctrl+I", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionClear->setText(QApplication::translate("Apollonius_graph_2", "&Clear", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionClear->setShortcut(QApplication::translate("Apollonius_graph_2", "Ctrl+C", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionLoadPoints->setText(QApplication::translate("Apollonius_graph_2", "&Load Points...", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionLoadPoints->setShortcut(QApplication::translate("Apollonius_graph_2", "Ctrl+L", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionSavePoints->setText(QApplication::translate("Apollonius_graph_2", "&Save Points...", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionSavePoints->setShortcut(QApplication::translate("Apollonius_graph_2", "Ctrl+S", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionRecenter->setText(QApplication::translate("Apollonius_graph_2", "Re&center the viewport", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionRecenter->setShortcut(QApplication::translate("Apollonius_graph_2", "Ctrl+R", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        fileToolBar->setWindowTitle(QApplication::translate("Apollonius_graph_2", "File Tools", Q_NULLPTR));
        toolBar->setWindowTitle(QApplication::translate("Apollonius_graph_2", "Visualization Tools", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("Apollonius_graph_2", "&File", Q_NULLPTR));
        menuEdit->setTitle(QApplication::translate("Apollonius_graph_2", "&Edit", Q_NULLPTR));
        menuTools->setTitle(QApplication::translate("Apollonius_graph_2", "&Tools", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Apollonius_graph_2: public Ui_Apollonius_graph_2 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_APOLLONIUS_GRAPH_2_H
