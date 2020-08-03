#ifndef HSVTUNNING_HPP
#define HSVTUNNING_HPP

#include <iostream>
#include <math.h>
#include <string>
#include <memory>

#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "RambunctionVision/thresholding.hpp"
#include "RambunctionVision/camera.hpp"
#include "RambunctionVision/config.hpp"

const static std::string window = "HSV Tunning";

void hsvTunning(rv::HSV &hsv, rv::Camera camera, rv::ThresholdingConfig config);
void hsvTunningPhotos(rv::HSV &hsv, rv::Camera camera, rv::ThresholdingConfig config);
void hsvTunningVideoCapture(rv::HSV &hsv, rv::Camera camera);

static void onLowH(int, void *ptr) {
  rv::HSV *hsv = (rv::HSV*) ptr;
  hsv->lowH = std::min(hsv->highH-1, hsv->lowH);
  cv::setTrackbarPos("Low H", window, hsv->lowH);
}
static void onHighH(int, void *ptr) {
  rv::HSV *hsv = (rv::HSV*) ptr;
  hsv->highH = std::max(hsv->highH, hsv->lowH+1);
  cv::setTrackbarPos("High H", window, hsv->highH);
}
static void onLowS(int, void *ptr) {
  rv::HSV *hsv = (rv::HSV*) ptr;
  hsv->lowS = std::min(hsv->highS-1, hsv->lowS);
  cv::setTrackbarPos("Low S", window, hsv->lowS);
}
static void onHighS(int, void *ptr) {
  rv::HSV *hsv = (rv::HSV*) ptr;
  hsv->highS = std::max(hsv->highS, hsv->lowS+1);
  cv::setTrackbarPos("High S", window, hsv->highS);
}
static void onLowV(int, void *ptr) {
  rv::HSV *hsv = (rv::HSV*) ptr;
  hsv->lowV = std::min(hsv->highV-1, hsv->lowV);
  cv::setTrackbarPos("Low V", window, hsv->lowV);
}
static void onHighV(int, void *ptr) {
  rv::HSV *hsv = (rv::HSV*) ptr;
  hsv->highV = std::max(hsv->highV, hsv->lowV+1);
  cv::setTrackbarPos("High V", window, hsv->highV);
}
#endif