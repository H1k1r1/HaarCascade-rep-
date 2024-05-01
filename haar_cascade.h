#ifndef HAAR_CASCADE_H
#define HAAR_CASCADE_H

#include <opencv2/opencv.hpp>

// Определение структуры слабого классификатора
struct WeakClassifier {
    cv::Rect region;  // Регион признака Хаара
    int threshold;    // Порог для классификации
    int polarity;     // Полярность классификации (+1 или -1)
};
// Объявления функций

// Вычисляет интегральное изображение для данного входного изображения.
// Интегральное изображение позволяет быстро вычислять сумму значений пикселей
// в прямоугольных областях изображения..
void computeIntegralImage(const cv::Mat& src, cv::Mat& integralImg);

// Вычисляет значение признака Хаара для заданной области изображения.
int computeHaarFeatureValue(const cv::Mat& integralImg, int x, int y, int width, int height);

// Обучает алгоритм AdaBoost для построения каскада Хаара.
void trainAdaboost(const std::vector<cv::Mat>& positiveSamples, const std::vector<cv::Mat>& negativeSamples, std::vector<WeakClassifier>& weakClassifiers, int numStages);

// Строит каскадный классификатор на основе слабых классификаторов.
void constructCascadeClassifier(const std::vector<WeakClassifier>& weakClassifiers, std::vector<std::vector<WeakClassifier>>& cascadeClassifier, int numStages);

// Обнаруживает объекты на изображении с использованием каскадного классификатора.
std::vector<cv::Rect> detectObjects(const cv::Mat& inputImg, const std::vector<std::vector<WeakClassifier>>& cascadeClassifier);
bool applyWeakClassifier(const cv::Mat& inputImg, const WeakClassifier& weakClassifier);
#endif
