#ifndef CALIBRATE_HPP
#define CALIBRATE_HPP

#include <vector>
#include <string>

#include <opencv2/core.hpp>

#include "RambunctionVision/config.hpp"

cv::Mat getUndistortedImage(rv::CalibrateConfig config, cv::Mat image, rv::Camera camera, bool undistortImage, bool showAxis);
void showUndistortedVideoCapture(rv::CalibrateConfig config, rv::Camera &camera);
void showUndistortedPhoto(rv::CalibrateConfig config, std::string file, rv::Camera camera, std::string windowName = "Undistorted");
void showUndistorted(rv::CalibrateConfig config, rv::Camera camera);

void calibrateFromVideoCapture(rv::CalibrateConfig config, rv::Camera &camera);
void calibrateFromPhotos(rv::CalibrateConfig config, rv::Camera &camera);
void calibrate(rv::CalibrateConfig config, rv::Camera &camera);

#endif