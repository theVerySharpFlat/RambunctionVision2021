#ifndef THRESHOLDING_HPP
#define THRESHOLDING_HPP

#include <string>

#include <opencv2/core.hpp>

namespace rv {
  struct HSV {
    const static int max = 255;
    const static int maxH = 180;
    int lowH = 0;
    int highH = maxH;
    int lowS = 0;
    int highS = max;
    int lowV = 0;
    int highV = max;
    cv::Scalar lowScalar() { 
      return cv::Scalar(lowH, lowS, lowV); }
    cv::Scalar highScalar() { 
      return cv::Scalar(highH, highS, highV); }
    void write(cv::FileStorage fs) const;
    void read(const cv::FileNode fn);
  };

  static void write(cv::FileStorage& fs, const std::string& name, const HSV& x) {
    x.write(fs); 
  }

  static void read(const cv::FileNode& node, HSV& x, const HSV& default_value = HSV()) {
    if(node.empty())
      x = default_value;
    else
      x.read(node);
  }
}

#endif