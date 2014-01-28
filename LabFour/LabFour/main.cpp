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
int main(int argc, const char * argv[])
{
    // Check any arguments have been passed
    if (argc >= 2) {
        
        VideoProcessor vp(argv[1]);
        if (vp.ERROR) {
            cerr << vp.err << endl;
            return -1;
        }
        vp.playMedia();
    }
    return 0;
}