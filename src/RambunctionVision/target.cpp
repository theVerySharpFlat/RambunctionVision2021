#include "RambunctionVision/target.hpp"

namespace rv {
  void Target::write(cv::FileStorage fs) const {
    fs << "{";
    fs << "Name" << name;
    fs << "Shape" << shape;
    fs << "}";  
  }

  void Target::read(const cv::FileNode fn) {
    fn["Name"] >> name;
    fn["Shape"] >> shape;
  }
}