#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <vector>
#include <opencv2/core.hpp>

namespace rv {
  struct Euler {
    double roll = 0;
    double pitch = 0;
    double yaw = 0;
    void write(cv::FileStorage fs) const;
    void read(cv::FileNode fn);
  };

  static void write(cv::FileStorage& fs, const std::string& name, const Euler& x) {
    x.write(fs); 
  }

  static void read(const cv::FileNode& node, Euler& x, const Euler& default_value = Euler()) {
    if(node.empty())
      x = default_value;
    else
      x.read(node);
  }
}

#endif