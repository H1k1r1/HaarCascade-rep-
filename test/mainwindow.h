#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <opencv2/opencv.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loadButton_clicked();

private:
    Ui::MainWindow *ui;
    cv::CascadeClassifier faceCascade;
    cv::Mat currentImage;
    bool detectAndDisplay(cv::Mat frame);
};
#endif // MAINWINDOW_H
