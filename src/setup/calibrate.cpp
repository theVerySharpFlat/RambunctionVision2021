#include "calibrate.hpp"

#include <iostream>
#include <chrono>
#include <string>

#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include "RambunctionVision/config.hpp"
#include "RambunctionVision/poseEstimation.hpp"

cv::Mat getUndistortedImage(cv::Mat image, rv::Camera &camera, bool undistortImage, bool showBox, bool useAxis) {
  cv::Mat shown;

  // Finds and conditianalu draws chessboard corrners
  std::vector<cv::Point2f> corners;
  bool foundChessboard = cv::findChessboardCorners(image, camera.calibrateConfig.boardSize, corners, 
    cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE + cv::CALIB_CB_FAST_CHECK);

  if (showBox && !camera.matrix.empty() && !camera.dst.empty() && foundChessboard) {
    rv::drawBox(image, rv::chessboardObjectPoints(camera.calibrateConfig.boardSize, camera.calibrateConfig.squareSize) , corners, camera, useAxis ? 1 : 0, true);
  } else {
    cv::drawChessboardCorners(image, camera.calibrateConfig.boardSize, corners, foundChessboard);
  }

  // Conditioanly undistortes the image
  if (undistortImage && !camera.matrix.empty() && !camera.dst.empty()) {
    cv::undistort(image, shown, camera.matrix, camera.dst);
  } else {
    shown = image;
  }
  return shown;
}

void showUndistortedVideoCapture(rv::Camera &camera) {
  cv::VideoCapture capture (camera.id);

  // Check Camera
  if (!capture.isOpened()) {
    std::cerr << "Could access camera with id: " << camera.id << '\n';
    return;
  }

  const std::string windowName = "Undistorted"; 

  bool undistortImage = true, showBox = false, useAxis = false;

  while (true) {
    cv::Mat frame, display;
    capture >> frame;

    // Check Data
    if (frame.empty()) {
      std::cerr << "Lost connection to camera with id: " << camera.id << '\n';
      break;
    }

    // Conditiaonly undistorts the image
    display = getUndistortedImage(frame, camera, undistortImage, showBox, useAxis);

    cv::imshow(windowName, display);

    // Parses Key presses
    int key = cv::waitKey(30);
    undistortImage = (key == 'd') ? !undistortImage : undistortImage;
    showBox = (key == 'b') ? !showBox : showBox;
    useAxis = (key == 'a') ? !useAxis : useAxis;

    if (key == 'q' || key == 27) {
      cv::destroyAllWindows();
      cv::waitKey(1);
      return;
    }
  }
}

void showUndistortedPhotos(rv::Camera &camera) {
  const std::string window = "Undistorted";
  cv::namedWindow(window);
  
  int numberOfImages = camera.calibrateConfig.images.end() - camera.calibrateConfig.images.begin();
  int currentImage = 0;

  bool undistortImage = true, showBox = false, useAxis = false;

  while (true) {
    cv::Mat image = cv::imread(camera.calibrateConfig.images[currentImage]);

    // Checks Image Data
    if (image.empty()) {
      std::cerr << "Error: Could not find: " << currentImage << '\n';
      break;
    }

    // Conditiaonly undistorts image
    cv::Mat display = getUndistortedImage(image, camera, undistortImage, showBox, useAxis);
    cv::imshow(window, display);

    // Parses hey presses
    int key = cv::waitKey(0);

    currentImage -= (key == 44 && currentImage > 0) ? 1 : 0;
    currentImage += (key == 46 && currentImage < (numberOfImages-1)) ? 1 : 0;

    undistortImage = (key == 'd') ? !undistortImage : undistortImage;
    showBox = (key == 'b') ? !showBox : showBox;
    useAxis = (key == 'a') ? !useAxis : useAxis;


    if (key == 'q' || key == 27) {
      break;
    }
  }
  cv::destroyAllWindows();
  cv::waitKey(1);
  return;
}

void showUndistorted(rv::Camera &camera) {
  return (camera.calibrateConfig.usePhotos) ? showUndistortedPhotos(camera) : showUndistortedVideoCapture(camera);
}

void calibrateFromVideoCapture(rv::Camera &camera) {
  const std::string windowName = "Camera Calibration";
  cv::namedWindow(windowName);
  cv::VideoCapture capture(camera.id);

  // Cheaks Camera
  if (!capture.isOpened()) {
    std::cerr << "Could access camera with id: " << camera.id << '\n';
    return;
  }

  std::vector<std::vector<cv::Point2f>> imagePoints;
  cv::Size imageSize;

  auto lastPointCapture = std::chrono::high_resolution_clock::now();

  int count = 0;
  while(count < camera.calibrateConfig.numberOfImages) {
    cv::Mat frame;
    capture >> frame;

    // Cheaks Data
    if (frame.empty()) {
      std::cerr << "Lost connection to camera with id: " << camera.id << '\n';
      break;
    }
    
    // Fins and draws Chessboars Corners
    std::vector<cv::Point2f> corners;
    bool foundChessboard = cv::findChessboardCorners(frame, camera.calibrateConfig.boardSize, corners, 
      cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE + cv::CALIB_CB_FAST_CHECK);
    cv::drawChessboardCorners(frame, camera.calibrateConfig.boardSize, corners, foundChessboard);
    
    // Calculates duration board has been in view to compare against delay
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastPointCapture);

    if (!foundChessboard) {
      lastPointCapture = now;
    }

    int key = cv::waitKey(30);

    // Captures Points if delay period has passed or space bar is pressed
    if(foundChessboard && ((camera.calibrateConfig.automatic && duration.count() >= camera.calibrateConfig.delay) || key == 32)) {
      lastPointCapture = now;
      imagePoints.push_back(corners);
      frame += cv::Scalar(255, 255, 255);
      imageSize = frame.size();
      count++;
    }

    cv::imshow(windowName, frame);

    if (key == 'q' || key == 27) {
      cv::destroyAllWindows();
      cv::waitKey(1);
      return;
    }
  }
  cv::waitKey(1);
  cv::destroyAllWindows();
  cv::waitKey(1);


  // Calculates vector of object points for the chessboard
  std::vector<std::vector<cv::Point3f>> objectPoints;
  for (auto i = imagePoints.begin(); i != imagePoints.end(); i++) {
    objectPoints.push_back(rv::chessboardObjectPoints(camera.calibrateConfig.boardSize, camera.calibrateConfig.squareSize));
  }

  // Runs calibration
  double error;
  cv::Mat cameraMatrix;
  cv::Mat dst;
  cv::Mat rvecs;
  cv::Mat tvecs;
  error = cv::calibrateCamera(objectPoints, imagePoints, imageSize, cameraMatrix, dst, rvecs, tvecs);

  // Stores results
  camera.matrix = cameraMatrix;
  camera.dst = dst;

  // Print results
  std::cout << "\nCamera Matrix:\n" << cameraMatrix << "\n";
  std::cout << "\nDst Coefficents:\n" << dst << "\n";

  // Display results
  showUndistortedVideoCapture(camera);
  return;
}

void calibrateFromPhotos(rv::Camera &camera) {
  std::vector<std::vector<cv::Point2f>> imagePoints;
  cv::Size imageSize;

  // Iterate through images to collect points
  for (auto file = camera.calibrateConfig.images.begin(); file != camera.calibrateConfig.images.end(); file++) {
    cv::Mat image = cv::imread(*file);

    // Cheaks Image Data
    if (image.empty()) {
      std::cerr << "Error: Could not find: " << *file << '\n';
      break;
    }

    // Find chessboad points in image
    bool foundChessboard;
    std::vector<cv::Point2f> points;
    foundChessboard = cv::findChessboardCorners(image, camera.calibrateConfig.boardSize, points, 
      cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE + cv::CALIB_CB_FAST_CHECK);

    if(foundChessboard) {
      imagePoints.push_back(points);
      imageSize = image.size();
    }
  }

  // Clalculates vector of object points for chessboard
  std::vector<std::vector<cv::Point3f>> objectPoints;
  for (auto i = imagePoints.begin(); i != imagePoints.end(); i++) {
    objectPoints.push_back(rv::chessboardObjectPoints(camera.calibrateConfig.boardSize, camera.calibrateConfig.squareSize));
  }

  // Run calibration
  double error;
  cv::Mat cameraMatrix;
  cv::Mat dst;
  cv::Mat rvecs;
  cv::Mat tvecs;
  error = cv::calibrateCamera(objectPoints, imagePoints, imageSize, cameraMatrix, dst, rvecs, tvecs);

  // Store results
  camera.matrix = cameraMatrix;
  camera.dst = dst;

  // Output Results
  std::cout << "\nCamera Matrix:\n" << cameraMatrix << "\n";
  std::cout << "\nDst Coefficents:\n" << dst << "\n";

  // Show Results
  showUndistortedPhotos(camera);
}

void calibrate(rv::Camera &camera) {
  return camera.calibrateConfig.usePhotos ? calibrateFromPhotos(camera) : calibrateFromVideoCapture(camera);
}