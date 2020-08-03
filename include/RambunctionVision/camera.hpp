#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <vector>
#include <opencv2/core.hpp>

#include "RambunctionVision/geometry.hpp"

namespace rv {
  
  struct Camera {
    int id = 0;
    cv::Mat matrix;
    cv::Mat dst;
    cv::Point3f offset;
    Euler angleOffset;
    void write(cv::FileStorage fs) const;
    void read(cv::FileNode fn); 
  };

  static void write(cv::FileStorage& fs, const std::string& name, const Camera x) {
    x.write(fs); 
  }

  static void read(const cv::FileNode& node, Camera& x, const Camera& default_value = Camera()) {
    if(node.empty())
      x = default_value;
    else
      x.read(node);
  }
}

#endif