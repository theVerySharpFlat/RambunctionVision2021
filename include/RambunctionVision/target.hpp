#ifndef TARGET_HPP
#define TARGET_HPP

#include <vector>
#include <opencv2/core.hpp>

namespace rv {
  struct Target {
    std::string name;
    std::vector<cv::Point> shape;
    double error = std::numeric_limits<double>::infinity(); 
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
}

#endif