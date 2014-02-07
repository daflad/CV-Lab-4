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
    cannyHigh =  10;
    frameNumber = 1;
    colourQuant = 14;
    threshold = 10;
    closestIndex = 0;
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
    HoughCircles( src, circles, CV_HOUGH_GRADIENT, 1, src.rows/8, cannyHigh, threshold + 50, 0, frame.rows / 3 );
    
    // merge images & display
    frame.copyTo(blend);
    blend.setTo(Scalar(0,255,0), edges);
    
    if (!mouseClicked) {
        
        // Draw the circles detected
        int index = 0;
        for( size_t i = 0; i < circles.size(); i++ ) {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);
            if ((center.x - radius > 0) && (center.y - radius > 0)) {
                if ((center.x + radius < frame.rows) && (center.y + radius < frame.cols)) {
                    Mat m = frame(Rect(Point(center.x - radius, center.y - radius), Size(2*radius, 2*radius)));
                    createDescriptor(false, index++, &m);
                    if (!desctiptorFound) {
                        // circle center
                        circle( blend, center, 3, Scalar(255,100,100), -1, 8, 0 );
                        // circle outline
                        circle( blend, center, radius, Scalar(0,0,255), 3, 8, 0 );
                    }
                }
            }
        }
        if (desctiptorFound) {
            float maxD = 0;
            for (int i = 0; i < des.size(); i++) {
                float d = compareHists(des[i].roiHist);
                if (maxD < d) {
                    maxD = d;
                    closestIndex = i;
                }
            }
            des.clear();
            printf("Chosen Index :: %d\n", closestIndex);
            Point center(cvRound(circles[closestIndex][0]),
                         cvRound(circles[closestIndex][1]));
            int radius = cvRound(circles[closestIndex][2]);
            // circle center
            circle( blend, center, 3, Scalar(200,0,200), -1, 8, 0 );
            // circle outline
            circle( blend, center, radius, Scalar(200,0,255), 3, 8, 0 );
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
                // Debug
//                printf("Found circle at x : %d, y : %d\n", center.x, center.y);
                roi = frame(Rect(Point(center.x - radius, center.y - radius), Size(2*radius + 3, 2*radius + 3)));
                // Show selected region
                namedWindow("roi", CV_WINDOW_AUTOSIZE);
                moveWindow("roi", frame.cols + 20, 0);
                imshow("roi", roi);
                createDescriptor(true, 0, &roi);
                desctiptorFound = true;
                break;
            }
        }
        // reset mouse click
        mouseClicked  = false;
    }
    // update display
    imshow("blended", blend );
    
}


// Create Desctiptor
// =================
//
// Conver to chromatic colour space:
// Cr = R / (R + G + B + 1) & Cb = B / (R + G + B + 1)
// Build histogram of Cr & Cb & divide by total pixels
//
void VideoProcessor::createDescriptor(bool chosenCircle, int index, Mat* roi) {
    // No need to worry about 3 channels here as condensing into 1 value
     
    int numPix = roi->rows * roi->cols;
    float Cr[numPix];
    float Cg[numPix];
    // Keep track of index for new array
    int ind = 0;
    for (int i = 0; i < roi->rows; i++) {
        for (int j = 0; j < roi->cols; j++) {
            // Fetch all values
            Vec3b pix = roi->at<Vec3b>(i,j);
            // Calculate conversion and quantize
            Cr[ind] = colourQuant * (float)pix[2] / (pix[0] + pix[1] + pix[2] + 1);
            Cg[ind++] = colourQuant * (float)pix[1] / (pix[0] + pix[1] + pix[2] + 1);
            // Debug
            //printf("Cr : %f, Cb : %f\n", Cr[index- 1], Cb[index -1]);
        }
    }
    // Histogram array
    float hist[15][15];
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            hist[i][j] = 0;
        }
    }
    for (int i = 0; i < numPix; i++) {
        // round values up or down.
        int Cri = (int)Cr[i];
        int Cgi = (int)Cg[i];
        if ( Cr[i] - Cr[i] > 0.5) {
            Cri++;
        }
        if (Cg[i] - Cg[i] > 0.5) {
            Cgi++;
        }
        if (Cgi < 0) {
            Cgi = 0;
        }
        if (Cri < 0) {
            Cri = 0;
        }
        // increment index
        if (chosenCircle) {
            roiHist[Cri][Cgi]++;
        } else {
            hist[Cri][Cgi]++;
        }
    }
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (chosenCircle) {
                roiHist[i][j] /= numPix;
            } else {
                hist[i][j] /= numPix;
            }
        }
    }
    if (!chosenCircle) {
        // Construct descriptor and add to list
        Descriptor d(hist, index);
        des.push_back(d);
    }

    // Debug
//    for (int i = 0; i < 15; i++) {
//        for (int j = 0; j < 15; j++) {
//            printf("x : %d \t y : %d\tv : %d\n",i,j,roiHist[i][j]);
//        }
//    }
}


// Compare Hists
// =============
//
// Compaer the current roi against the passed
float VideoProcessor::compareHists(float hist[15][15]) {
    
    float sum = 0;
    
    // For each possible location in hist take the smaller value and sum
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (hist[i][j] > roiHist[i][j]) {
                sum += roiHist[i][j];
            } else {
                sum += hist[i][j];
            }
        }
    }
    // Debug
//    printf("The sum is :: %f\n", sum);
    return sum;
}


// Descriptor
// ==========
//
// Descriptor class to simplify storing arrays.
Descriptor::Descriptor(float h[15][15], int i) {
    // Copy the array to the descriptor array.
    for (int j = 0; j < 15; j++) {
        for (int k = 0; k < 15; k++) {
            roiHist[j][k] = h[j][k];
        }
    }
    index = i;
}
