// #include <sys/types.h>
// #include <unistd.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <string.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include <pthread.h>
// #include <semaphore.h>
// #include <iostream>
// #include <algorithm>
// #include <mutex>
// #include <string>
// #include <thread>

// #include <opencv2/opencv.hpp>

// #include "visionUtils.hpp"


// #define PORTNUMBER  9001 
// #define DONOTKNOW 10000000

// //pthread_mutex_t dataLock;
// std::mutex dataMutex;

// // forward declaration of functions
// void handleClient(int&);
// void receiveNextCommand(char*, int);
// void capture(int& arg);

int main(void)
{
  // int n, s;
  // socklen_t len;
  // int max;
  // int number;
  // struct sockaddr_in name;
  // //pthread_mutex_init(&dataLock, NULL);
  // //mutex = std::mutex();

  // // create the socket
  // if ( (s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
  // perror("socket");
  // exit(1);
  // }

  // memset(&name, 0, sizeof(struct sockaddr_in));
  // name.sin_family = AF_INET;
  // name.sin_port = htons(PORTNUMBER);
  // len = sizeof(struct sockaddr_in);

  // // listen on all network interfaces
  // n = INADDR_ANY;
  // memcpy(&name.sin_addr, &n, sizeof(long));

  // int reuse = 1;
  // if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0) {
  // perror("setsockopt(SO_REUSEADDR)");
  // exit(1);
  // }

  // if (setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, (const char*)&reuse, sizeof(reuse)) < 0) {
  // perror("setsockopt(SO_KEEPALIVE)");
  // exit(1);
  // }

  // // bind socket the network interface
  // if (bind(s, (struct sockaddr *) &name, len) < 0) {
  // perror("bind");
  // exit(1);
  // }

  // // listen for connections
  // if (listen(s, 5) < 0) {
  // perror("listen");
  // exit(1);
  // }
 
  // //pthread_t captureThreadId;
  // int i = 3;
  // //int captureThread = pthread_create(&captureThreadId, NULL, capture, (void*)&i);

  // std::thread captureThread(capture, std::ref(i));

  // // it is important to detach the thread to avoid memory leak
  // //pthread_detach(captureThreadId);
  // captureThread.detach();

  // while(true) {

  // // block until get a request
  // int ns = accept(s, (struct sockaddr *) &name, &len);

  // if ( ns < 0 ) {
  //   perror("accept");
  //   exit(1);
  // }

//   // each client connection is handled by a seperate thread since
//   // a single client can hold a connection open indefinitely making multiple
//   // data requests prior to closing the connection
//   //pthread_t threadId;
//   //int thread = pthread_create(&threadId, NULL, handleClient, (void*) &ns);
//   std::thread childThread(handleClient, std::ref(ns));

//   // it is important to detach the thread to avoid a memory leak
//   //pthread_detach(threadId);
//   childThread.detach();
//   } 
  
//   close(s);
//   exit(0);
}

// void capture(int& i) {  
//   Camera camera;
//   HSV hsvValues;

//   // Define capture device (camera) and cheak it's connection
//   cv::VideoCapture capture(camera.device);
//   if(!capture.isOpened()) {
//   std::cout << "Failed to connect to the camera.\n";
//   }

//   // Comparison shape for power port vision target
//   std::vector<Target> targets;
//   Target powerPort;
//   powerPort.id = 0;
//   powerPort.name  = "PowerPort";
//   powerPort.size.width  = 39.25;
//   powerPort.size.height = 17.00;
//   powerPort.pose.x     = 89.25;
//   powerPort.shape.push_back(cv::Point2d(00.000, 17.0));
//   powerPort.shape.push_back(cv::Point2d(09.805, 00.0));
//   powerPort.shape.push_back(cv::Point2d(29.445, 00.0));
//   powerPort.shape.push_back(cv::Point2d(39.25 , 17.0));
//   powerPort.shape.push_back(cv::Point2d(36.941, 17.0));
//   powerPort.shape.push_back(cv::Point2d(28.290, 02.0));
//   powerPort.shape.push_back(cv::Point2d(10.960, 02.0));
//   powerPort.shape.push_back(cv::Point2d(02.309, 17.0));
//   powerPort.shape.push_back(cv::Point2d(00.000, 17.0));
//   targets[powerPort.id] = powerPort;

//   Target loadingBay;
//   loadingBay.id - 1;
//   loadingBay.name  = "loadingBay";
//   loadingBay.size.width  = 07.00;
//   loadingBay.size.height = 11.00;
//   loadingBay.pose.x     = 11.00;
//   loadingBay.shape.push_back(cv::Point2d(00.0 ,00.0));
//   loadingBay.shape.push_back(cv::Point2d(00.0 ,11.0));
//   loadingBay.shape.push_back(cv::Point2d(07.0 ,11.0));
//   loadingBay.shape.push_back(cv::Point2d(07.0 ,00.0));
//   loadingBay.shape.push_back(cv::Point2d(00.0 ,00.0));
//   targets[loadingBay.id] = loadingBay;
  
//   // Uncomment for debugging
//   // Defines windows for debugging
//   //  String rawWindow = "Raw";
//   //  String threshWindow = "Thresh";
//   //  namedWindow(rawWindow, WINDOW_NORMAL);
//   //  namedWindow(threshWindow, WINDOW_NORMAL);

//   while(true) {
//   cv::Mat frame, corrected, hsv, thresh;

//   // Get the current frame from the camer	and cheak for data
//   capture >> frame;
//   if(frame.empty()) {
//     std::cout << "failed to capture an image\n" ;
//   }


//   cv::undistort(frame, corrected, camera.matrix, camera.dist);
   
//   // Convert into HSV color space for better thresholding
//   cv::cvtColor(corrected, hsv, cv::COLOR_BGR2HSV);
//   // Threshold the frame into black and white image
//   // All pixles within range are pure white
//   // All pixles out of the threshold are pure black
//   cv::inRange(hsv, Thresh::lowScalar(), Thresh::highScalar(), thresh);

//   // Find find the contours 
//   // Only finds external contours
//   std::vector<std::vector<cv::Point>> contours;
//   cv::findContours(thresh, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
  
//   std::vector<Match> matches;

//   // Iterate over all he vectors to find which ones are which targets
//   for (auto contour = contours.begin(); contour != contours.end(); contour++) {
//     // Gets a value for how well the given contour matches each target
//     for (auto target = targets.begin(); target != targets.end(); target++) {
//     Match match;
//     match.id = target->id;
//     match.current = cv::matchShapes(target->shape, *contour, cv::CONTOURS_MATCH_I2, 0);
//     }

//      // best out of all the current matches
//     auto bestCurrentMatch = std::min_element(std::begin(matches), std::end(matches), [](Match a, Match b) -> bool {
//       return a.current < b.current; 
//     });

//     for (auto target = targets.begin(); target != targets.end(); target++) {
//     Match match = matches[target->id];
//     // Cheaks if the given target is the best match for the contour
//     // and if if it the best match to that target so far
//     // also makes sure none are zero since they default to that value
//     if (match.id == bestCurrentMatch->id && match.current < match.best) {
//       match.best = match.current;
//       target->bestMatch = *contour;
//     }
//     }
//   }

//   // Creat rotated bounding box
//   for (auto target = targets.begin(); target != targets.end(); target++) {
//     target->data.valid = !target->bestMatch.empty();
//     cv::RotatedRect box = cv::minAreaRect(target->bestMatch);
//     double centerX = box.center.x;
//     double centerX = box.center.y;
//     target->data.skew = box.angle;
//     target->data.xAngle = atan((box.center.x - camera.matrix.at<double>(0,2) / camera.matrix.at<double>(0,0)) * (180/M_PI) + camera.orient.yaw);
//     target->data.yAngle = atan((box.center.y - camera.matrix.at<double>(1,2) / camera.matrix.at<double>(1,1)) * (180/M_PI) + camera.orient.pitch);
//     target->data.xPos = target->size.height / box.size.height * camera.matrix.at<double>(1,1);
//     target->data.yPos = target->data.xPos * tan(target->data.xAngle);
//     target->data.zPos = target->data.xPos * tan(target->data.yAngle);
//   }

//   // Find the angle from the camera to the center of the image
//   // Uses pinhole camera modle
//   // 
//   // Cpp standard library uses radients not degrees
//   //   degrees --> radients: * (pi/180)
//   //   radients --> degrees: * (180/pi)
//   //
//   // w = camera frame width (in pixles)
//   // c = center of the target
//   // f = focal length (in pixles) 
//   // θ = angle of target from camera center
//   //
//   // tan(θ°) = (c-(w/2))/f
//   // 
//   // θ° = atan((c-(w/2))/f)
//   //
//   //  pinhole     / 
//   //    |    / 
//   // sensor |   /  
//   //  |   |  /   
//   //  |\  |   /  
//   //  | \   |  /   
//   // c*  \  | /    
//   //  |\_ \ |/     
//   // w|__\_\/_________
//   //  | f  /\\_ θ°
//   //  |   /  \ \_
//   //  |  /  \  \_
//   //  | /    \   \_
//   //  |/    \  \_
//   //       \   \
//   //        \   *
//   //         \
//   //        \
//   //

//   //Using Y angle find the distance from the target
//   //
//   // Cpp standard library uses radients not degrees
//   //   degrees --> radients: * (pi/180)
//   //   radients --> degrees: * (180/pi)
//   //
//   // h = height of the camera
//   // t = target height
//   // θ = angle to target
//   // d = distance to target
//   //
//   // tan(θ°) = (t-h)/d
//   //
//   // d = (t-h)/tan(θ°)
//   //
//   //        *target
//   //         /|
//   //        / |
//   //       /  |
//   //      /   |
//   //       /  |
//   //      /   |t
//   //     / θ°   |
//   // Camera /_ _ _ _|
//   //    |   d   |
//   //    h |     |
//   //    |_______| Ground
//   //

//   // obtain the lock and copy the data
//   //pthread_mutex_lock(&dataLock);
//   dataMutex.lock();

//   powerPortRelativeBearing = powerPortXAngle;
//   powerPortGlobalYAngle = powerPortYAngle;
//   powerPortGlobalDistance = powerPortDistance;
//   loadingBayRelativeBearing = loadingBayXAngle;
//   loadingBayGlobalYAngle = loadingBayYAngle;
//   loadingBayGlobalDistance = loadingBayDistance;

//   //pthread_mutex_unlock(&dataLock);
//   dataMutex.unlock();
//   }
// }



// void handleClient(int& ns) {
//   // printf("Thread starting\n");
//   //int ns = *((int*) arg);
//   char sendbuffer[1024];
//   char command[128];

//   // start conversation with client
//   while(true) {

//   receiveNextCommand(command, ns);

//   if ( strcmp(command, "STOP") == 0 ) {
//     //printf("Received STOP command\n");
//     break;
//   } else if ( strcmp(command, "DATA") == 0 ) {
//     //printf("Received DATA command\n");

//     // obtain the lock and copy the data
//     //pthread_mutex_lock(&dataLock);
//     dataMutex.lock();

//     double copyPowerPortRelativeBearing = powerPortRelativeBearing;
//     double copyPowerPortGlobalYAngle = powerPortGlobalYAngle;
//     double copyPowerPortGlobalDistance = powerPortGlobalDistance;
//     double copyLoadingBayRelativeBearing = loadingBayRelativeBearing;
//     double copyLoadingBayGlobalYAngle = loadingBayGlobalYAngle;
//     double copyLoadingBayGlobalDistance = loadingBayGlobalDistance;

//     //pthread_mutex_unlock(&dataLock);
//     dataMutex.unlock();

//     // the protocol will send an empty line when the data transfer is complete
//     int sendbufferLen = -1;
//     if (isnan(copyPowerPortRelativeBearing) && isnan(copyLoadingBayRelativeBearing)) {
//     sendbufferLen = sprintf(sendbuffer, "\n");
//     } else if (isnan(copyPowerPortRelativeBearing)){
//     sendbufferLen = sprintf(sendbuffer, "lbrb=%.1f\nlbya=%.1f\nlbd=%.1f\n\n", copyLoadingBayRelativeBearing, copyLoadingBayGlobalYAngle, copyLoadingBayGlobalDistance);
//     } else if (isnan(copyLoadingBayRelativeBearing)){
//     sendbufferLen = sprintf(sendbuffer, "pprb=%.1f\nppya=%.1f\nppd=%.1f\n\n", copyPowerPortRelativeBearing, copyPowerPortGlobalYAngle, copyPowerPortGlobalDistance);
//     } else{
//     sendbufferLen = sprintf(sendbuffer, "pprb=%.1f\nppya=%.1f\nppd=%.1f\nlbrb=%.1f\nlbya=%.1f\nlbd=%.1f\n\n", copyPowerPortRelativeBearing, copyPowerPortGlobalYAngle, copyPowerPortGlobalDistance, copyLoadingBayRelativeBearing, copyLoadingBayGlobalYAngle, copyLoadingBayGlobalDistance);
//     }
//     // write response to client
//     write(ns, sendbuffer, sendbufferLen);
//   } else {
//     //printf("Received unknown command '%s'\n", command);
//     break;
//   }

//   }
//   close(ns);
//   //printf("Thread ending\n");
// }

// void receiveNextCommand(char *command, int ns) {
//   int receiveLength = read(ns, command, 1024);
//   int commandLength = 0;
//   while(commandLength < receiveLength) {
//   char value = command[commandLength];
//   if ( value == 0x0d || value == 0x0a ) {
//     break;
//   } 
//   commandLength++;
//   }

//   // add the terminating 0 to mark the end of the string value in the char *
//   command[commandLength] = 0;
// }
