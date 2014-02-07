//
//  VideoProcessor.h
//  LabFour
//
//  Created by Stephen John Russell & Peter Butcher on 28/01/2014.
//  Copyright (c) 2014 Stephen John Russell & Peter Butcher. All rights reserved.
//

#ifndef __LabFour__VideoProcessor__
#define __LabFour__VideoProcessor__

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;


class Descriptor {
    
public:
    Descriptor(float[15][15],int);
    float roiHist[15][15];
    int index;
};

class VideoProcessor {
    
    public :
    
    Mat edges;
    Mat blend;
    Mat frame;
    Mat src;
    Mat roi;
    VideoCapture camera;
    
    int cannyLow;
    int cannyHigh;
    int frameNumber;
    int colourQuant;
    int threshold;
    int closestIndex;
    
    string vidPath;

    int mouseX;
    int mouseY;
    bool mouseClicked;
    
    bool desctiptorFound;
    bool paused;

    bool ERROR;
    string err;
    
    vector<Vec3f> circles;
    vector<Descriptor> des;
    float roiHist[15][15];
    
    VideoProcessor(string filePath);
    
    bool    checkInputArgs(int argc, const char * argv[]);
    void    processVideo();
    void    processImage();
    float   compareHists(float hist[15][15]);
    void    createDescriptor(bool, int, Mat*);
    void    setupControlWindow(int);
    void    MouseCallBackFunc(int event, int x, int y, int flags, void* userdata);
    
};


#endif /* defined(__LabFour__VideoProcessor__) */