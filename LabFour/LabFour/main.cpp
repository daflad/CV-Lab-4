//
//  main.cpp
//  LabFour
//
//  Created by Stephen John Russell & Peter Butcher on 27/01/2014.
//  Copyright (c) 2014 Stephen John Russell & Peter Butcher. All rights reserved.
//

#include "VideoProcessor.hpp"

void MouseCallBackFunc(int event, int x, int y, int flags, void* userdata) {
    
    VideoProcessor* vp = static_cast<VideoProcessor*>( userdata );
    
    if (event == CV_EVENT_LBUTTONUP) {
        vp->mouseX = x;
        vp->mouseY = y;
        vp->mouseClicked = true;
        // Debug
//        printf("Left mouse click at :: x = %d\t y = %d\n",x,y);
    }
}

//-----------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------
int main(int argc, const char * argv[]) {
    // Default fp will load avaiable cameras
    string fp = "none";
    
    // Check if any arguments have been passed and use first for video loading
    if (argc >= 2) {
        fp = argv[1];
    }
    
    // Construct & check for errors
    VideoProcessor vp(fp);
    if (vp.ERROR) {
        cerr << vp.err << endl;
        return -1;
    }
    
    int *h = &vp.cannyHigh;
    int *l = &vp.cannyLow;
    int *t = &vp.threshold;
//    int *q = &vp.colourQuant;
    // Window & trackbars
    namedWindow("blended", CV_WINDOW_AUTOSIZE);
    createTrackbar("Canny High", "blended", h, 205);
    createTrackbar("Canny Low", "blended", l, 205);
    createTrackbar("Detection Threshold", "blended", t, 100);
    setMouseCallback("blended", MouseCallBackFunc, &vp);
    
    // Play video applying
    vp.processVideo();
    return 0;
}