#include "RambunctionVision/camera.hpp"

namespace rv {
  void Camera::write(cv::FileStorage fs) const {
    fs << "{";
    fs << "ID" << id;
    fs << "Offset" << offset;
    fs << "AngleOffset" << angleOffset;
    fs << "CameraMatrix" << matrix;
    fs << "Distortion" << dst;
    fs << "}";  
  }

  void Camera::read(const cv::FileNode fn) {
    fn["ID"] >> id;
    fn["offset"] >> offset;
    fn["AngleOffset"] >> angleOffset;
    fn["CameraMatrix"] >> matrix;
    fn["Distortion"] >> dst;
  }
}