#ifndef CALIBRATE_HPP
#define CALIBRATE_HPP

#include <vector>
#include <string>

#include <opencv2/core.hpp>

#include "RambunctionVision/camera.hpp"

cv::Mat getUndistortedImage(cv::Mat image, rv::Camera &camera, bool undistortImage, bool showAxis);
void showUndistortedVideoCapture(rv::Camera &camera);
void showUndistorted(rv::Camera &camera);

void calibrateFromVideoCapture(rv::Camera &camera);
void calibrateFromPhotos(rv::Camera &camera);
void calibrate(rv::Camera &camera);

#endif