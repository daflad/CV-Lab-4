//
//  main.cpp
//  LabFour
//
//  Created by Stephen John Russell on 27/01/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
    Mat edges, blend, original;
    VideoCapture camera(argv[1]); //Open default camera
    if (!camera.isOpened()) {
        cerr << "Camera failed to initialize";
        return -1;
    }
    
    namedWindow("original");
    namedWindow("blended");
    namedWindow("edges");
    while (true) {
        Mat frame;
        camera >> frame; //Get frame
        frame.resize(248,440);
        cvtColor(frame, edges, CV_BGR2GRAY);
        GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
        Canny(edges, edges, 30, 70, 3);
        frame.copyTo(blend);
        //addWeighted(original, 0.9, edges, 0.1, 0.0,  blend);
        //add(frame, edges, blend);
        blend.setTo(255, edges);
        
        imshow("original", frame);
        imshow("edges", edges);
        imshow("blended", blend);
        if(waitKey(30) >= 0) {
            break;
        }
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    
    return 0;
    
}