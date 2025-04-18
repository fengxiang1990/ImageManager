#ifndef IMAGEPREVIEW_H
#define IMAGEPREVIEW_H

#include <QDialog>
#include <opencv2/opencv.hpp> // 添加 OpenCV 头文件
#include <QLabel>
#include <QPointer>
#include <QFuture>
#include <QFutureWatcher>

QT_BEGIN_NAMESPACE
namespace Ui { class ImagePreview; }
QT_END_NAMESPACE

class ImagePreview : public QDialog
{
Q_OBJECT

public:
    explicit ImagePreview(const QString &filePath, QWidget *parent = nullptr);
    ~ImagePreview();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void changeEvent(QEvent *event) override;


signals:
    void imageSaved(const QString &filePath);  // 保存成功的信号
    void imageDeleted(const QString &filePath);  // 删除成功的信号

private slots:
    void onSaveButtonClicked();
    void onSaveAsButtonClicked();
    void onDeleteButtonClicked();
    void onBrightnessChanged(int value);
    void onContrastChanged(int value);
    void onSaturationChanged(int value);
    void onExposureChanged(int value);       // 曝光度
    void onClarityChanged(int value);        // 清晰度
    void onTemperatureChanged(int value);    // 色温
    void onSharpenChanged(int value);     // 锐化

private:
    QFuture<void> future; // 保存后台任务的引用
    QFutureWatcher<void> watcher; // 监控后台任务的状态
    QPointer<QLabel> imageLabelPtr; // 使用 QPointer 包装 ui->imageLabel
    QString g_filePath;
    void updatePixmap();
    void restoreGeometryDelayed();
    cv::Mat processImage();
    std::mutex imageMutex; // 确保线程安全
    Ui::ImagePreview *ui;
    QPixmap m_pixmap;
    cv::Mat m_image; // 添加 cv::Mat 成员变量存储原始图像
    QRect m_normalGeometry; // 保存正常状态的几何信息
    bool isRestoring = false; // 正在还原中
    int m_brightness = 0; // 亮度
    int m_contrast = 0; // 对比度
    int m_saturation = 0; // 饱和度
    int m_exposure = 0;     // 曝光度
    int m_clarity = 0;      // 清晰度
    int m_temperature = 0;  // 色温
    int m_sharpen = 0;      // 锐化
};


#endif // IMAGEPREVIEW_H