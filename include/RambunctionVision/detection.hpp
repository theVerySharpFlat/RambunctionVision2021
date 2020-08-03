#ifndef DETECTION_HPP
#define DETECTION_HPP

#include <vector>

#include <opencv2/core.hpp>

#include "RambunctionVision/target.hpp"

namespace rv {
    /** 
    * Creates a perspective rotation matrix
    * 
    * @param center[in] Center point of the rotation
    * @param angle[in] Angle in degrees of rotation counter clockwise
    * @return 3x3 Perspective transformation matrix
    */
    cv::Mat getPerspectiveRotationMatrix(cv::Point center, double angle);

    /**
    * Reorders vector of points to begin with point closest to another given point
    *
    * @param points[in, out] vector of points to be reordered
    * @param nearestPoint[in] Point use as refrence for reordering
    */
    void reorderNearestPoint(std::vector<cv::Point2f> &points, cv::Point2f nearestPoint);

    /**
    * Normalizess contour into a 300x300 binary image
    * 
    * @param contour[in] Vector of points to be normalized
    * @param image[out] 300x300 normalized binary image
    * @param transform[out] perspective transformation matrix used to normalize the contours
    * */
    void normalizeContour(std::vector<cv::Point> contour, cv::Mat &image, cv::Mat &transform);

    /**
     * Compares two 300x300 binary images
     * 
     * @param image1[in] First image to compare
     * @param image2[in] Second image to compare
     * @param rotation[out] 3x3 perspective matrix representing the orientation
     *   of the second image that yeilds the least error
     * @return The error between the images, calculated by the overhang / overlap of the images 
    */
    double compareImages(cv::Mat image1, cv::Mat image2, cv::Mat &rotation);

    /**
     * Finds the targets (if any) that best match a vector of contours
     * 
     * @param contours[in] vector of vectors of points fro `cv::findContours()`
     * @param targets[in] vector of targets that the contours can match to
     * @param found[out] vector of targets holding the corrected (same size as the target) contours lables with the name of the target tghey matched with (if they matched with any)
    */
    void matchTargets(std::vector<rv::Target> targets, std::vector<std::vector<cv::Point>> contours, std::vector<rv::Target> &found);
}
#endif