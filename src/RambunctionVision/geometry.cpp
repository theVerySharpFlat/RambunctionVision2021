#include "RambunctionVision/geometry.hpp"

namespace rv {
  void Euler::write(cv::FileStorage fs) const {
    fs << "{";
    fs << "rool" << roll;
    fs << "pitch" << pitch;
    fs << "yaw" << yaw; 
    fs << "}";  
  }

  void Euler::read(const cv::FileNode fn) {
    fn["roll"] >> roll;
    fn["pitch"] >> pitch;
    fn["yaw"] >> yaw;
  }
}