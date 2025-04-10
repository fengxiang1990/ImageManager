/********************************************************************************
** Form generated from reading UI file 'imagepreview.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGEPREVIEW_H
#define UI_IMAGEPREVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ImagePreview
{
public:
    QHBoxLayout *mainLayout;
    QLabel *imageLabel;
    QVBoxLayout *controlLayout;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *brightnessLayout;
    QLabel *brightnessLabel;
    QSlider *brightnessSlider;
    QSpacerItem *spacer1;
    QHBoxLayout *contrastLayout;
    QLabel *contrastLabel;
    QSlider *contrastSlider;
    QSpacerItem *spacer2;
    QHBoxLayout *saturationLayout;
    QLabel *saturationLabel;
    QSlider *saturationSlider;
    QSpacerItem *verticalSpacer_2;
    QPushButton *saveButton;
    QPushButton *saveAsButton;
    QPushButton *deleteButton;

    void setupUi(QDialog *ImagePreview)
    {
        if (ImagePreview->objectName().isEmpty())
            ImagePreview->setObjectName("ImagePreview");
        ImagePreview->setWindowModality(Qt::WindowModality::WindowModal);
        ImagePreview->resize(1280, 800);
        mainLayout = new QHBoxLayout(ImagePreview);
        mainLayout->setObjectName("mainLayout");
        imageLabel = new QLabel(ImagePreview);
        imageLabel->setObjectName("imageLabel");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(3);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(imageLabel->sizePolicy().hasHeightForWidth());
        imageLabel->setSizePolicy(sizePolicy);
        imageLabel->setMinimumSize(QSize(720, 480));
        imageLabel->setMaximumSize(QSize(2200, 1440));
        imageLabel->setStyleSheet(QString::fromUtf8("border: 2px solid #D3D3D3;\n"
"padding: 5px;"));
        imageLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        mainLayout->addWidget(imageLabel);

        controlLayout = new QVBoxLayout();
        controlLayout->setSpacing(5);
        controlLayout->setObjectName("controlLayout");
        controlLayout->setSizeConstraint(QLayout::SizeConstraint::SetDefaultConstraint);
        controlLayout->setContentsMargins(10, -1, -1, -1);
        verticalSpacer_3 = new QSpacerItem(20, 50, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);

        controlLayout->addItem(verticalSpacer_3);

        brightnessLayout = new QHBoxLayout();
        brightnessLayout->setSpacing(6);
        brightnessLayout->setObjectName("brightnessLayout");
        brightnessLabel = new QLabel(ImagePreview);
        brightnessLabel->setObjectName("brightnessLabel");
        brightnessLabel->setMinimumSize(QSize(60, 0));
        brightnessLabel->setStyleSheet(QString::fromUtf8(""));

        brightnessLayout->addWidget(brightnessLabel);

        brightnessSlider = new QSlider(ImagePreview);
        brightnessSlider->setObjectName("brightnessSlider");
        brightnessSlider->setMinimum(-100);
        brightnessSlider->setMaximum(100);
        brightnessSlider->setOrientation(Qt::Orientation::Horizontal);

        brightnessLayout->addWidget(brightnessSlider);


        controlLayout->addLayout(brightnessLayout);

        spacer1 = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);

        controlLayout->addItem(spacer1);

        contrastLayout = new QHBoxLayout();
        contrastLayout->setSpacing(6);
        contrastLayout->setObjectName("contrastLayout");
        contrastLabel = new QLabel(ImagePreview);
        contrastLabel->setObjectName("contrastLabel");
        contrastLabel->setMinimumSize(QSize(60, 0));

        contrastLayout->addWidget(contrastLabel);

        contrastSlider = new QSlider(ImagePreview);
        contrastSlider->setObjectName("contrastSlider");
        contrastSlider->setMinimum(-100);
        contrastSlider->setMaximum(100);
        contrastSlider->setOrientation(Qt::Orientation::Horizontal);

        contrastLayout->addWidget(contrastSlider);


        controlLayout->addLayout(contrastLayout);

        spacer2 = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);

        controlLayout->addItem(spacer2);

        saturationLayout = new QHBoxLayout();
        saturationLayout->setSpacing(6);
        saturationLayout->setObjectName("saturationLayout");
        saturationLabel = new QLabel(ImagePreview);
        saturationLabel->setObjectName("saturationLabel");
        saturationLabel->setMinimumSize(QSize(60, 0));

        saturationLayout->addWidget(saturationLabel);

        saturationSlider = new QSlider(ImagePreview);
        saturationSlider->setObjectName("saturationSlider");
        saturationSlider->setMinimum(-100);
        saturationSlider->setMaximum(100);
        saturationSlider->setOrientation(Qt::Orientation::Horizontal);

        saturationLayout->addWidget(saturationSlider);


        controlLayout->addLayout(saturationLayout);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        controlLayout->addItem(verticalSpacer_2);

        saveButton = new QPushButton(ImagePreview);
        saveButton->setObjectName("saveButton");

        controlLayout->addWidget(saveButton);

        saveAsButton = new QPushButton(ImagePreview);
        saveAsButton->setObjectName("saveAsButton");

        controlLayout->addWidget(saveAsButton);

        deleteButton = new QPushButton(ImagePreview);
        deleteButton->setObjectName("deleteButton");

        controlLayout->addWidget(deleteButton);


        mainLayout->addLayout(controlLayout);


        retranslateUi(ImagePreview);

        QMetaObject::connectSlotsByName(ImagePreview);
    } // setupUi

    void retranslateUi(QDialog *ImagePreview)
    {
        ImagePreview->setWindowTitle(QCoreApplication::translate("ImagePreview", "\345\233\276\347\211\207\351\242\204\350\247\210", nullptr));
        brightnessLabel->setText(QCoreApplication::translate("ImagePreview", "\344\272\256\345\272\246", nullptr));
        contrastLabel->setText(QCoreApplication::translate("ImagePreview", "\345\257\271\346\257\224\345\272\246", nullptr));
        saturationLabel->setText(QCoreApplication::translate("ImagePreview", "\351\245\261\345\222\214\345\272\246", nullptr));
        saveButton->setText(QCoreApplication::translate("ImagePreview", "\344\277\235\345\255\230", nullptr));
        saveAsButton->setText(QCoreApplication::translate("ImagePreview", "\345\217\246\345\255\230\344\270\272", nullptr));
        deleteButton->setText(QCoreApplication::translate("ImagePreview", "\345\210\240\351\231\244", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ImagePreview: public Ui_ImagePreview {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGEPREVIEW_H
