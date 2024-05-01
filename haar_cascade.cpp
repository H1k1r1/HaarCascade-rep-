#include "haar_cascade.h"
#include <iostream> // для вывода отладочной информации
#include <cstdlib>  // для генерации случайных чисел
#include <ctime>    // для инициализации генератора случайных чисел
#include <cmath>    // для функции std::exp
#include <opencv2/ml.hpp> // для использования классификатора AdaBoost

// Реализация функции вычисления интегрального изображения
void computeIntegralImage(const cv::Mat& src, cv::Mat& integralImg) {
    cv::integral(src, integralImg, CV_32S);
}

// Реализация функции вычисления значения признака Хаара
int computeHaarFeatureValue(const cv::Mat& integralImg, int x, int y, int width, int height) {
    int whiteRectSum = integralImg.at<int>(y, x) + integralImg.at<int>(y + height, x + width);
    int blackRectSum = integralImg.at<int>(y, x + width) + integralImg.at<int>(y + height, x);
    return whiteRectSum - blackRectSum;
}

// Реализация функции обучения алгоритма AdaBoost
void trainAdaboost(const std::vector<cv::Mat>& positiveSamples, const std::vector<cv::Mat>& negativeSamples, std::vector<WeakClassifier>& weakClassifiers, int numStages) {
    // Создание классификатора AdaBoost
    cv::Ptr<cv::ml::Boost> adaboost = cv::ml::Boost::create();

    // Создание обучающего набора данных
    cv::Mat trainingData;
    std::vector<int> labels;

    // Добавление положительных примеров с меткой 1
    for (const auto& sample : positiveSamples) {
        cv::Mat integralImg;
        computeIntegralImage(sample, integralImg); // Вычисление интегрального изображения
        int featureValue = computeHaarFeatureValue(integralImg, 0, 0, sample.cols, sample.rows); // Вычисление значения признака Хаара
        trainingData.push_back(featureValue); // Добавление значения признака в обучающий набор
        labels.push_back(1);
    }

    // Добавление отрицательных примеров с меткой -1
    for (const auto& sample : negativeSamples) {
        cv::Mat integralImg;
        computeIntegralImage(sample, integralImg); // Вычисление интегрального изображения
        int featureValue = computeHaarFeatureValue(integralImg, 0, 0, sample.cols, sample.rows); // Вычисление значения признака Хаара
        trainingData.push_back(featureValue); // Добавление значения признака в обучающий набор
        labels.push_back(-1);
    }

    // Преобразование обучающих данных в формат Mat
    cv::Mat trainingDataMat(trainingData);
    cv::Mat labelsMat(labels);
    labelsMat = labelsMat.reshape(1, 1);

    // Обучение алгоритма AdaBoost
    adaboost->setBoostType(cv::ml::Boost::REAL);
    adaboost->setWeakCount(numStages); // Установка количества этапов обучения

    adaboost->train(trainingDataMat, cv::ml::ROW_SAMPLE, labelsMat);

    // Получение слабых классификаторов
    std::vector<cv::Ptr<cv::ml::DTrees>> weakTrees;
    adaboost->getWeakPredictors(weakTrees);

    // Преобразование слабых классификаторов в формат WeakClassifier
    for (const auto& tree : weakTrees) {
        // Получение параметров слабого классификатора (порога и индекса признака)
        float threshold = tree->getSplit().value;
        int featureIdx = tree->getSplit().var_idx;

        WeakClassifier classifier;
        classifier.region = cv::Rect(0, 0, sample.cols, sample.rows); // Регион равен размеру образца (пока без вычислений)
        classifier.threshold = threshold;

        // Проверка полярности
        // Проходим по обучающему набору данных и делаем предсказания
        bool isCorrectPolarity = true;
        for (int i = 0; i < trainingDataMat.rows; ++i) {
            float prediction = adaboost->predict(trainingDataMat.row(i));
            if (prediction * labelsMat.at<int>(0, i) < 0) { // Если предсказанное значение отличается от истинного
                isCorrectPolarity = false;
                break;
            }
        }
        // Если полярность неверная, меняем ее на противоположную
        if (!isCorrectPolarity) {
            classifier.polarity = -1;
            classifier.threshold = -threshold; // Инвертируем порог
        } else {
            classifier.polarity = 1;
        }

        weakClassifiers.push_back(classifier); // Добавление слабого классификатора в вектор
    }
}

// Реализация функции построения каскадного классификатора
void constructCascadeClassifier(const std::vector<WeakClassifier>& weakClassifiers, std::vector<std::vector<WeakClassifier>>& cascadeClassifier, int numStages) {
    // Создание каскадного классификатора с заданным количеством этапов
    cascadeClassifier.resize(numStages);

    // Распределение слабых классификаторов по этапам
    int classifiersPerStage = weakClassifiers.size() / numStages;
    int classifierIdx = 0;
    for (int stage = 0; stage < numStages; ++stage) {
        std::vector<WeakClassifier> stageClassifiers;
        for (int i = 0; i < classifiersPerStage; ++i) {
            stageClassifiers.push_back(weakClassifiers[classifierIdx]);
            ++classifierIdx;
        }
        cascadeClassifier[stage] = stageClassifiers;
    }
}

// Реализация функции обнаружения объектов на изображении
std::vector<cv::Rect> detectObjects(const cv::Mat& inputImg, const std::vector<std::vector<WeakClassifier>>& cascadeClassifier) {
    std::vector<cv::Rect> detectedObjects;

    // Проходим по каждому каскадному классификатору
    for (const auto& stageClassifiers : cascadeClassifier) {
        bool allPassed = true;
        // Проходим по каждому слабому классификатору на текущем этапе
        for (const auto& classifier : stageClassifiers) {
            // TODO: Применяем текущий слабый классификатор к изображению
            // Здесь нужно реализовать применение слабого классификатора к изображению,
            // чтобы определить, проходит ли объект тестирование или нет.
            // Если объект не проходит тестирование текущим классификатором, выходим из цикла.
            // В примере предполагается, что все объекты прошли тестирование.
            // bool passed = applyWeakClassifier(inputImg, classifier);
            bool passed = true; // заглушка, предполагается, что объект проходит тестирование
            if (!passed) {
                allPassed = false;
                break;
            }
        }
        // Если все объекты на текущем этапе прошли тестирование, добавляем их в результирующий вектор
        if (allPassed) {
            // TODO: Получаем координаты объектов и добавляем их в вектор detectedObjects
            // Здесь нужно реализовать определение координат объектов на изображении,
            // которые прошли тестирование всех слабых классификаторов на текущем этапе.
            // В примере предполагается, что объекты обнаружены в центре изображения с произвольными размерами.
            // cv::Rect objectRect(x, y, width, height);
            // detectedObjects.push_back(objectRect);
            cv::Rect objectRect(inputImg.cols / 4, inputImg.rows / 4, inputImg.cols / 2, inputImg.rows / 2); // заглушка
            detectedObjects.push_back(objectRect);
        }
    }

    return detectedObjects;
}
