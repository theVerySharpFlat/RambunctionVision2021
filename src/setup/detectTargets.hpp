#ifndef DETECTION_HPP
#define DETECTION_HPP

#include "RambunctionVision/camera.hpp"
#include "RambunctionVision/target.hpp"
#include "RambunctionVision/thresholding.hpp"

void detectTargets(rv::Camera &camera, std::vector<rv::Target> &globalTargets);
void detectTargetsPhotos(rv::Camera &camera, std::vector<rv::Target> &globalTargets);
void detectTargetsVideoCapture(rv::Camera &camera, std::vector<rv::Target> &globalTargets);

#endif