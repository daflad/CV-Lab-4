//
//  main.cpp
//  LabFour
//
//  Created by Stephen John Russell on 27/01/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#include "VideoProcessor.hpp"

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
    
    // Play video applying 
    vp.processVideo();
    return 0;
}