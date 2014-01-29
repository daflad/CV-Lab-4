//
//  VideoProcessor.h
//  LabFour
//
//  Created by Stephen John Russell on 28/01/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#ifndef __LabFour__VideoProcessor__
#define __LabFour__VideoProcessor__

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

class VideoProcessor {
    
    public :
    
    Mat edges;
    Mat blend;
    Mat frame;
    Mat src;
    VideoCapture camera;
    
    int cannyLow;
    int cannyHigh;
    int frameNumber;
    
    string vidPath;
    
    bool paused;
    bool ERROR;
    string err;
    
    vector<Vec3f> circles;
    
    VideoProcessor(string filePath);
    
    bool checkInputArgs(int argc, const char * argv[]);
    void processVideo();
    void processImage();
    void setupControlWindow(int);
    
};

#endif /* defined(__LabFour__VideoProcessor__) */