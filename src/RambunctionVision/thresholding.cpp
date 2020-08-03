#include "RambunctionVision/thresholding.hpp"

namespace rv {
  void HSV::write(cv::FileStorage fs) const {
    fs << "{";
    fs << "LowH" << lowH;
    fs << "HighH" << highH;
    fs << "LowS" << lowS;
    fs << "HighS" << highS;
    fs << "LowV" << lowV;
    fs << "HighV" << highV;
    fs << "}";  
  }

  void HSV::read(const cv::FileNode fn) {
    fn["LowH"] >> lowH;
    fn["HighH"] >> highH;
    fn["LowS"] >> lowS;
    fn["HighS"] >> highS;
    fn["LowV"] >> lowV;
    fn["HighV"] >> highV;
  }
}
