#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QFutureWatcher>
#include <QList>
#include <QPixmap>
#include <QMetaObject>
#include <QLabel>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void progressValueChanged(int value);
    void imagesFound(const QList<QPair<QString, QPixmap>> &images);

private slots:
    void onImagesLoaded();
    void handleProgress(int value);
    void handleImagesFound(const QList<QPair<QString, QPixmap>> &images);
    void onImageSelectionChanged(); // 新增：处理图片选中变化
    void onImageDoubleClicked(QListWidgetItem *item);
    void onImageSaved(const QString &filePath);  // 处理图片保存的槽函数
    void onImageDeleted(const QString &filePath);  // 处理图片删除的槽函数

private:
    void selectCDrive();
    void initDiskList();
    void loadImagesAsync(const QString &path);
    bool validateImageSize(const QString &path) const;

    Ui::MainWindow *ui;
    QFileSystemModel *diskModel;
    QFutureWatcher<void> imageWatcher;
    QHash<QString, QPixmap> thumbnailCache;
    int totalItems = 0;
    int processedItems = 0;
    int validFilesCount = 0;
    QStringList failedFiles;

    // 新增：存储图片路径和对应的 QListWidgetItem
    QHash<QListWidgetItem*, QString> itemToPathMap;

    QListWidgetItem *getItemForFilePath(const QString &filePath);
};

#endif // MAINWINDOW_H
