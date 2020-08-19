#ifndef POSE_ESTIMATION_HPP
#define POSE_ESTIMATION_HPP

#include <vector>

#include <opencv2/core.hpp>

#include "RambunctionVision/camera.hpp"
#include "RambunctionVision/geometry.hpp"

namespace rv {
  #define Box 0
  #define Axis 1

  /**
   * For some reason Opencv Points are not interoperable, so this function is nessesary to conver cv::Point to cv::Point2f
   * 
   * @param points[in] input vector of cv::Points
   * @return vector of cv::Point2fs with the same values
  */
  std::vector<cv::Point2f> asPointFloat(std::vector<cv::Point> points);

  /**
  * For some reason Opencv Points are not interoperable, so this function is nessesary to conver cv::Point2f to cv::Point
  * 
  * @param points[in] input vector of cv::Point2fs
  * @return vector of cv::Points with the same values
  */
  std::vector<cv::Point> asPoint(std::vector<cv::Point2f> points);

  /**
   * Converts 3d points to 2d points by truncating the z value
   * 
   * @param points[in] input vector of cv::Point3f
   * @return vector of cv::Point with the same values
  */
  std::vector<cv::Point> asPoint(std::vector<cv::Point3f> points);

  /**
   * Converts 2d points to 3d points by adding 0 in the z direction
   * 
   * @param points[in] input vector of cv::Points
   * @return vector of cv::Point3f with the same values
  */
  std::vector<cv::Point3f> asObjectPoints(std::vector<cv::Point> targetPoints);

  /**
   * Creats the object points for a chessboard
   * 
   * @param boardSize Size of thge shess board (colums, rows)
   * @param squareSize  Side length of chessboard square
   * @return 3d points of all the inner corners of the chessboard
   */
  std::vector<cv::Point3f> chessboardObjectPoints(cv::Size boardSize, double squareSize);
  
  /**
   * draws a 3d box around a target
   * 
   * @param[out] Image box will be drawn on to
   * @param objectPoints[in] Object(3d) points of the target
   * @param imagePoints Points of the target found in the image
   * @param camera Camera properties used to convert from cam,era pace to real world
   * @param type Type of box that will be draw (`Box`, `Axis`)
   * @param invert weather the box should bet inverted in the z dirstection
   */
  void drawBox(cv::Mat &image, std::vector<cv::Point3f> objectPoints, std::vector<cv::Point2f> imagePoints, rv::Camera camera, int type = 0, bool invert = false);
}
#endif