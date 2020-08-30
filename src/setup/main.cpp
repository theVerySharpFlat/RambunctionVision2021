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

enum Option {CHOOSE_CAMERA, CAMERA_CALIBRATION, SHOW_UNDISTORTED, HSV_TUNNING, DETECT_TARGETS, SAVE, EXIT};

int main(int argc, char **argv) {
  // Parse arguments
  const std::string keys =
  "{ h ? help usage |            | prints this message              }"
  "{ @input         |  input.xml | output file of vision config     }"
  "{ @output        |            | input file of calibration config }";

  cv::CommandLineParser parser(argc, argv, keys);
  parser.about("\nRambunctionVision2021 v0.0\nVision code for FRC Team Rambunction 4330\n");

  if (parser.has("help")) {
    parser.printMessage();
    return 0;
  } 

  std::string inputFile = parser.get<std::string>(0);
  std::string outputFile = parser.get<std::string>(1);

  outputFile = outputFile == "" ? inputFile : outputFile;


  if (!parser.check()) {
      parser.printErrors();
      return 0;
  }

  std::vector<rv::Camera> cameras;
  std::vector<rv::Target> globalTargets;

  cv::FileStorage fs(inputFile, cv::FileStorage::READ);
  fs["Cameras"] >> cameras;
  fs["GlobalTargets"] >> globalTargets;
  fs.release();

  bool saved = true;

  rv::Camera &currentCamera = cameras[0];

  while (true) {  
    // Show message
    std::cout << "\nCurrent Camera: " << currentCamera.id << '\n';
    std::cout << "\nOptions:\n";
    std::cout << "1. Choose Camera\n";
    std::cout << "2. Calibrate Camera\n";
    std::cout << "3. Show Undistorted\n";
    std::cout << "4. HSV Tunning\n";
    std::cout << "5. Detect Targets\n";
    std::cout << "6. Save\n";
    std::cout << "7. Exit\n";
    std::cout << "\nEnter option number: ";
    int x;
    std::cin >> x;
    std::cin.clear(); 
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    Option choice = static_cast<Option>(x-1); 

    switch (choice) {
      case CHOOSE_CAMERA: {
        int id;
        std::cout << "\nPossible Cameras:\n";

        for (rv::Camera &camera : cameras) {
          std::cout << camera.id << '\n';
        }

        std::cout << "\nEnter Camera id: ";
        std::cin  >> id;

        auto newCamera = std::find_if(cameras.begin(), cameras.end(), [i = id] (rv::Camera c) { return c.id == i; });

        if (newCamera != cameras.end()) currentCamera = *newCamera;

        break;
      } case CAMERA_CALIBRATION: {
        saved = false;
        std::cout << "\nRunning Camera Calibration ...\n";
        calibrate(currentCamera);
        break;
      } case SHOW_UNDISTORTED: {
        std::cout << "\nShowing Undistorted ...\n";
        showUndistorted(currentCamera);
        break;
      } case HSV_TUNNING: {
        saved = false;
        std::cout << "\nRunning HSV Tunning ...\n";
        hsvTunning(currentCamera);
        break;
      } case DETECT_TARGETS: {
        std::cout << "\nDetecting Targets ...\n";
        detectTargets(currentCamera, globalTargets);
        break;
      } case SAVE: {
        // Save to file
        saved = true;
        fs.open(outputFile, cv::FileStorage::WRITE);
        fs << "Cameras" << cameras;
        fs << "GlobalTargets" << globalTargets;
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
