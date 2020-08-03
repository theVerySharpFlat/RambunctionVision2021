#ifndef TARGET_HPP
#define TARGET_HPP

#include <vector>
#include <opencv2/core.hpp>

#include "geometry.hpp"

namespace rv {
  struct Target {
    int id;
    std::string name;
    std::vector<cv::Point> shape;
    std::vector<cv::Point> bestMatch;
    double currentMatchValue = std::numeric_limits<double>::infinity();
    double bestMatchValue = std::numeric_limits<double>::infinity();
    cv::Point pose;
    Euler euler;
    void write(cv::FileStorage fs) const;
    void read(cv::FileNode fn); 
  };

  static void write(cv::FileStorage& fs, const std::string& name, const Target x) {
    x.write(fs); 
  }

  static void read(const cv::FileNode& node, Target& x, const Target& default_value = Target()) {
    if(node.empty())
      x = default_value;
    else
      x.read(node);
  }

  struct Match {
    int id;
    double current = std::numeric_limits<double>::infinity();
    double best  = std::numeric_limits<double>::infinity();
  };
}

#endif