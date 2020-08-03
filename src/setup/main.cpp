#include <iostream>
#include <string>

#include <opencv2/core.hpp>

#include "RambunctionVision/config.hpp"
#include "RambunctionVision/camera.hpp"
#include "RambunctionVision/thresholding.hpp"
#include "RambunctionVision/target.hpp"

#include "hsvTunning.hpp"
#include "calibrate.hpp"
#include "detectTargets.hpp"

enum Option {CAMERA_CALIBRATION, SHOW_UNDISTORTED, HSV_TUNNING, DETECT_TARGETS, SAVE, EXIT};

int main(int argc, char **argv) {
  // Parse arguments
  const std::string keys =
  "{ h ? help usage   |                     | prints this message                  }"
  "{ @output          |    output.xml       | output file of vision config         }"
  "{ @setupConfig     |   setupConfig.xml   | input file of calibration config     }"
  "{ @startingPoint   | visionConfig.xml    | starting point for the visionConfig  }";

  cv::CommandLineParser parser(argc, argv, keys);
  parser.about("\nRambunctionVision2021 v0.0\nVision code for FRC Team Rambunction 4330\n");

  if (parser.has("help")) {
    parser.printMessage();
    return 0;
  } 

  std::string outputFile = parser.get<std::string>(0);
  std::string setupConfigFile = parser.get<std::string>(1);
  std::string startingPointFile = parser.get<std::string>(2);

  if (!parser.check()) {
      parser.printErrors();
      return 0;
  }

  // Read in from files
  rv::CalibrateConfig calibrateConfig;
  rv::ThresholdingConfig thresholdingConfig;

  cv::FileStorage fs(setupConfigFile, cv::FileStorage::READ);
  fs["CalibrateConfig"] >> calibrateConfig;
  fs["ThresholdingConfig"] >> thresholdingConfig;
  fs.release();

  rv::HSV thresholds;
  rv::Camera camera ;
  std::vector<rv::Target> targets;

  fs.open(startingPointFile, cv::FileStorage::READ);
  fs["Thresholds"] >> thresholds;
  fs["Camera"] >> camera;
  fs["Targets"] >> targets;
  fs.release();

  bool saved = true;

  while (true) {  
    // Show message
    std::cout << "\nOptions:\n";
    std::cout << "1. Calibrate Camera\n";
    std::cout << "2. Show Undistorted\n";
    std::cout << "3. HSV Tunning\n";
    std::cout << "4. Detect Targets\n";
    std::cout << "5. Save\n";
    std::cout << "6. Exit\n";
    std::cout << "\nEnter option number: ";
    int x;
    std::cin >> x;
    std::cin.clear(); 
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    Option choice = static_cast<Option>(x-1); 

    switch (choice) {
      case CAMERA_CALIBRATION: {
        saved = false;
        std::cout << "\nRunning Camera Calibration ...\n";
        calibrate(calibrateConfig, camera);
        break;
      }
      case SHOW_UNDISTORTED: {
        std::cout << "\nShowing Undistorted ...\n";
        showUndistorted(calibrateConfig, camera);
        break;
      }
      case HSV_TUNNING: {
        saved = false;
        std::cout << "\nRunning HSV Tunning ...\n";
        hsvTunning(thresholds, camera, thresholdingConfig);
        break;
      }
      case DETECT_TARGETS: {
        std::cout << "\nDetecting Targets ...\n";
        detectTargets(camera, thresholds, targets, thresholdingConfig);
        break;
      }
      case SAVE: {
        // Save to file
        saved = true;
        fs.open(outputFile, cv::FileStorage::WRITE);
        fs << "Thresholds" << thresholds;
        fs << "Camera" << camera;
        fs << "Targets" << targets;
        fs.release();
      }
        break;
      case EXIT: {
        // Cheack if progress has been saved
        if (!saved) {
          std::string sure;
          std::cout << "\nYour progrss will NOT be saved UNLESS you run the \"Save\" comand. Are you sure you want to continue (yes or no): ";
          std::cin >> sure;
          std::cout << "\n";
          std::cin.clear(); 
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
          if (sure != "yes" && sure != "y" && sure != "Y") {
            break;
          }
        }
        return 0;
      }
      default: {
        std::cout << "\nInvalid Option! Please try again.\n"; 
        break;
      }
    }
  }
}
