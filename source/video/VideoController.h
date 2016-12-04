//
//  VideoController.h
//  MedianFlow

#ifndef __MedianFlow__VideoController__
#define __MedianFlow__VideoController__

#include <iostream>
#include <opencv2/opencv.hpp>
#include "../TLDSystemStruct.h"
#include "../utils/utils.h"

using namespace std;
using namespace cv;

class VideoController {
public:
    bool cameraMode, imageMode;
    
    VideoController(const string &path);
    VideoController(const string &path, const string &append);
    VideoController(int camera = 0);

    ~VideoController();

public:
    Mat getCurrFrame();
    Mat getPrevFrame();
    bool readNextFrame();
    
    Size frameSize();
    int frameNumber();
    void jumpToFrameNum(int num);

    void showCache(const string &winName = string());
    Rect getRect();
    void drawRect(const Rect_<float> &rect, const Scalar &color = COLOR_WHITE, int thickness = 1);

private:
    int curr, frame;
    Mat frames[2], cache;
    Size _frameSize;
    VideoCapture *videoCapture;

    string path, append, retWindowName;
    int totalFrame;
};

#endif /* defined(__MedianFlow__VideoController__) */
