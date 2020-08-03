#ifndef DATA_HPP
#define DATA_HPP

#include <string>
#include <vector>

#include "geometry.hpp"
#include "thresholding.hpp"

// namespace rv {
//   struct TargetData {
//     int id;
//     std::string name;
//     Position pose;
//     Orientation orient;
//     double confidence;
//     double latency;
//     void write(cv::FileStorage fs) const;
//     void read(cv::FileNode fn);
//   };

//   static void write(cv::FileStorage& fs, const std::string& name, const TargetData& x) {
//     x.write(fs); 
//   }

//   static void read(const cv::FileNode& node, TargetData& x, const TargetData& default_value = TargetData()) {
//     if(node.empty())
//       x = default_value;
//     else
//       x.read(node);
//   }

//   struct CameraData {
//     int device;
//     double frameRate;
//     Size size;
//     double aspectRatio;
//     HSV thresholds;
//     void write(cv::FileStorage fs) const;
//     void read(cv::FileNode fn);
//   };

//   static void write(cv::FileStorage& fs, const std::string& name, const CameraData& x) {
//     x.write(fs); 
//   }

//   static void read(const cv::FileNode& node, CameraData& x, const CameraData& default_value = CameraData()) {
//     if(node.empty())
//       x = default_value;
//     else
//       x.read(node);
//   }

//   struct SocketData {
//     int port;
//     std::string ipAdress;
//     void write(cv::FileStorage fs) const;
//     void read(cv::FileNode fn);
//   };

//   static void write(cv::FileStorage& fs, const std::string& name, const SocketData& x) {
//     x.write(fs); 
//   }

//   static void read(const cv::FileNode& node, SocketData& x, const SocketData& default_value = SocketData()) {
//     if(node.empty())
//       x = default_value;
//     else
//       x.read(node);
//   }

//   struct AllData {
//     std::vector<TargetData> target;
//     CameraData camera;
//     SocketData socket; 
//     void write(cv::FileStorage fs) const;
//     void read(cv::FileNode fn);
//   };

//   static void write(cv::FileStorage& fs, const std::string& name, const AllData& x) {
//     x.write(fs); 
//   }

//   static void read(const cv::FileNode& node, AllData& x, const AllData& default_value = AllData()) {
//     if(node.empty())
//       x = default_value;
//     else
//       x.read(node);
//   }
// }

#endif