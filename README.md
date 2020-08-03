# RambunctionVision 2021
Vision code for FRC team Rambunction 4330 for the 2021 season.
## Requirments
* Cmake: https://cmake.org
* OpenCV: https://opencv.org
  
*Note: these can not curretly be installed on the Laue school computers*
### Instalation
Any supported package manager will work (or you can compile from scource), but here are some simple examples for varius platforms. 
#### Linux (with apt):
```
$ apt install cmake
$ apt install opencv
```

#### MacOS (with homebrew):
https://brew.sh
```
$ brew install cmake
$ brew install opencv
```

#### Windows:
🤷 I have no idea???????

## Build
Naviget to project folder and creat build directory
```
$ cd RambuncionVision2021
$ mkdir build
```
Enter the build directory and compile with cmake.
```
$ cd build
$ cmake ..
$ make
```

The executables can then be found in the /bin folder.
```
$ cd ..
$ cd bin
```
I recomended that you copy the executables back into the projects root folder
```
$ mv setup /../
$ mv vision /../
```
## Setup
Before running vision the camera must be setup to calculate thresholding values and intresic camera properties.  To run the setup you will need a chessboard [or this print out](images/pattern.png) and the target(s) you want to detect. Without retroreflective tape and a led ring I found that a cutout of brightly colord paper taped to a dark background works well.

The `setup` executable takes 3 arguments: a setup config file, a vision config file, and a path to an output file (All files must be in either .xml .yml or .json formatt).
```
$ ./setup setupConfig.xml visionConfig.xml output.xml
``` 
If no argumanets are given it will use `setupConfig.xml`, `visionConfig.xml`, and output to `output.xml`. 

### setupConfig.xml
I recomed that you dont directly edit any of the files and instead use a copy.

The file has two main sections: `<CalibrateConfig>` and `<ThresholdingConfig>` used to (you guessed it) configure the camera calibration and thresholding.

#### CalibrateConfig
* `<BoardWidthg>` and `<BoardHeight>` The number of inner corners of the chess board in each dimension (a standard 8x8 square chessboard has 7x7 inner corners, and the [pattern](images/pattern.png) is 9x7) 
* `<SquareSize>` The side length of each chess board square. *Note: It doesnt matter what units you use as long as they are consistent across the entire Project*
* `<NumberOfImages>` Number of images of the board it will use to calibrate the camera
* `<Automatic>` If the value is not 0 the program will automaticly capture images of the board when all the inner corrners can be seen
* `<Delay>` The delay between each capture if `<Automatic>` is not 0
* `<UsePhotos>` If not 0, the program will use a seriers of photos to calibrate the camera instead of a live video capture
* `<Images>` List of images to use if `<UsePohots>` is not 0

#### ThresholdingConfig
* `<UsePhotos>` If not 0 the program will use a series of photos to tune tghe thresholding values
* `<Image>` List of images to use if `<UsePohots>` is not 0

### visionConfig.xml and Output.xml
I recomed that you dont directly edit any of the files and instead use a copy.

These files have the same structer so that the output can be put bake into the program to be edited without starting from scratch. This means that on the initial setup there are several values that can be left blank because they will be calculated later.
 

It has three main sections: `<Thresholding>`, `<Camera>` and `<Targets>`.

#### Thresholding
It holds the minimum and maximun hsv values for thresholding. It can be left blank to be calculated by the program

#### Camera
This holds the properties of the camera
* `<ID>` the id for the camera (starts at 0 and counts up for each camera that is added to the computer)
* `<Offset>` Physical Offset of the camera from the center of the robot 
* `<AngleOffset>` Physical Offset of the camera angle from forward
* `<CameraMatrix>` intresic camera matrix used to  convert from image points to real world pints (Calculated by program)
* `<Distortion>` Values to account for a camera lense distortion (Calculated by program)
  
#### Targets
List of the targets for vision to detect each entry uses with a blank tag: `<_>` and has two properties:
* `<Name>` Identifier for the target
* `<Shape>` List of 2d points that make up the shape of the target starting at the bottom left and going around counter clockwise. *Note: Use the same units as uesd for the chessboard*

### Running the program
The prgram once run will promped yuo with 6 oprtions:
```
Options:
1. Calibrate Camera
2. Show Undistorted
3. HSV Tunning
4. Detect Targets
5. Save
6. Exit
```

To choose an option enter the number corosponding to thet option into the prompt and hit enter

#### 1. Calibrate Camera

This option should open a window with a camera feed. If the chessboard is in view of the camera the corners will be draw onto the feed and an image will be captured after every period of delay or whenever the spacebar is pressed. If `<UsePhotos>` is treu this will be skiped in favor of using the images.

The program will the autoaticlystart the `Show Undistorted` option.

#### 2. Show Undistorted

Opens a winow showing an undistorted camera feed. 
* Pressing `d` will toggle between an undistorted view and the raw camera feed 
* Pressing `b` will toggle between shouing chessboard points and a 3d bounding box when the chessboard is in view
*  Pressing `a` will toggle between showing a 3d bounding box and just the 3 axis of the board

If `<UsePhotos>` is true you can use `,` and `.` to flip between images

#### 3. HSV 
Opens a window with 6 trackbars corosponding to min and max hsv values to 
* Pressing `t` will toggle between a thresholded view and the raw image

If `<UsePhotos>` is true you can use `,` and `.` to flip between images

#### 4. Detect Targets
Opens a window where the wear targets will be detected
* Pressing `t` togles between a raw and thresholded view
* Pressing `d` will toggle between an undistorted view and the raw camera feed 
* Pression `b` will togggle between a 3d box bounding the target, and an outline of the target
* Pressing `a` will toggle between a bounding box and axis for the targets

If `<UsePhotos>` is true you can use `,` and `.` to flip between images

#### 5.Save 
Saves the calculated values to the output file

#### 6. Exits
Exits the program

## Vision
Runs vision Software (Uncompleted)

