#include "RambunctionVision/camera.hpp"

namespace rv {

  cv::Mat Camera::offsetMatrix() {
    cv::Mat rotation(3,3, CV_64F), translation(3,1, CV_64F), offsetMatrix;

    double s1 = sin(angleOffset[0] * (M_PI/180));
    double s2 = sin(angleOffset[1] * (M_PI/180));
    double s3 = sin(angleOffset[2] * (M_PI/180));

    double c1 = cos(angleOffset[0] * (M_PI/180));
    double c2 = cos(angleOffset[1] * (M_PI/180));
    double c3 = cos(angleOffset[2] * (M_PI/180));

    rotation.at<double>(0,0) = c1*c2;
    rotation.at<double>(0,1) = (c1*s2*s3)-(c3*s1);
    rotation.at<double>(0,2) = (s1*s2)+(c1*c3*s2);
    rotation.at<double>(1,0) = c2*s1;
    rotation.at<double>(1,1) = (c1*c3)+(s1*s2*s3);
    rotation.at<double>(1,2) = (c3*s1*s2)-(c1*s3);
    rotation.at<double>(2,0) = -s2;
    rotation.at<double>(2,1) = c2*s3;
    rotation.at<double>(2,2) = c2*c3;

    translation.at<double>(0,0) = offset.x;
    translation.at<double>(1,0) = offset.y;
    translation.at<double>(2,0) = offset.z;

    double data[4] = {0.0,0.0,0.0,1.0};

    cv::hconcat(rotation, translation, offsetMatrix);
    cv::vconcat(offsetMatrix, cv::Mat(1,4, CV_64F, &data), offsetMatrix);

    return offsetMatrix;
  }

  void Camera::write(cv::FileStorage fs) const {
    fs << "{";
    fs << "ID" << id;
    fs << "Offset" << offset;
    fs << "AngleOffset" << angleOffset;
    fs << "CameraMatrix" << matrix;
    fs << "Distortion" << dst;
    fs << "Thresholds" << thresholds;
    fs << "Targets" << targets;
    fs << "CalibrationConfig" << calibrateConfig;
    fs << "ThresholdingConfig" << thresholdingConfig;
    fs << "}";  
  }

  void Camera::read(const cv::FileNode fn) {
    fn["ID"] >> id;
    fn["Offset"] >> offset;
    fn["AngleOffset"] >> angleOffset;
    fn["CameraMatrix"] >> matrix;
    fn["Distortion"] >> dst;
    fn["Thresholds"] >> thresholds;
    fn["Targtes"] >> targets;
    fn["CalibrationConfig"] >> calibrateConfig;
    fn["ThresholdingConfig"] >> thresholdingConfig;
  }
}