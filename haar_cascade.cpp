#include "haar_cascade.h"
#include <iostream>
#include <cstdlib>// для генерации случайных чисел
#include <ctime>  // для инициализации генератора случайных чисел
#include <cmath> // для функции std::exp
#include <opencv2/ml.hpp>  // для использования классификатора AdaBoost

// Функция вычисления интегрального изображения
void computeIntegralImage(const cv::Mat& src, cv::Mat& integralImg) {
    cv::integral(src, integralImg, CV_32S);
}

// Функция вычисления значения признака Хаара
int computeHaarFeatureValue(const cv::Mat& integralImg, int x, int y, int width, int height) {
    int whiteRectSum = integralImg.at<int>(y, x) + integralImg.at<int>(y + height, x + width);
    int blackRectSum = integralImg.at<int>(y, x + width) + integralImg.at<int>(y + height, x);
    return whiteRectSum - blackRectSum;
}

// Функция обучения алгоритма AdaBoost
void trainAdaboost(const std::vector<cv::Mat>& positiveSamples, const std::vector<cv::Mat>& negativeSamples, std::vector<WeakClassifier>& weakClassifiers, int numStages) {
    // Создание классификатора AdaBoost
    auto adaboost = cv::ml::Boost::create();

    // Создание обучающего набора данных
    cv::Mat trainingData;
    std::vector<int> labels;

    // Добавление положительных и отрицательных примеров с метками
    for (const auto& samples : {std::make_pair(positiveSamples, 1), std::make_pair(negativeSamples, -1)}) {
        for (const auto& sample : samples.first) {
            cv::Mat integralImg;
            computeIntegralImage(sample, integralImg);
            int featureValue = computeHaarFeatureValue(integralImg, 0, 0, sample.cols, sample.rows);
            trainingData.push_back(featureValue);
            labels.push_back(samples.second);
        }
    }

    // Преобразование обучающих данных в формат Mat
    cv::Mat trainingDataMat(trainingData);
    cv::Mat labelsMat(labels);
    labelsMat = labelsMat.reshape(1, 1);

    // Обучение алгоритма AdaBoost
    adaboost->setBoostType(cv::ml::Boost::REAL);
    adaboost->setWeakCount(numStages);

    adaboost->train(trainingDataMat, cv::ml::ROW_SAMPLE, labelsMat);

    // Получение слабых классификаторов и преобразование их в формат WeakClassifier
    for (int i = 0; i < numStages; ++i) {
        cv::Ptr<cv::ml::DTrees> tree = cv::ml::DTrees::create();
        adaboost->getWeakClassifier(i, tree);
        float threshold = tree->getSplit().value;
        int featureIdx = tree->getSplit().var_idx;

        WeakClassifier classifier;
        classifier.region = cv::Rect(0, 0, sample.cols, sample.rows);
        classifier.threshold = threshold;

        bool isCorrectPolarity = true;
        for (int i = 0; i < trainingDataMat.rows; ++i) {
            float prediction = adaboost->predict(trainingDataMat.row(i));
            if (prediction * labelsMat.at<int>(0, i) < 0) {
                isCorrectPolarity = false;
                break;
            }
        }
        if (!isCorrectPolarity) {
            classifier.polarity = -1;
            classifier.threshold = -threshold;
        } else {
            classifier.polarity = 1;
        }

        weakClassifiers.push_back(classifier);
    }
}

// Функция построения каскадного классификатора
void constructCascadeClassifier(const std::vector<WeakClassifier>& weakClassifiers, std::vector<std::vector<WeakClassifier>>& cascadeClassifier, int numStages) {
    cascadeClassifier.resize(numStages);

    int classifiersPerStage = weakClassifiers.size() / numStages;
    auto classifierIter = weakClassifiers.begin();
    for (auto& stage : cascadeClassifier) {
        stage.insert(stage.end(), classifierIter, classifierIter + classifiersPerStage);
        classifierIter += classifiersPerStage;
    }
}

// Функция обнаружения объектов на изображении
std::vector<cv::Rect> detectObjects(const cv::Mat& inputImg, const std::vector<std::vector<WeakClassifier>>& cascadeClassifier) {
    std::vector<cv::Rect> detectedObjects;

    for (const auto& stageClassifiers : cascadeClassifier) {
        bool allPassed = true;
        for (const auto& classifier : stageClassifiers) {
            // Применяем текущий слабый классификатор к изображению
            int featureValue = computeHaarFeatureValue(inputImg, classifier.region.x, classifier.region.y, classifier.region.width, classifier.region.height);
            bool passed = (featureValue * classifier.polarity) >= classifier.threshold;
            if (!passed) {
                allPassed = false;
                break;
            }
        }
        if (allPassed) {
            // Получаем координаты объектов и добавляем их в вектор detectedObjects
            // В данном случае объекты обнаружены в центре изображения с произвольными размерами.
            cv::Rect objectRect(inputImg.cols / 4, inputImg.rows / 4, inputImg.cols / 2, inputImg.rows / 2);
            detectedObjects.push_back(objectRect);
        }
    }

    return detectedObjects;
}
