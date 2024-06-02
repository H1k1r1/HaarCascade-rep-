#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <opencv2\imgproc.hpp>
#include <opencv2\highgui.hpp>
#include <thread>
#include <chrono>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // Load Haar Cascade
    if (!faceCascade.load("haarcascade_frontalface_default.xml")) {
        QMessageBox::critical(this, "Error", "Could not load Haar cascade classifier.");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_loadButton_clicked()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp *.jpeg)"));
  if (fileName.isEmpty()) {
      return;
  }
  
  currentImage = cv::imread(fileName.toStdString());
  if (currentImage.empty()) {
      QMessageBox::critical(this, "Error", "Could not open or find the image.");
      return;
  }
  ui->progressBar->setValue(0);
  detectAndDisplay(currentImage);
}

bool MainWindow::detectAndDisplay(cv::Mat frame)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 20));
    ui->progressBar->setValue(rand() % 20);
    std::vector<cv::Rect> faces;
    cv::Mat frameGray;
  
    cv::cvtColor(frame, frameGray, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(frameGray, frameGray);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    ui->progressBar->setValue(40 + rand() % 20);
    // Detect faces
    faceCascade.detectMultiScale(frameGray, faces);
    std::this_thread::sleep_for(std::chrono::milliseconds(130));
    ui->progressBar->setValue(60 + rand() % 20);
    for (size_t i = 0; i < faces.size(); i++)
    {
        cv::Point center(faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2);
        cv::ellipse(frame, center, cv::Size(faces[i].width/2, faces[i].height/2), 0, 0, 360, cv::Scalar(0, 0, 255), 2);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100));
    ui->progressBar->setValue(99);
  
    // Convert the frame to QImage to display it in QLabel
    QImage qimg(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_BGR888);
    ui->imageLabel->setPixmap(QPixmap::fromImage(qimg));
    //ui->label->resize(ui->label->pixmap()->size()); // Corrected line
    ui->imageLabel->setScaledContents(true);
    ui->imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    std::this_thread::sleep_for(std::chrono::milliseconds(130));
    ui->progressBar->setValue(100);
    return true;
}
