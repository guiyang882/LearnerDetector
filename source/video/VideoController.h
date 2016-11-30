//
//  VideoController.h
//  MedianFlow

#ifndef __MedianFlow__VideoController__
#define __MedianFlow__VideoController__

#include <iostream>
#include <opencv2/opencv.hpp>
#include "../TLDSystemStruct.h"

using namespace std;
using namespace cv;

class VideoController {
private:
    int curr, frame;
    Mat frames[2];
    Size _frameSize;
    VideoCapture *videoCapture;
    
    string path, append;
    int totalFrame;
    
public:
    bool cameraMode, imageMode;
    
    VideoController();
    VideoController(const string &path);
    VideoController(const string &path, const string &append);
    VideoController(int camera = 0);

    ~VideoController();

    Mat getCurrFrame();
    Mat getPrevFrame();
    bool readNextFrame();
    
    Size frameSize();
    int frameNumber();
    void jumpToFrameNum(int num);
};

#endif /* defined(__MedianFlow__VideoController__) */