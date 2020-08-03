#include "RambunctionVision/detection.hpp"

#include <string>
#include <vector>
#include <math.h>

#include <opencv2/imgproc.hpp>

#include "RambunctionVision/poseEstimation.hpp"

namespace rv {

  cv::Mat getPerspectiveRotationMatrix(cv::Point center, double angle) {
    cv::Mat matrix(3,3, CV_64FC1);

    // sin and cos values for the image
    double c = cos(angle * (M_PI /180));
    double s = sin(angle * (M_PI /180));
  
    // generates 3x3 2d roatation matrix
    matrix.at<double>(0,0) = c;
    matrix.at<double>(0,1) = s;
    matrix.at<double>(0,2) = ((1-c) * center.x) - (s * center.y);
    matrix.at<double>(1,0) = -s;
    matrix.at<double>(1,1) = c;
    matrix.at<double>(1,2) = (s * center.x) + ((1-c) * center.y);
    matrix.at<double>(2,0) = 0;
    matrix.at<double>(2,1) = 0;
    matrix.at<double>(2,2) = 1;
    return matrix;
  }

  void reorderNearestPoint(std::vector<cv::Point2f> &points, cv::Point2f nearestPoint) {
    std::vector<cv::Point2f>::iterator nearest = points.begin();
    double best = std::numeric_limits<double>::infinity();

    // Iterates through points to find wich is closest to "nearestPoint"
    for (auto point = points.begin(); point != points.end(); point++) {
      cv::Point diff = nearestPoint - *point;
      double dist = std::sqrt((diff.x * diff.x) + (diff.y * diff.y)); 

      nearest = dist < best ? point : nearest;
      best = std::min(dist, best);
    }

    // Rotates vector to begin with the closest point
    std::rotate(points.begin(), nearest, points.end());
  }

  void normalizeContour(std::vector<cv::Point> contour, cv::Mat &image, cv::Mat &transform) {
    cv::Mat contourImage(2048, 2048, CV_8UC1, cv::Scalar(0)), projectedImage(300, 300, CV_8UC1, cv::Scalar(0)), boxPoints;
    std::vector<cv::Point2f> dstPoints;
      dstPoints.push_back({0,   300});
      dstPoints.push_back({0,   0  });
      dstPoints.push_back({300, 0  });
      dstPoints.push_back({300, 300});

    // Draws contours onto binary image
    cv::drawContours(contourImage, std::vector<std::vector<cv::Point>>{contour}, 0, 255, cv::FILLED);

    // Finds the rectangle of best fit
    cv::RotatedRect box = cv::minAreaRect(contour);
    cv::boxPoints(box, boxPoints);
    transform = cv::getPerspectiveTransform(boxPoints, dstPoints);

    // Warps binary image such that the bounding rectangle becomes the fram of the 300x300 image
    cv::warpPerspective(contourImage, projectedImage, transform, {300, 300}, cv::INTER_NEAREST);
    image = projectedImage;
  }

  double compareImages(cv::Mat image1, cv::Mat image2, cv::Mat &rotation) {
    cv::Mat overlap(300,300,CV_8UC1), overhang(300,300,CV_8UC1), image2Temp, rotationTemp;
    double best = std::numeric_limits<double>::infinity();
    double bestMatch = std::numeric_limits<double>::infinity();

    // Iterates through possible rotations of image 2 to find the one with the least error
    for (int angle = 0; angle < 360; angle += 90) {
      // rotates image 2 by an angel
      rotationTemp = getPerspectiveRotationMatrix({150, 150}, angle);
      cv::warpPerspective(image2, image2Temp, rotationTemp, {300,300}, cv::INTER_NEAREST);

      // Computes the overlap(and) and overhang(xor) of the images 
      cv::bitwise_and(image2Temp, image1, overlap);
      cv::bitwise_xor(image2Temp, image1, overhang);

      // Error is calcultesd by overhang / overlap (snaller is better)
      double error = (double)cv::countNonZero(overhang) / (double)cv::countNonZero(overlap);

      double match = cv::matchShapes(image2Temp, image1, 1, 0.0);

      // Determins witch rotation is best
      rotation = (error < best) ? rotationTemp : rotation; 
      best = std::min(best, error);
      bestMatch = std::min(bestMatch, match);
  }
  return bestMatch;
}

  void matchTargets(std::vector<rv::Target> targets, std::vector<std::vector<cv::Point>> contours, std::vector<rv::Target> &found) {
    
    // Iterates through contours to determ whitch targets is the best match for each one 
    for (auto contour = contours.begin(); contour != contours.end(); contour++) {
      cv::Mat contourImage(300, 300, CV_8UC1), transform;

      // Normalizes the contour into a 300x300 binary image
      normalizeContour(*contour, contourImage, transform);
      
      //Iterates through Targets to see wich matches the contour best
      double best = 5;
      for (auto target = targets.begin(); target != targets.end(); target++) {
        cv::Mat targetImage, targetTransform, rotation(3,3, CV_64FC1);

        // Normalized the target into a 300x300 binary image and compares it to the normalized contours
        normalizeContour(target->shape, targetImage, targetTransform);
        double error = compareImages(targetImage, contourImage, rotation);

        // Determines if this is the best match
        if (error < best) {
          best = error;

          std::vector<cv::Point2f> polygon;

          // Reorders the contour poinst so that the fist point corosponds to the first point of the target
          cv::perspectiveTransform(asPointFloat(*contour), polygon, rotation * transform);
          cv::approxPolyDP(polygon, polygon, 30, true);
          cv::cornerSubPix(contourImage, polygon, {5, 5}, {-1,-1}, {cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 40, 0.001});
          reorderNearestPoint(polygon, {300,300});
          cv::perspectiveTransform(polygon, polygon, transform.inv() * rotation.inv());

          if ((polygon.end() - polygon.begin ()) == (target->shape.end() - target->shape.begin()) && cv::contourArea(polygon) > 30) {
            // Lables contour and adds it to the output vector 
            rv::Target match;
            std::transform(polygon.begin(), polygon.end(), std::back_inserter(match.shape), [](cv::Point2f point) -> cv::Point {
              return cv::Point(point.x, point.y); });
            match.name = target->name;
            found.push_back(match);
          }
        }
      }
    }
  }
}


