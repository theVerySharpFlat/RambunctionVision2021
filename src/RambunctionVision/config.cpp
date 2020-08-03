#include "RambunctionVision/config.hpp"

namespace rv {
  void CalibrateConfig::write(cv::FileStorage fs) const {
    fs << "{";
    fs << "BoardWidth" << boardSize.width;
    fs << "BoardHeight" << boardSize.height;
    fs << "SquareSize" << squareSize;
    fs << "NumberOfImages" << numberOfImages;
    fs << "Automatic" << automatic;
    fs << "Delay" << delay;
    fs << "UsePhotos" << usePhotos;
    fs << "Images" << images;
    fs << "}";
  }

  void CalibrateConfig::read(cv::FileNode fn) {
    fn["BoardWidth"] >> boardSize.width;
    fn["BoardHeight"] >> boardSize.height;
    fn["SquareSize"] >> squareSize;
    fn["NumberOfImages"] >> numberOfImages;
    fn["Automatic"] >> automatic;
    fn["Delay"] >> delay;
    fn["UsePhotos"] >> usePhotos;
    fn["Images"] >> images;
  }

  void ThresholdingConfig::write(cv::FileStorage fs) const {
    fs << "{";
    fs << "UsePhotos" << usePhotos;
    fs << "Images" << images;
    fs << "}";
  }

  void ThresholdingConfig::read(cv::FileNode fn) {
    fn["UsePhotos"] >> usePhotos;
    fn["Images"] >> images;
  }
}