//
//  VideoProcessor.cpp
//  LabFour
//
//  Created by Stephen John Russell on 28/01/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#include "VideoProcessor.hpp"

// Constructor
//
// Init variables
// Open video / camera stream
VideoProcessor::VideoProcessor(string filePath){
    
    // Setup parameters
    cannyLow = 45;
    cannyHigh =  50;
    frameNumber = 1;
    
    paused = false;
    ERROR = false;
    vidPath = filePath;
    
    // Attempt to load video
    camera = VideoCapture(vidPath);
    // Check video
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

// Video processing
// ================
//
// Init window
// Loop through video
// Listen for exit
void VideoProcessor::processVideo() {
    
    // Window & trackbars
    namedWindow("blended", CV_WINDOW_AUTOSIZE);
    createTrackbar("Canny High", "blended", &cannyHigh, 255);
    createTrackbar("Canny Low", "blended", &cannyLow, 255);
    
    // Until exit key pressed. . .
    while (true) {
        processImage();
        // Debug data
        // printf("Canny High \t\t::%d \nCanny Low \t\t::%d \nFrame Number \t::%d\n",cannyHigh, cannyLow, frameNumber);
        // Listen for key press
        int k = waitKey(30);
        
        if(k == 32) {
            // Space bar pauses % unpauses video
            paused = !paused;
        } else if (k != -1) {
            // Any other key exits program
            break;
        }
    }
}

// Image processing
// ================
//
// Copy current frame
// Check for empty frame
// Convert frame to gray scale
// Blur frame & perform canny edge detection
// Blur frame & perform HoughCircle detection
void VideoProcessor::processImage() {

    // Copy data from video / camera to frame
    if (!paused) {
        camera >> frame;
    }
    
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
    
    // Begin image processing

    cvtColor(frame, edges, CV_BGR2GRAY);

    GaussianBlur(edges, src, Size(9, 9), 2, 2);
    GaussianBlur(edges, edges, Size(9, 9), 2, 2);
    
    
    Canny(edges, edges, cannyLow, cannyHigh, 3);
    
    HoughCircles( src, circles, CV_HOUGH_GRADIENT, 1, src.rows/8, cannyHigh, cannyLow, 0, 0 );
    
    // merge images & display
    frame.copyTo(blend);
    blend.setTo(Scalar(0,255,0), edges);
    
    // Draw the circles detected
    for( size_t i = 0; i < circles.size(); i++ ) {

        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        // circle center
        circle( blend, center, 3, Scalar(255,100,100), -1, 8, 0 );
        // circle outline
        circle( blend, center, radius, Scalar(0,0,255), 3, 8, 0 );
    }

    imshow("blended", blend );
    
}