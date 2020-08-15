#include "hsvTunning.hpp"

void hsvTunningPhotos(rv::Camera &camera) {

  cv::namedWindow(window);

  // Trackbars to set thresholds for HSV values
  cv::createTrackbar("Low H", window, &camera.thresholds.lowH, camera.thresholds.maxH, onLowH, &camera.thresholds);
  cv::createTrackbar("High H", window, &camera.thresholds.highH, camera.thresholds.maxH, onHighH, &camera.thresholds);
  cv::createTrackbar("Low S", window, &camera.thresholds.lowS, camera.thresholds.max, onLowS, &camera.thresholds);
  cv::createTrackbar("High S", window, &camera.thresholds.highS, camera.thresholds.max, onHighS, &camera.thresholds);
  cv::createTrackbar("Low V", window, &camera.thresholds.lowV, camera.thresholds.max, onLowV, &camera.thresholds);
  cv::createTrackbar("High V", window, &camera.thresholds.highV, camera.thresholds.max, onHighV, &camera.thresholds);

  int currentImage = 0;
  int numberOfImages = camera.thresholdingConfig.images.end() - camera.thresholdingConfig.images.begin();

  bool showThresh = true;
  bool changed = true;

  cv::Mat image, imageHSV, thresh, threshColor, display;

  while (true) {

    image = cv::imread(camera.thresholdingConfig.images[currentImage]);
    
    if (image.empty()) {
      std::cerr << "Error: Could not find: " << currentImage << '\n';
      break;
    }

    // Convert from BGR to HSV colorspace
    cv::cvtColor(image, imageHSV, cv::COLOR_BGR2HSV);
    // Detect the object based on HSV Range Values
    cv::inRange(imageHSV, camera.thresholds.lowScalar(), camera.thresholds.highScalar(), thresh);

    cv::cvtColor(thresh, threshColor, cv::COLOR_GRAY2BGR);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(thresh, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    display = showThresh ? threshColor : image;

    for (auto contour = contours.begin(); contour != contours.end(); contour++) {
      std::vector<cv::Point> poly;
      cv::approxPolyDP(*contour, poly, 2, true);
      cv::drawContours(display, std::vector<std::vector<cv::Point>>{poly}, 0, cv::Scalar(255,0,0), 1);
    }

    cv::imshow(window, display);

    // Parse key presses
    int key = cv::waitKey(30);

    currentImage -= (key == 44 && currentImage > 0) ? 1 : 0;
    currentImage += (key == 46 && currentImage < (numberOfImages-1)) ? 1 : 0;

    changed = (key == 44 || key == 46);

    showThresh = (key == 't') ? !showThresh : showThresh;

    if (key == 'q' || key == 27) {
      break;
    }
  }
  cv::destroyAllWindows();
  cv::waitKey(1);
  return;
}

void hsvTunningVideoCapture(rv::Camera &camera) {

  cv::VideoCapture capture(camera.id);

  // Check camera
  if (!capture.isOpened()) {
    std::cerr << "Could access camera with id: " << camera.id << '\n';
    return;
  }

  cv::namedWindow(window);

  // Trackbars to set thresholds for HSV values
  cv::createTrackbar("Low H", window, &camera.thresholds.lowH, camera.thresholds.maxH, onLowH, &camera.thresholds);
  cv::createTrackbar("High H", window, &camera.thresholds.highH, camera.thresholds.maxH, onHighH, &camera.thresholds);
  cv::createTrackbar("Low S", window, &camera.thresholds.lowS, camera.thresholds.max, onLowS, &camera.thresholds);
  cv::createTrackbar("High S", window, &camera.thresholds.highS, camera.thresholds.max, onHighS, &camera.thresholds);
  cv::createTrackbar("Low V", window, &camera.thresholds.lowV, camera.thresholds.max, onLowV, &camera.thresholds);
  cv::createTrackbar("High V", window, &camera.thresholds.highV, camera.thresholds.max, onHighV, &camera.thresholds);
  cv::Mat frame, frameHSV, thresh, threshColor, display;

  bool showThresh = true;

  while (true) {
    capture >> frame;

    // Check camera Data 
    if (frame.empty()) {
      std::cerr << "Lost connection to camera with id: " << camera.id << '\n';
      break;
    }

    // Convert from BGR to HSV colorspace
    cv::cvtColor(frame, frameHSV, cv::COLOR_BGR2HSV);
    // Detect the object based on HSV Range Values
    cv::inRange(frameHSV, camera.thresholds.lowScalar(), camera.thresholds.highScalar(), thresh);

    cv::cvtColor(thresh, threshColor, cv::COLOR_GRAY2BGR);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(thresh, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    display = showThresh ? threshColor : frame;

    for (auto contour = contours.begin(); contour != contours.end(); contour++) {
      std::vector<cv::Point> poly;
      cv::approxPolyDP(*contour, poly, 2, true);
      cv::drawContours(display, std::vector<std::vector<cv::Point>>{poly}, 0, cv::Scalar(255,0,0), 1);
    }

    // Show the frames
    cv::imshow(window, display);

    // Parse key presses
    int key = cv::waitKey(30);

    showThresh = (key == 't') ? !showThresh : showThresh;

    if (key == 'q' || key == 27) {
      break;
    }
  }
  cv::destroyAllWindows();
  capture.release();
  cv::waitKey(1);
  return;
}

void hsvTunning(rv::Camera &camera) {
  return camera.thresholdingConfig.usePhotos ? hsvTunningPhotos(camera) : hsvTunningVideoCapture(camera);
}