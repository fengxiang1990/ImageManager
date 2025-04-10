/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *hboxLayout;
    QListView *listView;
    QWidget *contentWidget;
    QVBoxLayout *vboxLayout;
    QListWidget *imageGridWidget;
    QProgressBar *progressBar;
    QWidget *infoWidget;
    QVBoxLayout *infoLayout;
    QLabel *pathLabel;
    QLabel *creationTimeLabel;
    QLabel *sizeLabel;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        MainWindow->setStyleSheet(QString::fromUtf8("\n"
"    QProgressBar {\n"
"        border: 2px solid #5A5A5A;\n"
"        border-radius: 5px;\n"
"        text-align: center;\n"
"    }\n"
"    QProgressBar::chunk {\n"
"        background-color: #4CAF50;\n"
"        width: 10px;\n"
"    }\n"
"   "));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        hboxLayout = new QHBoxLayout(centralwidget);
        hboxLayout->setObjectName("hboxLayout");
        listView = new QListView(centralwidget);
        listView->setObjectName("listView");
        listView->setMaximumWidth(150);

        hboxLayout->addWidget(listView);

        contentWidget = new QWidget(centralwidget);
        contentWidget->setObjectName("contentWidget");
        vboxLayout = new QVBoxLayout(contentWidget);
        vboxLayout->setObjectName("vboxLayout");
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        imageGridWidget = new QListWidget(contentWidget);
        imageGridWidget->setObjectName("imageGridWidget");
        imageGridWidget->setIconSize(QSize(128, 128));
        imageGridWidget->setGridSize(QSize(150, 150));
        imageGridWidget->setViewMode(QListView::IconMode);
        imageGridWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        imageGridWidget->setStyleSheet(QString::fromUtf8("\n"
"           QListWidget::item {\n"
"               border: 2px solid transparent; /* \351\273\230\350\256\244\346\227\240\350\276\271\346\241\206 */\n"
"               padding: 2px;\n"
"           }\n"
"           QListWidget::item:selected {\n"
"               border: 2px solid #87CEEB; /* \346\267\241\350\223\235\350\211\262\350\276\271\346\241\206 */\n"
"               background: rgba(135, 206, 235, 50); /* \346\267\241\350\223\235\350\211\262\350\203\214\346\231\257\357\274\214\345\215\212\351\200\217\346\230\216 */\n"
"           }\n"
"          "));

        vboxLayout->addWidget(imageGridWidget);

        progressBar = new QProgressBar(contentWidget);
        progressBar->setObjectName("progressBar");
        progressBar->setMinimum(0);
        progressBar->setMaximum(100);
        progressBar->setTextVisible(true);
        progressBar->setAlignment(Qt::AlignCenter);
        progressBar->setVisible(false);

        vboxLayout->addWidget(progressBar);

        infoWidget = new QWidget(contentWidget);
        infoWidget->setObjectName("infoWidget");
        infoLayout = new QVBoxLayout(infoWidget);
        infoLayout->setObjectName("infoLayout");
        infoLayout->setContentsMargins(0, 0, 0, 0);
        pathLabel = new QLabel(infoWidget);
        pathLabel->setObjectName("pathLabel");

        infoLayout->addWidget(pathLabel);

        creationTimeLabel = new QLabel(infoWidget);
        creationTimeLabel->setObjectName("creationTimeLabel");

        infoLayout->addWidget(creationTimeLabel);

        sizeLabel = new QLabel(infoWidget);
        sizeLabel->setObjectName("sizeLabel");

        infoLayout->addWidget(sizeLabel);


        vboxLayout->addWidget(infoWidget);


        hboxLayout->addWidget(contentWidget);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Image Browser", nullptr));
        pathLabel->setText(QCoreApplication::translate("MainWindow", "\350\267\257\345\276\204: \346\234\252\351\200\211\346\213\251\345\233\276\347\211\207", nullptr));
        creationTimeLabel->setText(QCoreApplication::translate("MainWindow", "\345\210\233\345\273\272\346\227\266\351\227\264: \346\234\252\351\200\211\346\213\251\345\233\276\347\211\207", nullptr));
        sizeLabel->setText(QCoreApplication::translate("MainWindow", "\345\260\272\345\257\270: \346\234\252\351\200\211\346\213\251\345\233\276\347\211\207", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
