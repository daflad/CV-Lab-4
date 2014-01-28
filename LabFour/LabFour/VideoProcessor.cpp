//
//  VideoProcessor.cpp
//  LabFour
//
//  Created by Stephen John Russell on 28/01/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#include "VideoProcessor.hpp"

VideoProcessor::VideoProcessor(string filePath){
    
    // Setup parameters
    cannyLow = 0;
    cannyHigh = 255;
    frameNumber = 1;
    ERROR = false;
    vidPath = filePath;
    
    // Attempt to load video
    camera = VideoCapture(vidPath);
    
    if (!camera.isOpened()) {
        // Attempt to load camera if no video
        camera = VideoCapture(0);
        if (!camera.isOpened()) {
            // Still no feed!!
            err = "Video & Camera failed to initialize";
            ERROR = true;
        }
    }
}

void VideoProcessor::processVideo() {
    
    // Window & trackbars
    namedWindow("blended", CV_WINDOW_AUTOSIZE);
    createTrackbar("Canny High", "blended", &cannyHigh, 255);
    createTrackbar("Canny Low", "blended", &cannyLow, 255);
    
    // Until exit key pressed. . .
    while (true) {
        // Process each frame as it comes in
        processImage();
        // Debug data
        printf("Canny High \t\t::%d \nCanny Low \t\t::%d \nFrame Number \t::%d\n",cannyHigh, cannyLow, frameNumber);
        // Listen for exit key
        if(waitKey(30) >= 0) {
            break;
        }
    }
}

void VideoProcessor::processImage() {

    // Copy data from video / camera to frame
    camera >> frame;
    
    // Check for empty frame as this is end of video
    if (frame.empty()) {
        // Reload video & reset frame number
        camera = VideoCapture(vidPath);
        camera >> frame;
        frameNumber = 0;
    } else {
        frameNumber++;
    }
    
    //resize(frame, frame, Size(440,248));
    
    // Image processing
    // ================
    //
    // Convert frame to gray scale
    // Blur frame & perform canny edge detection
    //
    cvtColor(frame, edges, CV_BGR2GRAY);
    GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
    Canny(edges, edges, cannyLow, cannyHigh, 3);
    frame.copyTo(blend);
    blend.setTo(255, edges);
    imshow("blended", blend);

}