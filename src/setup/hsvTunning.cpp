#include "hsvTunning.hpp"

void hsvTunningPhotos(rv::HSV &hsv, rv::Camera camera, rv::ThresholdingConfig config) {

  cv::namedWindow(window);

  // Trackbars to set thresholds for HSV values
  cv::createTrackbar("Low H", window, &hsv.lowH, hsv.maxH, onLowH, &hsv);
  cv::createTrackbar("High H", window, &hsv.highH, hsv.maxH, onHighH, &hsv);
  cv::createTrackbar("Low S", window, &hsv.lowS, hsv.max, onLowS, &hsv);
  cv::createTrackbar("High S", window, &hsv.highS, hsv.max, onHighS, &hsv);
  cv::createTrackbar("Low V", window, &hsv.lowV, hsv.max, onLowV, &hsv);
  cv::createTrackbar("High V", window, &hsv.highV, hsv.max, onHighV, &hsv);

  int currentImage = 0;
  int numberOfImages = config.images.end() - config.images.begin();

  bool showThresh = true;
  bool changed = true;

  cv::Mat image, imageHSV, thresh, threshColor, display;

  while (true) {

    image = cv::imread(config.images[currentImage]);
    
    if (image.empty()) {
      std::cerr << "Error: Could not find: " << currentImage << '\n';
      break;
    }

    // Convert from BGR to HSV colorspace
    cv::cvtColor(image, imageHSV, cv::COLOR_BGR2HSV);
    // Detect the object based on HSV Range Values
    cv::inRange(imageHSV, hsv.lowScalar(), hsv.highScalar(), thresh);

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

void hsvTunningVideoCapture(rv::HSV &hsv, rv::Camera camera) {

  cv::VideoCapture capture(camera.id);

  // Check camera
  if (!capture.isOpened()) {
    std::cerr << "Could access camera with id: " << camera.id << '\n';
    return;
  }

  cv::namedWindow(window);

  // Trackbars to set thresholds for HSV values
  cv::createTrackbar("Low H", window, &hsv.lowH, hsv.maxH, onLowH, &hsv);
  cv::createTrackbar("High H", window, &hsv.highH, hsv.maxH, onHighH, &hsv);
  cv::createTrackbar("Low S", window, &hsv.lowS, hsv.max, onLowS, &hsv);
  cv::createTrackbar("High S", window, &hsv.highS, hsv.max, onHighS, &hsv);
  cv::createTrackbar("Low V", window, &hsv.lowV, hsv.max, onLowV, &hsv);
  cv::createTrackbar("High V", window, &hsv.highV, hsv.max, onHighV, &hsv);
  cv::Mat frame, frameHSV, thresh;

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
    cv::inRange(frameHSV, hsv.lowScalar(), hsv.highScalar(), thresh);
    // Show the frames
    cv::imshow(window, frame);

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

void hsvTunning(rv::HSV &hsv, rv::Camera camera, rv::ThresholdingConfig config) {
  return config.usePhotos ? hsvTunningPhotos(hsv, camera, config) : hsvTunningVideoCapture(hsv, camera);
}