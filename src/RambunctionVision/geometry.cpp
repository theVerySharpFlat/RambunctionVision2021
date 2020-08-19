#include "RambunctionVision/geometry.hpp"

namespace rv {
  void Euler::write(cv::FileStorage fs) const {
    fs << "{";
    fs << "x" << x;
    fs << "y" << y;
    fs << "z" << z; 
    fs << "}";  
  }

  void Euler::read(const cv::FileNode fn) {
    fn["x"] >> x;
    fn["y"] >> y;
    fn["z"] >> z;
  }
}