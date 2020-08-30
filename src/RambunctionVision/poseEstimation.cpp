#include "RambunctionVision/poseEstimation.hpp"

#include <iostream>


#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>

namespace rv {
  std::vector<cv::Point2f> asPointFloat(std::vector<cv::Point> points) {
    std::vector<cv::Point2f> correctedPoints;
    std::transform(points.begin(), points.end(), std::back_inserter(correctedPoints), [](cv::Point point) -> cv::Point2f {
      return cv::Point2f(point.x, point.y); }); 
    return correctedPoints;
  }

  std::vector<cv::Point> asPoint(std::vector<cv::Point2f> points) {
    std::vector<cv::Point> correctedPoints;
    std::transform(points.begin(), points.end(), std::back_inserter(correctedPoints), [](cv::Point2f point) -> cv::Point {
      return cv::Point(point.x, point.y); }); 
    return correctedPoints;
  }

  std::vector<cv::Point> asPoint(std::vector<cv::Point3f> points) {
    std::vector<cv::Point> correctedPoints;
    std::transform(points.begin(), points.end(), std::back_inserter(correctedPoints), [](cv::Point3f point) -> cv::Point {
      return cv::Point(point.x, point.y); }); 
    return correctedPoints;
  }

  std::vector<cv::Point3f> asObjectPoints(std::vector<cv::Point> targetPoints) {
    std::vector<cv::Point3f> correctedPoints;
    std::transform(targetPoints.begin(), targetPoints.end(), std::back_inserter(correctedPoints), [](cv::Point point) -> cv::Point3f {
      return cv::Point3f(point.x, point.y, 0); });
    return correctedPoints;
  }

  std::vector<cv::Point3f> chessboardObjectPoints(cv::Size boardSize, double squareSize) {
    std::vector<cv::Point3f> points; 
    for (int i = 0; i < boardSize.height; i++) {
      for (int j = 0; j < boardSize.width; j++) {
        points.push_back(cv::Point3f(j * squareSize, i * squareSize, 0));
      }
    }
    return points;
  }

  void drawBox(cv::Mat &image, std::vector<cv::Point3f> objectPoints, std::vector<cv::Point2f> imagePoints, rv::Camera camera, int type, bool invert) {
    cv::Mat rvec, tvec, rotation, transformation, corrected;
    cv::RotatedRect box2d = cv::minAreaRect(asPoint(objectPoints));
    double data[4] = {0.0,0.0,0.0,1.0};

    // Solves perspective-n-point returning a rotation vector (rvec) and translation vector (tvec)
    cv::solvePnP(objectPoints, imagePoints, camera.matrix, camera.dst, rvec, tvec);
    cv::Rodrigues(rvec, rotation);
    cv::hconcat(rotation, tvec, transformation);
    cv::vconcat(transformation, cv::Mat(1,4, CV_64F, &data), transformation);
    corrected = camera.offsetMatrix() * transformation;
    cv::Rodrigues(corrected.rowRange(0, 3).colRange(0,3), rvec);
    tvec = corrected.rowRange(0,3).colRange(3,4);

    switch (type) {
      case Box: {
        std::vector<cv::Point3f> bottom;
          bottom.push_back(cv::Point3f(0, 0, 0));
          bottom.push_back(cv::Point3f(box2d.size.height, 0, 0));
          bottom.push_back(cv::Point3f(box2d.size.height, box2d.size.width, 0));
          bottom.push_back(cv::Point3f(0, box2d.size.width, 0));
        std::vector<cv::Point2f> imageBottom;
        std::vector<cv::Point3f> top;
          top.push_back(cv::Point3f(0, 0, (invert ? -1 : 1) * (box2d.size.aspectRatio() * box2d.size.height) / 2));
          top.push_back(cv::Point3f(box2d.size.height, 0, (invert ? -1 : 1) * (box2d.size.aspectRatio() * box2d.size.height) / 2));
          top.push_back(cv::Point3f(box2d.size.height, box2d.size.width, (invert ? -1 : 1) * (box2d.size.aspectRatio() * box2d.size.height) / 2));
          top.push_back(cv::Point3f(0, box2d.size.width, (invert ? -1 : 1) * (box2d.size.aspectRatio() * box2d.size.height) / 2));
        std::vector<cv::Point2f> imageTop;
        
        // Projects and then draws box onto image
        cv::projectPoints(bottom, rvec, tvec, camera.matrix, camera.dst, imageBottom);
        cv::projectPoints(top, rvec, tvec, camera.matrix, camera.dst, imageTop);

        cv::drawContours(image, std::vector<std::vector<cv::Point>>{asPoint(imageBottom)}, 0, cv::Scalar(0,255,0), 2);
        cv::drawContours(image, std::vector<std::vector<cv::Point>>{asPoint(imageTop)}, 0, cv::Scalar(0,0,255), 2);
        cv::line(image, imageBottom[0], imageTop[0], cv::Scalar(255,0,0), 2);
        cv::line(image, imageBottom[1], imageTop[1], cv::Scalar(255,0,0), 2);
        cv::line(image, imageBottom[2], imageTop[2], cv::Scalar(255,0,0), 2);
        cv::line(image, imageBottom[3], imageTop[3], cv::Scalar(255,0,0), 2);
        break;

      } case Axis: {
        std::vector<cv::Point3f> axis;
          axis.push_back(cv::Point3f(0, 0, 0));
          axis.push_back(cv::Point3f(box2d.size.height/2, 0, 0));
          axis.push_back(cv::Point3f(0, box2d.size.width/2 ,0));
          axis.push_back(cv::Point3f(0, 0, ((invert ? -1 : 1) * box2d.size.aspectRatio() * box2d.size.height) / 2));
        std::vector<cv::Point2f> imageAxis;

        // Projects and then draws Axis onto image
        cv::projectPoints(axis, rvec, tvec, camera.matrix, camera.dst, imageAxis);
        cv::line(image, imageAxis[0], imageAxis[1], cv::Scalar(255,0,0), 4);
        cv::line(image, imageAxis[0], imageAxis[2], cv::Scalar(0,255,0), 4);
        cv::line(image, imageAxis[0], imageAxis[3], cv::Scalar(0,0,255), 4);
        break;

      } default: {
        // Defaults to just drawing te contours
        cv::drawContours(image, std::vector<std::vector<cv::Point>>{asPoint(imagePoints)}, 0, cv::Scalar(0,0,255), 2); 
      }
    }
  }
}