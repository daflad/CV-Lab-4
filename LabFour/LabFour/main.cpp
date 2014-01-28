//
//  main.cpp
//  LabFour
//
//  Created by Stephen John Russell on 27/01/2014.
//  Copyright (c) 2014 Stephen John Russell. All rights reserved.
//

#include "VideoProcessor.hpp"

int main(int argc, char* argv[]) {

    VideoProcessor vp(argv[1]);
    
    return vp.processImage();
    
}