//
//  VideoProcessor.cpp
//  LabFour
//
//  Created by Stephen John Russell & Peter Butcher on 28/01/2014.
//  Copyright (c) 2014 Stephen John Russell & Peter Butcher. All rights reserved.
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
    colourQuant = 15;
    threshold = 50;
    paused = false;
    ERROR = false;
    desctiptorFound = false;
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

// Create Desctiptor
// =================
//
// Conver to chromatic colour space:
// Cr = R / (R + G + B + 1) & Cb = B / (R + G + B + 1)
// Build histogram of Cr & Cb & divide by total pixels
//
void VideoProcessor::createDescriptor() {
    // No need to worry about 3 channels here as condensing into 1 value
    int numPix = roi.rows * roi.cols;
    float Cr[numPix];
    float Cb[numPix];
    int index = 0;
    for (int i = 0; i < roi.rows; i++) {
        for (int j = 0; j < roi.cols; j++) {
            Vec3b pix = roi.at<Vec3b>(i,j);
            Cr[index] = colourQuant * (float)pix[2] / (pix[0] + pix[1] + pix[2] + 1);
            Cb[index++] = colourQuant * (float)pix[0] / (pix[0] + pix[1] + pix[2] + 1);
            printf("Cr : %f, Cb : %f\n", Cr[index- 1], Cb[index -1]);
        }
    }
    int roiHist[15][15] = {0};
    for (int i = 0; i < numPix; i++) {
        int Cri = (int)Cr[i];
        int Cbi = (int)Cb[i];
        if ( Cr[i] - (int)Cr[i] > 0.5) {
            Cri++;
        }
        if (Cb[i] - (int)Cb[i] > 0.5) {
            Cbi++;
        }
        roiHist[Cri][Cbi]++;
    }
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            printf("x : %d \t y : %d\tv : %d\n",i,j,roiHist[i][j]);
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
    
    // Begin image processing

    cvtColor(frame, edges, CV_BGR2GRAY);

    // Blur & copy for both Canny & HoughCircles
    GaussianBlur(edges, edges, Size(9, 9), 2, 2);
    edges.copyTo(src);
    
    Canny(edges, edges, cannyLow, cannyHigh, 3);
    HoughCircles( src, circles, CV_HOUGH_GRADIENT, 1, src.rows/8, cannyHigh, threshold, 0, 0 );
    
    // merge images & display
    frame.copyTo(blend);
    blend.setTo(Scalar(0,255,0), edges);
    
    if (!mouseClicked) {
        // Draw the circles detected
        for( size_t i = 0; i < circles.size(); i++ ) {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);
            // circle center
            circle( blend, center, 3, Scalar(255,100,100), -1, 8, 0 );
            // circle outline
            circle( blend, center, radius, Scalar(0,0,255), 3, 8, 0 );
        }
    } else {
        // check if mouse clicked within one of the circles
        for( size_t i = 0; i < circles.size(); i++ ) {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);
            // Calculate delta x & y ^2
            float x2 = (center.x - mouseX) * (center.x - mouseX);
            float y2 = (center.y - mouseY) * (center.y - mouseY);
            // Calculate euclidean distance
            float dist = sqrt(x2 + y2);
            // Have we found a circle?
            if (dist <= radius + 3) {
                printf("Found circle at x : %d, y : %d\n", center.x, center.y);
                roi = frame(Rect(Point(center.x - radius, center.y - radius), Size(2*radius + 3, 2*radius + 3)));
                // Show selected region
                namedWindow("roi", CV_WINDOW_AUTOSIZE);
                moveWindow("roi", frame.cols + 20, 0);
                imshow("roi", roi);
                createDescriptor();
                break;
            }
        }
        // reset mouse click
        mouseClicked  = false;
    }
    // update display
    imshow("blended", blend );
    
}
