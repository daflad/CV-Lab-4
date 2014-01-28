//
//  VideoProcessor.cpp
//  LabFour
//
//  Created by Stephen John Russell on 28/01/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#include "VideoProcessor.hpp"

VideoProcessor::VideoProcessor(string filePath){
    
    camera = VideoCapture(filePath);
}

int VideoProcessor::processImage(){
    if (!camera.isOpened()) {
        cerr << "Video file failed to initialize";
        camera = VideoCapture(0);
        if (!camera.isOpened()) {
            cerr << "Camera failed to initialize";
            return -1;
        }
    }
    namedWindow("blended");
    while (true) {
        Mat frame;
        camera >> frame; //Get frame
        resize(frame, frame, Size(440,248));
        cvtColor(frame, edges, CV_BGR2GRAY);
        GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
        Canny(edges, edges, 30, 70, 3);
        frame.copyTo(blend);
        //addWeighted(original, 0.9, edges, 0.1, 0.0,  blend);
        //add(frame, edges, blend);
        blend.setTo(255, edges);
        imshow("blended", blend);
        if(waitKey(30) >= 0) {
            break;
        }
    }
    return 0;
}