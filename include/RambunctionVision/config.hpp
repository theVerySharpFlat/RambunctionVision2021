#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <vector>
#include <string>

#include <opencv2/core.hpp>

namespace rv {
  struct CalibrateConfig {
    cv::Size boardSize;
    double squareSize;
    int numberOfImages;
    bool automatic;
    int delay;
    bool usePhotos;
    std::vector<std::string> images;
    void write(cv::FileStorage fs) const;
    void read(cv::FileNode fn);
  };

  static void write(cv::FileStorage& fs, const std::string& name, const CalibrateConfig& x) {
    x.write(fs); 
  }

  static void read(const cv::FileNode& node, CalibrateConfig& x, const CalibrateConfig& default_value = CalibrateConfig()) {
    if(node.empty())
      x = default_value;
    else
      x.read(node);
  }

  struct ThresholdingConfig {
    bool usePhotos;
    std::vector<std::string> images;
    void write(cv::FileStorage fs) const;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
    void read(cv::FileNode fn);
  };

  static void write(cv::FileStorage& fs, const std::string& name, const ThresholdingConfig& x) {
    x.write(fs); 
  }

  static void read(const cv::FileNode& node, ThresholdingConfig& x, const ThresholdingConfig& default_value = ThresholdingConfig()) {
    if(node.empty())
      x = default_value;
    else
      x.read(node);
  }
}
#endif