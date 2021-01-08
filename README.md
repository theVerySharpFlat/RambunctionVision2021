# RambunctionVision 2021
Vision code for FRC team Rambunction 4330 for the 2021 season.

<image src="images/example_photo.png"
     alt="Example Photo" height="250"/>
     
## Requirments
* OpenCV: https://opencv.org
* CMake: https://cmake.org
## Installation
*Note: this was last updated August 29th, 2020 so newer versions may be availible*

### Ladue Macobooks
*Note: I believ you will need to have the Xcode tools installed.*

1. creat `~/usr` folder. 

this is where all the libraries and executables will be stored
```
$ mkdir ~/usr
```

5. Edit .bash_profile 

This will point the system to the folder with the executables
```
$ nano ~/.bash_profile
```
add the line: 
```
export PATH="$PATH:~/usr/bin" 
```

After this you must create a new terminal instance (close and reopen the window)

2. Download CMake
```
$ curl -L -O https://github.com/Kitware/CMake/releases/download/v3.18.2/cmake-3.18.2.tar.gz
```
uncompress the file and enter the root directory
```
$ tar -xf cmake-3.18.2.tar.gz 
$ cd cmake-3.18.2
```

3. Build and install CMake to the ~/usr directory (this may take some time)

configures cmake to properly install
```
$ ./configure --prefix=$HOME/usr
```
compiles cmake 
```
$ make
```
installs the proper files into the ~/usr directory
```
$ make install
```

4. Clean up after instalation
```
$ cd
$ rm -rf CMake
``` 

6. Download Opencv

uncompress it and enters the root directory
```
$ curl -O -L https://github.com/opencv/opencv/archive/4.4.0.tar.gz
$ tar -xf 4.4.0.tar.gz 
$ cd opencv-4.4.0
```

7. Build and install Opencv

Uses the previusly installed cmake to configure opencv by first creating and entering a build directory to store all the build files
```
$ mkdir build
$ cd build
```
uses cmake to configure opencv for install to the ~/usr directory
```
$ cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=~/usr/local ..
```
compiles and installs opencv
```
$ make
$ make install
```

8. Clean up after instalation
```
$ cd
$ rm -rf opencv
```

### Linux (with apt):
```
$ apt install cmake
$ apt install libopencv-dev
```

### MacOS (with homebrew):
https://brew.sh

*None: homebrew dose not currently work on the school computers, I have asked the tech department, but since they are preocupied holding together all of school, they have not responded yet.*
```
$ brew install cmake
$ brew install opencv
```

### Windows:
[this link](https://www.opencv-srf.com/2017/11/install-opencv-with-visual-studio.html) should be an explination of how to install for windows, but I have never tried it, so I dont know if it works. 

*Note: The rest of the instructins are for MacOS and Linxs/Unix and not Windows (exept if your running a linux subsystem, but I dont really know anything about that)*

## Build
Naviget to project folder and creat build directory
```
$ cd RambuncionVision2021
$ mkdir build
$ cd build
```
Enter the build directory and compile with cmake. and then move back into the root folder
```
$ cmake ..
$ make
$ cd ..
```

The executables can then be found in the /bin folder.

I recomended that you copy the executables back into the projects root folder
```
$ mv bin/setup ./
$ mv bin/vision ./
```
## Setup
Before running vision the camera must be setup to calculate thresholding values and intresic camera properties.  To run the setup you will need a chessboard [or this print out](images/pattern.png) and the target(s) you want to detect. Without retroreflective tape and a led ring I found that a cutout of brightly colord paper taped to a dark background works well enough.

the setup program will take 2 agrumnets. an input file and an output file. if only and input is provided, the program will output to that same file.
```
$ ./setup input.xml output.xml
```

The files should be structered with two section. a list of the cameras, and a list of globla targets all the cameras should look for. *Note: each camera object also holds a list of targets specific to that camera and non of the others.*

### Cameras
List of camera that willbe used and their properties. Each entry into the `<Cameras>` section must start and end with blank tags: `<_>` with the data for a given camera inside the tags.

* `<ID>` - Camera id used by the computer (0 is the built in camera, and the number counts up for every scuccesive camera).
* `<Offset>` - Distance offset from the origin (center of the robot). In the order x y z with 1 spaces between each number.
* `<AngleOffset>` - Angle offset from foward (as compared to the root). In the order x andle y angle z angle with 1 spaces between each number.
* `<CameraMatrix>` - Intrensic camera matrix that will be calculated by the program. Don't mauely modify.
* `<Distortion>` - Coeffecients to account for lense distortion. Don't manuely modify.
*  `<Thresholds>` - Max and min HSV thresholding values for finding the target.
* `<Targets>` - List of targets for the individual camera to detect. See below for detail.
* `<CalibrationConfig>` - Details on how to calibrate the camera.
    * `<BoardWidth>` and `<BoardHeight>` - Dimensions of inner corrners of the chessboard (the print out is 9x6, and a standard 8x8 chess board will have 7x7 inner corrners).
    * `<SquareSize>` - Size of the chess board squares
    * `<NumberOfImages>` - Number of images used to calibrate the camera. I recomend around 30.
    * `<Automatic>` - If not `0` it will automaticaly capture images when the board is in frame.
    * `<Delay>` - Time (ms) to wait between image captures if automatic.
    * `<UsePhotos>` If not `0` the program will use the list of images below to calibrate instead of live capture.
    * `<Images>` - Images to use to calibrate if UsePhots is true.
* `<ThresholdingConfig>` details about how to configure the thresholding values.
    * `<UsePhotos>` - If not `0` the program will use a list of images instead of live capture to calibrate the values.
    * `<Images>` - The list of images to use if `<UsePhotos>` is true.

### Global Targets
A list of targets for all the camera to search for. Each entry i `<GlobalTargets>` must be enclosed in a enpty tag `<_>` with the data for that targets inside.
* `<Name>` - The unique name to identify the target
* `<Shape>` - The list of 2d coordinates of target. Each number must be seperated by a single space.
### Running the program
The prgram once run will promped you with 7 oprtions:
```
Options:
1. Choose camera
2. Calibrate Camera
3. Show Undistorted
4. HSV Tunning
5. Detect Targets
6. Save
7. Exit
```

To choose an option enter the number corosponding to thet option into the prompt and hit enter

#### 1. Calibrate Camera

This option should open a window with a camera feed. If the chessboard is in view of the camera the corners will be draw onto the feed and an image will be captured after every period of delay or whenever the spacebar is pressed. If `<UsePhotos>` is true this will be skiped in favor of using the images.

The program will the autoaticlystart the `Show Undistorted` option.

#### 2. Show Undistorted

Opens a winow showing an undistorted camera feed. 

Button Presses
* `d` - Toggles between an undistorted view and the raw camera feed 
* `b` Toggles between shouing chessboard points and a 3d bounding box when the chessboard is in view
*  `a` Toggles between showing a 3d bounding box and just the 3 axis of the board

If `<UsePhotos>` is true you can use `,` and `.` to flip between images

#### 3. HSV 
Opens a window with 6 trackbars corosponding to min and max hsv values to adjust threholding.

Button Presses
* `t` - Toggles between a raw view and a thresholded view
* `d` - Toggles between an undistorted view and the raw camera feed 
* `b` Toggles between shouing chessboard points and a 3d bounding box when the chessboard is in view
*  `a` Toggles between showing a 3d bounding box and just the 3 axis of the boarde

If `<UsePhotos>` is true you can use `,` and `.` to flip between images

#### 4. Detect Targets
Opens a window where the wear targets will be detected

Button presses
* `t` - Toggles between a raw view and a thresholded view
* `d` - Toggles between an undistorted view and the raw camera feed 
* `b` Toggles between showing the outline of the target and a 3d bounding box when the target is in view
*  `a` Toggles between showing a 3d bounding box and just the 3 axis of the boarde

If `<UsePhotos>` is true you can use `,` and `.` to flip between images

#### 5.Save 
Saves the calculated values to the output file

#### 6. Exits
Exits the program.

## Vision
Using the data gathered in the steup, will run a detectionwith the given cameras and targets along with a TCP server which can be requested for data.

The program takes 1 input file from the setup pogram. You can use an optional `-p=xxxx` or `--port=xxxx` flag tospecify which port the server should run on. It will default to 4330 if the flag is not used. 
```
$ ./vision config.xml --port=4330
```

There is also a simple client program called `simpleClient` that can be run to test the program. It takes the an address and port as it's arguments, and can be used to manuely send request to the main vision program.

### Requests
The request follow the format of:
```
action:subject:key=value
```

### Actions
* `get` - request to get the value of a key
* `set` - request to change the value of a key
* `data` - request returned from the program with the asked for data
* `error` - an error message stating that sothing went wrong

### Subjects
The subjects are either a given target name followed by the id of the camera that detected it, or the word `camera` followed by the camera id. `[target][id]` or `camera[id]` (ex. `powerPort0` or `camera1`).

### Keys
* Camera Keys
    * `id` - the id of the camera
    * `fps` - the average capture rate of the camera including processing time of the last 4 captures *(not yet implemented)*
    * `rawFPS` - raw frame rate of the camera not inculding processing time
    * `size` - size of the images the camera produces
    * `offset` - offset of the camera from the origin
    * `angleOffset` - angle offset of the camera from forward
    * `thresholdsH` - Hue thresholds the camera is using
    * `thresholdsS` - Stauration thresholds the camera is using
    * `thresholdsV` - Value thresholds the camera is using
    * `fx` - x focal length of the camera
    * `fy` y focal length of the camera
    * `targets` - list of targets a given camera is searching for
    * `snapshot` - setable value that if not `0` will save a snapshot from the camera to the `images/snapshots` folder *(not yet implemnted)*
    * `startRecording` and `stopRecording` - starts and stops a recording of the camera capture and saves it to the `images/recordings` folder. *(not yet implemented)*
* Target keys
    * `name` - name of the target
    * `latency` - delay scince the targetwas captured on the server side *(not yet implemented)*
    * `matchError` - match error of the detection
    * `rawPose` - position of the target in the image space
    * `rawArea` - area of the camera in the image space in pixles
    * `targetPoints` - point that the the detected target was matched against
    * `boxSize` - size of the rotated bounding box in the image space
    * `boxArea` - area of the rotated bounding boxin the image spce
    * `boxSkew` - skew of the rotated bounding box in the image space 
    * `xAngle` - x angle to the target in 3d from the origin
    * `yAngle` - y angle to the target in 3d from the origin
    * `roughSize` - size of the horizontal bounding box in the image space
    * `roughArea` - area of the horizontal bonding box in the image space
    * `3dPose` - 3d poisiton og the target
    * `3dAngle` - 3d orientation of the target

### Value
Will be either a numberic value, a string, or a list of values enclosed in braces and seperated by commans.






