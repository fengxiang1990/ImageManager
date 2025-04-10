#include "imagepreview.h"
#include "ui_imagepreview.h"

#include <QResizeEvent>
#include <QTimer>
#include <opencv2/imgproc.hpp>
#include <QMessageBox>
#include <QFileDialog>



ImagePreview::ImagePreview(const QString &filePath, QWidget *parent)
        : QDialog(parent)
        , ui(new Ui::ImagePreview)
{
    ui->setupUi(this);

    g_filePath = filePath;

    // 设置窗口标志
    setWindowFlags(Qt::Window | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);

    // 设置为模态对话框
    setModal(true);

    // 初始大小
    resize(1280, 800);

    // 记录初始几何信息
    m_normalGeometry = geometry();
    qDebug() << "Initial m_normalGeometry:" << m_normalGeometry;

    // 加载图片
    m_pixmap = QPixmap(filePath);
    if (m_pixmap.isNull()) {
        qDebug() << "Failed to load image from path:" << filePath;
        return;
    }

    // 将 QPixmap 转换为 cv::Mat（深拷贝，防止无效指针）
    QImage qImage = m_pixmap.toImage().convertToFormat(QImage::Format_RGB888);
    cv::Mat tempMat(qImage.height(), qImage.width(), CV_8UC3, qImage.bits(), qImage.bytesPerLine());
    m_image = tempMat.clone();  // 关键：深拷贝
    cv::cvtColor(m_image, m_image, cv::COLOR_RGB2BGR);

    setWindowTitle("图片预览");

    // 连接滑动条信号与槽
    connect(ui->brightnessSlider, &QSlider::valueChanged, this, &ImagePreview::onBrightnessChanged);
    connect(ui->contrastSlider, &QSlider::valueChanged, this, &ImagePreview::onContrastChanged);
    connect(ui->saturationSlider, &QSlider::valueChanged, this, &ImagePreview::onSaturationChanged);

    //点击事件
    connect(ui->saveButton, &QPushButton::clicked, this, &ImagePreview::onSaveButtonClicked);
    connect(ui->saveAsButton, &QPushButton::clicked, this, &ImagePreview::onSaveAsButtonClicked);
    connect(ui->deleteButton, &QPushButton::clicked, this, &ImagePreview::onDeleteButtonClicked);

    updatePixmap();
}

ImagePreview::~ImagePreview()
{
    delete ui;
}

void ImagePreview::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
// 仅在正常状态且不在还原过程中更新 m_normalGeometry
    if (windowState() == Qt::WindowNoState && !isRestoring) {
        QRect currentGeometry = geometry();
        // 如果 m_normalGeometry 尚未初始化（宽高为 0），直接赋值
        if (m_normalGeometry.width() == 0 || m_normalGeometry.height() == 0) {
            m_normalGeometry = currentGeometry;
            qDebug() << "Updated m_normalGeometry in resizeEvent (initial):" << m_normalGeometry;
        } else {
            // 比较宽高，只保留宽高较小的几何信息
            if (currentGeometry.width() <= m_normalGeometry.width() &&
                currentGeometry.height() <= m_normalGeometry.height()) {
                m_normalGeometry = currentGeometry;
                qDebug() << "Updated m_normalGeometry in resizeEvent (smaller size):" << m_normalGeometry;
            } else {
                qDebug() << "Skipped updating m_normalGeometry: current size ("
                         << currentGeometry.width() << "x" << currentGeometry.height()
                         << ") is larger than stored size ("
                         << m_normalGeometry.width() << "x" << m_normalGeometry.height() << ")";
            }
        }
    }
    if (!m_image.empty()) {
        updatePixmap();
    }
}


void ImagePreview::onSaveButtonClicked()
{
    std::lock_guard<std::mutex> lock(imageMutex);
    if (m_image.empty()) {
        QMessageBox::warning(this, "错误", "无法保存：图片数据为空！");
        return;
    }

    // 将 m_image 转换为 QPixmap
    cv::Mat adjustedImage = m_image.clone();

    // 调整亮度和对比度
    double alpha = 1.0 + m_contrast / 100.0;
    double beta = m_brightness;
    adjustedImage.convertTo(adjustedImage, -1, alpha, beta);

    // 调整饱和度
    if (m_saturation != 0) {
        cv::Mat hsvImage;
        cv::cvtColor(adjustedImage, hsvImage, cv::COLOR_BGR2HSV);

        std::vector<cv::Mat> hsvChannels;
        cv::split(hsvImage, hsvChannels);

        double saturationScale = 1.0 + m_saturation / 100.0;
        hsvChannels[1] = hsvChannels[1] * saturationScale;

        cv::merge(hsvChannels, hsvImage);
        cv::cvtColor(hsvImage, adjustedImage, cv::COLOR_HSV2BGR);
    }

    // 将处理后的 OpenCV 图像转换为 QPixmap
    cv::cvtColor(adjustedImage, adjustedImage, cv::COLOR_BGR2RGB);
    QImage qImage(adjustedImage.data, adjustedImage.cols, adjustedImage.rows, adjustedImage.step, QImage::Format_RGB888);
    QPixmap pixmap = QPixmap::fromImage(qImage);

    // 保存为原路径
    if (pixmap.save(g_filePath)) {
        QMessageBox::information(this, "保存成功", "图片已成功保存！");
        emit imageSaved(g_filePath);  // 发射图片已保存的信号
    } else {
        QMessageBox::warning(this, "保存失败", "无法保存图片！");
    }
}

void ImagePreview::onSaveAsButtonClicked()
{
    std::lock_guard<std::mutex> lock(imageMutex);
    if (m_image.empty()) {
        QMessageBox::warning(this, "错误", "无法另存为：图片数据为空！");
        return;
    }

    QString newFilePath = QFileDialog::getSaveFileName(this, "另存为", "", "PNG (*.png);;JPEG (*.jpg *.jpeg);;BMP (*.bmp)");
    if (newFilePath.isEmpty()) {
        return;
    }

    // 将 m_image 转换为 QPixmap
    cv::Mat adjustedImage = m_image.clone();

    // 调整亮度和对比度
    double alpha = 1.0 + m_contrast / 100.0;
    double beta = m_brightness;
    adjustedImage.convertTo(adjustedImage, -1, alpha, beta);

    // 调整饱和度
    if (m_saturation != 0) {
        cv::Mat hsvImage;
        cv::cvtColor(adjustedImage, hsvImage, cv::COLOR_BGR2HSV);

        std::vector<cv::Mat> hsvChannels;
        cv::split(hsvImage, hsvChannels);

        double saturationScale = 1.0 + m_saturation / 100.0;
        hsvChannels[1] = hsvChannels[1] * saturationScale;

        cv::merge(hsvChannels, hsvImage);
        cv::cvtColor(hsvImage, adjustedImage, cv::COLOR_HSV2BGR);
    }

    // 将处理后的 OpenCV 图像转换为 QPixmap
    cv::cvtColor(adjustedImage, adjustedImage, cv::COLOR_BGR2RGB);
    QImage qImage(adjustedImage.data, adjustedImage.cols, adjustedImage.rows, adjustedImage.step, QImage::Format_RGB888);
    QPixmap pixmap = QPixmap::fromImage(qImage);

    // 保存为新路径
    if (pixmap.save(newFilePath)) {
        QMessageBox::information(this, "另存为成功", "图片已成功另存为！");
    } else {
        QMessageBox::warning(this, "另存为失败", "无法保存图片！");
    }
}

void ImagePreview::onDeleteButtonClicked()
{
    int ret = QMessageBox::warning(this, "删除确认", "确定要删除这张图片吗？", QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        if (QFile::remove(g_filePath)) {
            QMessageBox::information(this, "删除成功", "图片已被删除！");
            emit imageDeleted(g_filePath);  // 发射图片已保存的信号
            close();
        } else {
            QMessageBox::warning(this, "删除失败", "无法删除图片！");
        }
    }
}

void ImagePreview::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::WindowStateChange) {
        QWindowStateChangeEvent *stateEvent = static_cast<QWindowStateChangeEvent *>(event);
        Qt::WindowStates oldState = stateEvent->oldState();
        Qt::WindowStates newState = windowState();

        if (!(oldState & Qt::WindowMaximized) && (newState & Qt::WindowMaximized)) {
            // 进入最大化前保存正常状态的几何信息
            qDebug() << "Entering Maximized. Stored geometry:" << m_normalGeometry;
        } else if ((oldState & Qt::WindowMaximized) && !(newState & Qt::WindowMaximized)) {
            // 退出最大化时恢复几何信息
            qDebug() << "Leaving Maximized. Trying to restore geometry to:" << m_normalGeometry;
            isRestoring = true;
            restoreGeometryDelayed();
        }
    }

    QDialog::changeEvent(event);
}

void ImagePreview::restoreGeometryDelayed()
{
    // 延迟设置几何信息，确保窗口状态切换完成
    QTimer::singleShot(0, [this] {
        // 强制清除最小尺寸限制
        setMinimumSize(0, 0);
        setWindowState(Qt::WindowNoState);
        setGeometry(m_normalGeometry);
        qDebug() << "Restored geometry to:" << m_normalGeometry;
        qDebug() << "Actual geometry after restore:" << geometry();
        isRestoring = false; // 还原完成
        updatePixmap(); // 调整图片大小适应窗口
    });
}

void ImagePreview::onBrightnessChanged(int value)
{
    m_brightness = value;
    updatePixmap();
}

void ImagePreview::onContrastChanged(int value)
{
    m_contrast = value;
    updatePixmap();
}

void ImagePreview::onSaturationChanged(int value)
{
    m_saturation = value;
    updatePixmap();
}

void ImagePreview::updatePixmap()
{
    std::lock_guard<std::mutex> lock(imageMutex); // 确保线程安全

    if (m_image.empty()) {
        qDebug() << "m_image is empty in updatePixmap";
        return;
    }

    cv::Mat adjustedImage;
    try {
        adjustedImage = m_image.clone();
    } catch (const cv::Exception &e) {
        qDebug() << "Failed to clone m_image:" << e.what();
        return;
    }

    // 调整亮度和对比度
    double alpha = 1.0 + m_contrast / 100.0;
    double beta = m_brightness;
    adjustedImage.convertTo(adjustedImage, -1, alpha, beta);

    // 调整饱和度
    if (m_saturation != 0) {
        cv::Mat hsvImage;
        cv::cvtColor(adjustedImage, hsvImage, cv::COLOR_BGR2HSV);

        std::vector<cv::Mat> hsvChannels;
        cv::split(hsvImage, hsvChannels);

        double saturationScale = 1.0 + m_saturation / 100.0;
        hsvChannels[1] = hsvChannels[1] * saturationScale;

        cv::merge(hsvChannels, hsvImage);
        cv::cvtColor(hsvImage, adjustedImage, cv::COLOR_HSV2BGR);
    }

    // 将 OpenCV Mat 转换为 QPixmap
    cv::cvtColor(adjustedImage, adjustedImage, cv::COLOR_BGR2RGB);
    QImage qImage(adjustedImage.data, adjustedImage.cols, adjustedImage.rows, adjustedImage.step, QImage::Format_RGB888);
    QPixmap pixmap = QPixmap::fromImage(qImage);

    // 适应窗口大小，保持宽高比
    QPixmap scaled = pixmap.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->imageLabel->setPixmap(scaled);
}
