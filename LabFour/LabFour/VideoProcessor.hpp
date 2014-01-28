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
    VideoCapture camera;
    
    bool ERROR;
    string err;
    
    VideoProcessor(string filePath);
    
    bool checkInputArgs(int argc, const char * argv[]);
    
    int processImage();
    void setupControlWindow(int);
    
};


#endif /* defined(__LabFour__VideoProcessor__) */
