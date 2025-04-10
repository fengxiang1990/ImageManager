#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ImagePreview.h"
#include <QtConcurrent/QtConcurrent>
#include <QImageReader>
#include <QDir>
#include <QDebug>
#include <QElapsedTimer>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , diskModel(new QFileSystemModel(this))
{
    ui->setupUi(this);
    // 设置窗口标题为“相册”
    setWindowTitle("相册");
    // 设置窗口图标和任务栏图标
    setWindowIcon(QIcon(":/resources/ic_icon.png")); // 确保图标文件路径正确

    // 初始化磁盘列表
    initDiskList();

    // 配置进度条
    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(false);

    // 连接信号槽
    connect(ui->listView->selectionModel(), &QItemSelectionModel::currentChanged,
            [this](const QModelIndex &index){
                if(index.isValid()) loadImagesAsync(diskModel->filePath(index));
            });
    connect(this, &MainWindow::progressValueChanged, this, &MainWindow::handleProgress);
    connect(this, &MainWindow::imagesFound, this, &MainWindow::handleImagesFound);
    connect(&imageWatcher, &QFutureWatcher<void>::finished, this, &MainWindow::onImagesLoaded);
    connect(ui->imageGridWidget, &QListWidget::itemSelectionChanged, this, &MainWindow::onImageSelectionChanged);
    // 添加双击事件
    connect(ui->imageGridWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::onImageDoubleClicked);
}

void MainWindow::onImageDoubleClicked(QListWidgetItem *item)
{
    QString filePath = itemToPathMap.value(item, "");
    if (filePath.isEmpty()) {
        return;
    }

    ImagePreview *preview = new ImagePreview(filePath, this);
    //连接信号和槽
    connect(preview, &ImagePreview::imageSaved, this, &MainWindow::onImageSaved);
    connect(preview, &ImagePreview::imageDeleted, this, &MainWindow::onImageDeleted);
    preview->exec(); // 使用模态对话框显示预览窗口
    delete preview;  // 释放内存
}

void MainWindow::initDiskList()
{
    diskModel->setFilter(QDir::Drives | QDir::NoDotAndDotDot);
    // 使用平台默认的根目录，而不是空字符串
//    diskModel->setRootPath(QDir::rootPath());

    diskModel->setRootPath(QDir::rootPath());
    ui->listView->setModel(diskModel);

    // 等待模型加载完成
    QTimer::singleShot(100, this, [this]() {
        qDebug() << "rowCount after delay:" << diskModel->rowCount();
        selectCDrive();
    });
}

void MainWindow::selectCDrive()
{
    bool cDriveFound = false;
    for (int row = 0; row < diskModel->rowCount(); ++row) {
        QModelIndex index = diskModel->index(row, 0);
        QString drivePath = diskModel->filePath(index);
        qDebug() << "drivePath:" << drivePath;
        if (drivePath.compare("C:/", Qt::CaseInsensitive) == 0 || drivePath.compare("C:\\", Qt::CaseInsensitive) == 0) {
            ui->listView->setCurrentIndex(index);
            cDriveFound = true;
            break;
        }
    }

    if (!cDriveFound && diskModel->rowCount() > 0) {
        QModelIndex firstIndex = diskModel->index(0, 0);
        ui->listView->setCurrentIndex(firstIndex);
    }
}


void MainWindow::onImageSaved(const QString &filePath)
{
    qDebug() << "imageSaved：" << filePath;
    // 执行需要的操作，例如更新界面或记录日志
    QListWidgetItem *item = getItemForFilePath(filePath);
    if (item) {
        QPixmap pixmap(filePath);
        if (!pixmap.isNull()) {
            QPixmap thumbnail = pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            item->setIcon(QIcon(thumbnail)); // Update the thumbnail
        }
    }
}

void MainWindow::onImageDeleted(const QString &filePath)
{
    qDebug() << "imageDeleted：" << filePath;
    // 执行需要的操作，例如更新界面或删除相关记录
    QListWidgetItem *item = getItemForFilePath(filePath);
    if (item) {
        delete item; // Remove the item from the grid
    }
}

QListWidgetItem* MainWindow::getItemForFilePath(const QString &filePath)
{
    for (int row = 0; row < ui->imageGridWidget->count(); ++row) {
        QListWidgetItem *item = ui->imageGridWidget->item(row);
        if (itemToPathMap.value(item, "") == filePath) {
            return item; // Return the item that corresponds to the file path
        }
    }
    return nullptr; // Return null if no item is found for the given file path
}


void MainWindow::handleProgress(int value)
{
    ui->progressBar->setValue(value);
    qDebug() << "Progress:" << value << "%";
}

void MainWindow::handleImagesFound(const QList<QPair<QString, QPixmap>> &images)
{
    qDebug() << "Received" << images.size() << "images in handleImagesFound";
    for (const auto &image : images) {
        const QString &filePath = image.first;
        qDebug() << "Adding images to UI:" << filePath;
        const QPixmap &thumbnail = image.second;
        thumbnailCache[filePath] = thumbnail;
        QListWidgetItem *item = new QListWidgetItem(
            QIcon(thumbnail),
            QFileInfo(filePath).fileName()
            );
        ui->imageGridWidget->addItem(item);
        itemToPathMap[item] = filePath;
    }
}


void MainWindow::loadImagesAsync(const QString &path)
{
    if (imageWatcher.isRunning()) {
        imageWatcher.cancel();
        imageWatcher.waitForFinished();
    }

    // 重置状态
    totalItems = 0;
    processedItems = 0;
    validFilesCount = 0;
    failedFiles.clear();
    thumbnailCache.clear();
    itemToPathMap.clear();
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(true);
    ui->imageGridWidget->clear();

    QFuture<void> future = QtConcurrent::run([=](){
        QStringList filters;
        for (const QByteArray &format : QImageReader::supportedImageFormats()) {
            filters << QString("*.%1").arg(QString(format).toLower());
            filters << QString("*.%1").arg(QString(format).toUpper());
        }
        qDebug() << "Filters:" << filters;

        // 定义要跳过的系统目录（忽略大小写）
        QStringList systemDirs;
        systemDirs << "C:/Windows"
                   << "C:/Program Files"
                   << "C:/Program Files (x86)"
                   << "C:/System Volume Information"
                   << "C:/$Recycle.Bin";

        QElapsedTimer timer;
        timer.start();

        // 检查是否为系统目录的辅助函数
        auto isSystemDir = [&](const QString &dirPath) {
            for (const QString &systemDir : systemDirs) {
                if (dirPath.startsWith(systemDir, Qt::CaseInsensitive)) {
                    return true;
                }
            }
            return false;
        };

        // 第一步：统计四层深度下的总文件夹和文件数
        QDir dir(path);
        QDir::Filters dirFilters = QDir::Dirs | QDir::NoDotAndDotDot;
        QDir::Filters fileFilters = QDir::Files;

        std::function<void(const QString &, int)> countItems = [&](const QString currentPath, int depth) {
            if (depth > 4 || imageWatcher.isCanceled()) return;

            // 跳过系统目录
            if (isSystemDir(currentPath)) {
                qDebug() << "Skipping system directory in countItems:" << currentPath;
                return;
            }

            QDir currentDir(currentPath);
            if (!currentDir.exists()) {
                qDebug() << "Directory does not exist:" << currentPath;
                return;
            }

            // 统计当前目录下的文件夹
            QFileInfoList dirList = currentDir.entryInfoList(dirFilters);
            qDebug() << "Counting subdirs in" << currentPath << ": Subdirs found =" << dirList.size();
            for (const QFileInfo &dirInfo : dirList) {
                totalItems++;
                countItems(dirInfo.absoluteFilePath(), depth + 1);
            }

            // 统计当前目录下的文件
            currentDir.setNameFilters(filters);
            QFileInfoList fileList = currentDir.entryInfoList(fileFilters);
            totalItems += fileList.size();
            qDebug() << "Counting in" << currentPath << ": Files found =" << fileList.size();
            for (const QFileInfo &fileInfo : fileList) {
                qDebug() << "Found file:" << fileInfo.absoluteFilePath().toUtf8();
            }
        };

        countItems(path, 0);
        qDebug() << "Total items (folders + files):" << totalItems << ", Elapsed:" << timer.elapsed() << "ms";

        // 第二步：扫描并分批加载
        std::function<void(const QString &, int)> processItems = [&](const QString currentPath, int depth) {
            if (depth > 4 || imageWatcher.isCanceled()) return;

            // 跳过系统目录
            if (isSystemDir(currentPath)) {
                qDebug() << "Skipping system directory in processItems:" << currentPath;
                return;
            }

            QDir currentDir(currentPath);
            if (!currentDir.exists()) {
                qDebug() << "Directory does not exist:" << currentPath;
                return;
            }

            qDebug() << "Processing dir:" << currentPath;

            // 处理当前目录下的文件
            currentDir.setNameFilters(filters);
            QFileInfoList fileList = currentDir.entryInfoList(fileFilters);
            QList<QPair<QString, QPixmap>> foundImages;

            qDebug() << "Processing in" << currentPath << ": Files found =" << fileList.size();
            for (const QFileInfo &fileInfo : fileList) {
                processedItems++;
                QString filePath = fileInfo.absoluteFilePath();
                qDebug() << "Processing file:" << filePath;

                if (validateImageSize(filePath)) {
                    QPixmap pixmap(filePath);
                    if (!pixmap.isNull()) {
                        QPixmap thumbnail = pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                        foundImages.append(qMakePair(filePath, thumbnail));
                        validFilesCount++;
                        qDebug() << "Valid image added:" << filePath;
                    } else {
                        failedFiles << fileInfo.fileName();
                        qDebug() << "Failed to load image:" << filePath;
                    }
                } else {
                    qDebug() << "File does not meet size requirements:" << filePath;
                }

                // 每处理 10 个文件更新一次进度
                if (totalItems > 0 && processedItems % 10 == 0) {
                    int progress = static_cast<int>(static_cast<double>(processedItems) / totalItems * 100.0);
                    emit progressValueChanged(progress);
                }
            }

            if (!foundImages.isEmpty()) {
                emit imagesFound(foundImages);
            }

            // 处理子目录
            // 处理子目录之前清除 nameFilters
            currentDir.setNameFilters(QStringList()); // 重置过滤器
            QFileInfoList dirList = currentDir.entryInfoList(dirFilters);
            qDebug() << "Entering subdirectories in" << currentPath << ": Subdirs found =" << dirList.size();
            for (const QFileInfo &dirInfo : dirList) {
                processedItems++;
                qDebug() << "Entering subdir:" << dirInfo.absoluteFilePath();
                processItems(dirInfo.absoluteFilePath(), depth + 1);

                // 每处理 10 个目录更新一次进度
                if (totalItems > 0 && processedItems % 10 == 0) {
                    int progress = static_cast<int>(static_cast<double>(processedItems) / totalItems * 100.0);
                    emit progressValueChanged(progress);
                }
            }
        };

        qDebug() << "Starting scan..." << path;
        processItems(path, 0);

        emit progressValueChanged(100);
        qDebug() << "Scan completed: Total items:" << totalItems
                 << ", Valid files:" << validFilesCount
                 << ", Elapsed:" << timer.elapsed() << "ms";
    });

    imageWatcher.setFuture(future);
}


bool MainWindow::validateImageSize(const QString &path) const
{
    QImageReader reader(path);
    QSize size = reader.size();
    int width = size.width();
    int height = size.height();
    bool isValid = (width >= 400) && (height >= 400);
    return isValid;
}

void MainWindow::onImagesLoaded()
{
    ui->progressBar->setVisible(false);
    if (ui->imageGridWidget->count() == 0) {
        QListWidgetItem *item = new QListWidgetItem("未找到宽高大于400的图片");
        item->setTextAlignment(Qt::AlignCenter);
        ui->imageGridWidget->addItem(item);
    }

    if (!failedFiles.isEmpty()) {
        QListWidgetItem *item = new QListWidgetItem(
            QString("以下文件加载失败：%1").arg(failedFiles.join(", "))
            );
        item->setTextAlignment(Qt::AlignCenter);
        ui->imageGridWidget->addItem(item);
    }
}

void MainWindow::onImageSelectionChanged()
{
    QList<QListWidgetItem*> selectedItems = ui->imageGridWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        ui->pathLabel->setText("路径: 未选择图片");
        ui->creationTimeLabel->setText("创建时间: 未选择图片");
        ui->sizeLabel->setText("尺寸: 未选择图片");
        return;
    }

    QListWidgetItem *selectedItem = selectedItems.first();
    QString filePath = itemToPathMap.value(selectedItem, "");
    if (filePath.isEmpty()) {
        return;
    }

    QFileInfo fileInfo(filePath);
    QImageReader reader(filePath);
    QSize size = reader.size();

    ui->pathLabel->setText(QString("路径: %1").arg(filePath));
    ui->creationTimeLabel->setText(QString("创建时间: %1").arg(fileInfo.birthTime().toString("yyyy-MM-dd hh:mm:ss")));
    ui->sizeLabel->setText(QString("尺寸: %1 x %2").arg(size.width()).arg(size.height()));
}

MainWindow::~MainWindow()
{
    delete ui;
    thumbnailCache.clear();
}
