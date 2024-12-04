#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
//#include <opencv2/highgui.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>


namespace TestReadTiffImage {

struct BallPercept {
  cv::Point3f pos;     // The 3D position of the ball (assume projection relationship is known)
  double      radius;  // Radius of the ball
  double      stamp;   // Timestamp (if time-related processing is needed)
};

cv::Mat detectTennisBall(const cv::Mat& inputImage, std::vector<BallPercept>& detectedBalls, double timestamp) {
  // Output image (used to display detection results)
  cv::Mat outputImage = inputImage.clone();

  // 1. Apply Gaussian blur to smooth the image and reduce noise
  cv::Mat smoothedImg;
  cv::GaussianBlur(inputImage, smoothedImg, cv::Size(5, 5), 0, 0);

  // 2. Convert to HSV color space
  cv::Mat hsvImg;
  cv::cvtColor(smoothedImg, hsvImg, cv::COLOR_BGR2HSV);

  // 3. Thresholding (assuming the tennis ball color is known; adjust the range below)
  cv::Mat thresholdImg;
  cv::inRange(hsvImg, cv::Scalar(20, 100, 100), cv::Scalar(40, 255, 255), thresholdImg);

  // 4. Morphological operations (opening: erosion followed by dilation)
  cv::Mat morphImg;
  cv::Mat element1 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(1, 1));
  cv::Mat element2 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
  cv::erode(thresholdImg, morphImg, element1);
  cv::dilate(morphImg, morphImg, element2);

  // 5. Find contours
  std::vector<std::vector<cv::Point>> contours;
  std::vector<cv::Vec4i>              hierarchy;
  cv::findContours(morphImg, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

  // 6. Iterate through contours and filter for valid tennis balls
  for (size_t i = 0; i < contours.size(); i++) {
    float       radius;
    cv::Point2f center;
    // Fit a minimum enclosing circle for the contour
    cv::minEnclosingCircle(contours[i], center, radius);

    // Calculate the contour area and the circle area
    double contourArea = cv::contourArea(contours[i]);
    double circleArea  = CV_PI * radius * radius;

    // Filtering conditions: minimum radius and shape close to a circle
    if (radius >= 10 && contourArea > 0.6 * circleArea) {
      // Draw the detected circle
      cv::circle(outputImage, center, static_cast<int>(radius), cv::Scalar(0, 255, 0), 3);

      // Store the detection result as a BallPercept
      BallPercept percept;
      percept.pos    = cv::Point3f(center.x, center.y, radius * 2);  // Assuming Z-axis represents diameter
      percept.radius = radius;
      percept.stamp  = timestamp;
      detectedBalls.push_back(percept);
    }
  }

  // Return the image with detection results
  return outputImage;
}

void test() {
  const auto  filePath     = R"(F:\pics\tennis.tiff)";
  cv::Mat     image    = cv::imread(filePath, cv::IMREAD_UNCHANGED); 

  if (!image.empty()) {
    std::cout << "Image loaded successfully!" << std::endl;
    std::cout << "Image size: " << image.cols << " x " << image.rows << std::endl;
    std::cout << "Channels: " << image.channels() << std::endl;
    std::cout << "Depth: " << image.depth() << std::endl;
  }

  // Store detected tennis balls
  std::vector<BallPercept> detectedBalls;
  double                   timestamp =
    0.0;  // Assume timestamp is 0; in practice, it can be set using system time or passed as a parameter

  // Call the tennis ball detection function
  cv::Mat outputImage = detectTennisBall(image, detectedBalls, timestamp);
  int     a           = 1;

}
}  // namespace TestReadTiffImage

void test() {
  TestReadTiffImage::test();
}