#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <vector>
#include <map>
#include <thread>
#include <mutex>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/calib3d.hpp>

#include "RambunctionVision/thresholding.hpp"
#include "RambunctionVision/comms.hpp"
#include "RambunctionVision/camera.hpp"
#include "RambunctionVision/target.hpp"
#include "RambunctionVision/detection.hpp"
#include "RambunctionVision/poseEstimation.hpp"

std::mutex mutex;
std::map<std::string, std::map<std::string, std::string>> data;

void handleClient(rv::Comms comms, std::map<std::string, std::map<std::string, std::string>> &data, std::mutex &mutex);
void detection(rv::Camera camera, std::vector<rv::Target> globalTargets, std::map<std::string, std::map<std::string, std::string>> &data, std::mutex &mutex);



int main(int argc, char **argv) {
  const std::string keys = 
  "{help h usage ? |            | shows this message }"
  "{ port p        |    4330    | vision config file }"
  "{ @config       | output.xml | vision config file }";

  cv::CommandLineParser parser(argc, argv, keys);
  parser.about("Rambunction Visoon v0.0\nvison software for FRC team Rambunctio 4330 for the 2021 season\n");

  if (parser.has("help")) {
    parser.printMessage();
    return 0;
  }

  std::string visionConfigFile = parser.get<std::string>(0);
  int port = parser.get<int>("port");

  if (!parser.check()) {
    parser.printErrors();
    return -1;
  }

  std::vector<rv::Camera> cameras;
  std::vector<rv::Target> globalTargets;
  cv::FileStorage fs(visionConfigFile, cv::FileStorage::READ);
  fs["Cameras"] >> cameras;
  fs["GlobalTargets"] >> globalTargets;
  fs.release();

  for (rv::Camera &camera : cameras) {
    std::thread detectionThread(detection, camera, globalTargets, std::ref(data), std::ref(mutex));
    detectionThread.detach();
  }

  rv::Listener listener(port, 5);
  while(true) {
    rv::Comms comms;
    comms = listener.waitForConnection();

    std::cout << "connection Made!\n";

    std::thread handleThread(handleClient, comms, std::ref(data), std::ref(mutex));
    handleThread.detach();
  }
  return 0;
}

void detection(rv::Camera camera, std::vector<rv::Target> globalTargets, std::map<std::string, std::map<std::string, std::string>> &data, std::mutex &mutex) {
  std::vector<rv::Target> targets = globalTargets;
  targets.reserve(globalTargets.size() + camera.targets.size());
  targets.insert(targets.end(), camera.targets.begin(), camera.targets.end());


  cv::VideoCapture capture(camera.id);

  if (!capture.isOpened()) {
    std::cout << "Could not open video capture on:" << camera.id << '\n';
    return;
  }

  mutex.lock();
    data["camera" + std::to_string(camera.id)]["id"] = std::to_string(camera.id);
    data["camera" + std::to_string(camera.id)]["rawFPS"] = std::to_string(capture.get(cv::CAP_PROP_FPS));
    data["camera" + std::to_string(camera.id)]["size"] = "{" + std::to_string(capture.get(cv::CAP_PROP_FRAME_HEIGHT)) + "," + std::to_string(capture.get(cv::CAP_PROP_FRAME_WIDTH)) + "}";
    data["camera" + std::to_string(camera.id)]["offset"] = "{" + std::to_string(camera.offset.x) + "," + std::to_string(camera.offset.y) + "," + std::to_string(camera.offset.z) + "}";
    data["camera" + std::to_string(camera.id)]["angleOffset"] = "{" + std::to_string(camera.angleOffset[0]) + "," + std::to_string(camera.angleOffset[1]) + "," + std::to_string(camera.angleOffset[3]) + "}";
    data["camera" + std::to_string(camera.id)]["thresholdsH"] = "{" + std::to_string(camera.thresholds.lowH) + "," + std::to_string(camera.thresholds.highH) + "}";
    data["camera" + std::to_string(camera.id)]["thresholdsS"] = "{" + std::to_string(camera.thresholds.lowS) + "," + std::to_string(camera.thresholds.highS) + "}";
    data["camera" + std::to_string(camera.id)]["thresholdsV"] = "{" + std::to_string(camera.thresholds.lowV) + "," + std::to_string(camera.thresholds.highV) + "}";
    data["camera" + std::to_string(camera.id)]["fx"] = std::to_string(camera.matrix.at<double>(0,0));
    data["camera" + std::to_string(camera.id)]["fy"] = std::to_string(camera.matrix.at<double>(1,1));
    data["camera" + std::to_string(camera.id)]["snapshot"] = "0";
    data["camera" + std::to_string(camera.id)]["startRecording"] = "0";
    data["camera" + std::to_string(camera.id)]["stopRecording"] = "0";
  mutex.unlock();

  // bool recording = false;

  while (true) {
    cv::Mat frame, hsv, thresh;

    capture >> frame;

    if (frame.empty()) {
      std::cout << "Lost connection to camera\n";
      return;
    }

    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

    cv::inRange(hsv, camera.thresholds.lowScalar(), camera.thresholds.highScalar(), thresh);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<rv::Target> found;
    cv::findContours(thresh, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);


    rv::matchTargets(targets, contours, found);

    for (rv::Target &find :  found) {
      cv::Mat rvec, tvec, rotation, transform, corrected, mtxR, mtxQ;

      std::vector<cv::Point2f> undistortedPoints;
      cv::undistortPoints(rv::asPointFloat(find.shape), undistortedPoints, camera.matrix, camera.dst);

      cv::RotatedRect box = cv::minAreaRect(undistortedPoints);
      cv::Rect2f roughBox = box.boundingRect2f();

      auto target = std::find_if(targets.begin(), targets.end(), [f = find](rv::Target t) {return t.name == f.name; });
      cv::solvePnP(rv::asObjectPoints(target->shape), rv::asPointFloat(find.shape), camera.matrix, camera.dst, rvec, tvec);
      
      cv::Rodrigues(rvec, rotation);
      cv::hconcat(rotation, tvec, transform);
      double lastRow[4] = {0.0,0.0,0.0,1.0};
      cv::vconcat(transform, cv::Mat(1,4, CV_64F, &lastRow), transform);

      corrected = camera.offsetMatrix() * transform;

      tvec = corrected.rowRange(0,3).colRange(3,4);
      cv::Vec3d angles = cv::RQDecomp3x3(corrected.rowRange(0, 3).colRange(0,3), mtxR, mtxQ);

      mutex.lock(); 
        data[find.name + std::to_string(camera.id)]["name"] = find.name;
        data[find.name + std::to_string(camera.id)] ["matchError"] = std::to_string(find.error);
        data[find.name + std::to_string(camera.id)]["rawPose"] = "{" + std::to_string(box.center.x - camera.matrix.at<double>(0,2)); + "," + std::to_string(box.center.y - camera.matrix.at<double>(1,2)) + "}";
        data[find.name + std::to_string(camera.id)]["rawArea"] = std::to_string(cv::contourArea(find.shape));
        data[find.name + std::to_string(camera.id)]["boxSize"] = "{" + std::to_string(box.size.height) + "," + std::to_string(box.size.width) + "}";
        data[find.name + std::to_string(camera.id)]["boxArea"] = std::to_string(box.size.area());
        data[find.name + std::to_string(camera.id)]["boxSkew"] = std::to_string(box.angle);
        data[find.name + std::to_string(camera.id)]["xAngle"] = std::to_string(-atan((box.center.x - camera.matrix.at<double>(0,2) / camera.matrix.at<double>(0,0)) * (180/M_PI) + camera.angleOffset[0]));
        data[find.name + std::to_string(camera.id)]["yAngle"] = std::to_string(-atan((box.center.y - camera.matrix.at<double>(1,2) / camera.matrix.at<double>(1,1)) * (180/M_PI) + camera.angleOffset[1]));
        data[find.name + std::to_string(camera.id)]["roughSize"] = "{" + std::to_string(roughBox.height) + "," + std::to_string(roughBox.width);
        data[find.name + std::to_string(camera.id)]["roughtArea"] = std::to_string(roughBox.area());
        data[find.name + std::to_string(camera.id)]["3dPose"] = "{" + std::to_string(tvec.at<double>(0,0)) + "," + std::to_string(tvec.at<double>(0,1)) + "," + std::to_string(tvec.at<double>(0,2)) + "}";
        data[find.name + std::to_string(camera.id)]["3dAngle"] = "{" + std::to_string(angles[0]) + "," + std::to_string(angles[1]) + "," + std::to_string(angles[2]) + "}";
      mutex.unlock();
    }
  }
}

void handleClient(rv::Comms comms, std::map<std::string, std::map<std::string, std::string>> &data, std::mutex &mutex) {
  while (true) {
    rv::Request request, reply;

    try { 
      request = comms.recive(); 
    } catch (const std::exception &error) { 
      if (std::string(error.what()) == "Invalid Formatt!") {
        try { 
          comms.send(rv::Request("error", "", "", "Invalid Formatt"));
        } catch (const std::exception &err) {
          std::cout << err.what() << '\n';
          std::cout << "Lost connection!\n";
          break;
        } 
      }
      continue;
    }

    mutex.lock();
    if (data.find(request.subject) != data.end()) {
      if (data[request.subject].find(request.key) != data[request.subject].end()) {
        if (request.action == "get") {
          reply = rv::Request("data", request.subject, request.key, data[request.subject][request.key]);
        } else if (request.action == "set") {
          data[request.subject][request.key] = request.value;
          reply = rv::Request("data", request.subject, request.key,  data[request.subject][request.key]);
        } else if (request.action == "error") {
          std::cout << "Comms error: " << request.value << '\n';
        } else {
          reply = rv::Request("error", request.subject, request.key, "Inavlid Action");
          std::cout << "Invalid Action!\n";
        }
      } else {
        reply = rv::Request("error", request.subject, request.key, "Invaid Key");
        std::cout << "Invalid Key!\n";
      }
    } else {
      reply = rv::Request("error", request.subject, request.key, "Invalid Subject");
      std::cout << "Invalid Subject!\n";
    }
    mutex.unlock();

    try { 
      comms.send(reply);
    } catch (const std::exception &error) {
      std::cout << error.what() << '\n';
      std::cout << "Lost connection!\n";
      break;
    }
  }
  std::cout << "Connection closed!\n";
  comms.closeConnection();
} 