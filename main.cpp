#include <iostream>
#include <filesystem>
#include "haar_cascade.h"

namespace fs = std::filesystem;

void loadImages(const std::string& directoryPath, std::vector<cv::Mat>& images) {
    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        cv::Mat img = cv::imread(entry.path().string(), cv::IMREAD_GRAYSCALE);
        if (!img.empty()) {
            images.push_back(img);
        }
    }
}

int main() {
    // Load sample images
    std::vector<cv::Mat> positiveSamples;
    std::vector<cv::Mat> negativeSamples;

    loadImages("positive_samples/", positiveSamples);
    loadImages("negative_samples/", negativeSamples);

    // Train the AdaBoost classifier
    std::vector<WeakClassifier> weakClassifiers;
    int numStages = 10; // Number of stages in the cascade
    trainAdaboost(positiveSamples, negativeSamples, weakClassifiers, numStages);

    // Construct the cascade classifier
    std::vector<std::vector<WeakClassifier>> cascadeClassifier;
    constructCascadeClassifier(weakClassifiers, cascadeClassifier, numStages);

    // Load an image for object detection
    cv::Mat inputImg = cv::imread("images/test_image.jpg", cv::IMREAD_GRAYSCALE);
    if (inputImg.empty()) {
        std::cerr << "Could not open or find the image!" << std::endl;
        return -1;
    }

    // Detect objects
    std::vector<cv::Rect> detectedObjects = detectObjects(inputImg, cascadeClassifier);

    // Display detected objects
    for (const auto& rect : detectedObjects) {
        cv::rectangle(inputImg, rect, cv::Scalar(255, 0, 0), 2);
    }

    cv::imshow("Detected Objects", inputImg);
    cv::waitKey(0);

    return 0;
}
