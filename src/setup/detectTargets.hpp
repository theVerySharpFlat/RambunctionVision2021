#ifndef DETECTION_HPP
#define DETECTION_HPP

#include "RambunctionVision/camera.hpp"
#include "RambunctionVision/target.hpp"
#include "RambunctionVision/thresholding.hpp"

void detectTargets(rv::Camera camera, rv::HSV thresholds, std::vector<rv::Target> targets, rv::ThresholdingConfig config);
void detectTargetsPhotos(rv::Camera camera, rv::HSV thresholds, std::vector<rv::Target> targets, rv::ThresholdingConfig config);
void detectTargetsVideoCapture(rv::Camera camera, rv::HSV thresholds, std::vector<rv::Target> targets);

#endif