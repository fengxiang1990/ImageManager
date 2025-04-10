#ifndef IMAGEPREVIEW_H
#define IMAGEPREVIEW_H

#include <QDialog>
#include <opencv2/opencv.hpp> // 添加 OpenCV 头文件

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

private:
    QString g_filePath;
    void updatePixmap();
    void restoreGeometryDelayed();
    std::mutex imageMutex; // 确保线程安全
    Ui::ImagePreview *ui;
    QPixmap m_pixmap;
    cv::Mat m_image; // 添加 cv::Mat 成员变量存储原始图像
    QRect m_normalGeometry; // 保存正常状态的几何信息
    bool isRestoring = false; // 正在还原中
    int m_brightness = 0; // 亮度
    int m_contrast = 0; // 对比度
    int m_saturation = 0; // 饱和度
};

#endif // IMAGEPREVIEW_H