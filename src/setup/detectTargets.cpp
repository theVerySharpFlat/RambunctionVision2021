#include <iostream>
#include <vector>
#include <string>
#include <math.h>

#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>

#include "RambunctionVision/detection.hpp"
#include "RambunctionVision/camera.hpp"
#include "RambunctionVision/thresholding.hpp"
#include "RambunctionVision/poseEstimation.hpp"
#include "RambunctionVision/config.hpp"

void detectTargetsVideoCapture(rv::Camera camera, rv::HSV thresholds, std::vector<rv::Target> targets) {
  cv::VideoCapture capture(camera.id);

  // Cheak Camera
  if (!capture.isOpened()) {
    std::cerr << "Could access camera with id: " << camera.id << '\n';
    return;
  }

  const std::string window = "Detection";
  cv::namedWindow(window);
  
  bool showThresh = false, showBox = true, useAxis = false, showUndistorted = false;
  while (true) {
    cv::Mat frame, blur, hsv, thresh, threshColor, edges,  display;
    capture >> frame;
   
    // Cheak Camera
    if (frame.empty()) {
      std::cerr << "Lost connection to camera with id: " << camera.id << '\n';
      break;
    }

    // Pre prosses image for contour detection
    // cv::blur(frame, blur, cv::Size(10,10));
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);
    cv::inRange(hsv, thresholds.lowScalar(), thresholds.highScalar(), thresh);
    cv::cvtColor(thresh, threshColor, cv::COLOR_GRAY2BGR);
    display = showThresh ? threshColor : frame;
    
    // Find and match contours
    std::vector<std::vector<cv::Point>> contours;
    std::vector<rv::Target> found;
    cv::findContours(thresh, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    matchTargets(targets, contours, found);

    // Draw on image
    if (showBox && !camera.matrix.empty() && !camera.dst.empty()) {
      for (auto find = found.begin(); find != found.end(); find++) {
        // finds target that matches the contour for position estimation
        std::vector<rv::Target>::iterator target = std::find_if(targets.begin(), targets.end(), [&f = find] (const rv::Target &t) -> bool {return t.name == f->name; });
        rv::drawBox(display, rv::asObjectPoints(target->shape), rv::asPointFloat(find->shape), camera, (useAxis) ? 1 : 0);
      }
    } else {
      for (auto target = found.begin(); target != found.end(); target++) {
        cv::putText(display, target->name, target->shape[0], cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255,0,0));
        cv::drawContours(display, std::vector<std::vector<cv::Point>>{target->shape}, 0, cv::Scalar(255,0,0), 2);
      }
    }

    // Conditionaly undistort the frame
    if (showUndistorted && !camera.matrix.empty() && !camera.dst.empty()) {
      cv::undistort(display, display, camera.matrix, camera.dst);
    }

    // Shows the frame
    cv::imshow(window, display);

    // Parese key presses
    int key = cv::waitKey(30);

    showThresh = (key == 't') ? !showThresh : showThresh;
    showUndistorted = (key == 'd') ? !showUndistorted : showUndistorted;
    showBox = (key == 'b') ? !showBox : showBox;
    useAxis = (key == 'a') ? !useAxis : useAxis;

    if (key == 'q' || key == 27) {
      break;
    }
  }
  cv::destroyAllWindows();
  cv::waitKey(1);
  return;
}

void detectTargetsPhotos(rv::Camera camera, rv::HSV thresholds, std::vector<rv::Target> targets, rv::ThresholdingConfig config) {

  const std::string window = "Detection";
  cv::namedWindow(window);

  int currentImage = 0;
  int numberOfImages = config.images.end() - config.images.begin();
  
  bool showThresh = false, showBox = true, useAxis = false, showUndistorted = false;
  while (true) {
    cv::Mat image = cv::imread(config.images[currentImage]), blur, hsv, thresh, threshColor, edges,  display, undistorted;

    // Cheak Image
    if (image.empty()) {
      std::cerr << "Error: Could not find: " << currentImage << '\n';
      break;
    }

    // Pre prosses image for contour detection
    // cv::blur(image, blur, cv::Size(10,10));
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
    cv::inRange(hsv, thresholds.lowScalar(), thresholds.highScalar(), thresh);
    cv::cvtColor(thresh, threshColor, cv::COLOR_GRAY2BGR);
    display = showThresh ? threshColor : image;
    
    // Find and match contours
    std::vector<std::vector<cv::Point>> contours;
    std::vector<rv::Target> found;
    cv::findContours(thresh, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    matchTargets(targets, contours, found);

    // Draw on image
    if (showBox && !camera.matrix.empty() && !camera.dst.empty()) {
      for (auto find = found.begin(); find != found.end(); find++) {
        // finds target that matches the contour for position estimation
        std::vector<rv::Target>::iterator target = std::find_if(targets.begin(), targets.end(), [&f = find] (const rv::Target &t) -> bool {return t.name == f->name; });
        rv::drawBox(display, rv::asObjectPoints(target->shape), rv::asPointFloat(find->shape), camera, (useAxis) ? 1 : 0);
      }
    } else {
      for (auto target = found.begin(); target != found.end(); target++) {
        cv::putText(display, target->name, target->shape[0], cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255,0,0));
        cv::drawContours(display, std::vector<std::vector<cv::Point>>{target->shape}, 0, cv::Scalar(255,0,0), 2);
      }
    }

    // Conditionaly undistort the frame
    if (showUndistorted && !camera.matrix.empty() && !camera.dst.empty()) {
      cv::undistort(display, undistorted, camera.matrix, camera.dst);
      display = undistorted;
    }

    // Shows the frame
    cv::imshow(window, display);

    // parese key presses
    int key = cv::waitKey(30);

    currentImage -= (key == 44 && currentImage > 0) ? 1 : 0;
    currentImage += (key == 46 && currentImage < (numberOfImages-1)) ? 1 : 0;

    showThresh = (key == 't') ? !showThresh : showThresh;
    showUndistorted = (key == 'd') ? !showUndistorted : showUndistorted;
    showBox = (key == 'b') ? !showBox : showBox;
    useAxis = (key == 'a') ? !useAxis : useAxis;

    if (key == 'q' || key == 27) {
      break;
    }
  }
  cv::destroyAllWindows();
  cv::waitKey(1);
  return;
}

void detectTargets(rv::Camera camera, rv::HSV thresholds, std::vector<rv::Target> targets, rv::ThresholdingConfig config) {
  return config.usePhotos ? detectTargetsPhotos(camera, thresholds, targets, config) : detectTargetsVideoCapture(camera, thresholds, targets);
}