//
//  TLD.h
//  TLD

#ifndef __TLD__TLD__
#define __TLD__TLD__

#include <iostream>
#include <queue>
#include <opencv2/opencv.hpp>
#include <algorithm>

#include "tracker/medianflow/MedianFlow.h"
#include "detector/Detector.h"
#include "learner/Learner.h"

#include "TLDSystemStruct.h"

using namespace std;
using namespace cv;

class TLD {
private:
    MedianFlow *medianflow_tracker;
    Detector detector;
    Learner learner;
    
    Mat prevImg, nextImg, nextImgB, nextImg32F;
    TYPE_BBOX bbox;
    
    TYPE_DETECTOR_RET detectorRet;
    TYPE_DETECTOR_RET clusterBB;
    
    bool trainValid;
    
    float overlap(const TYPE_BBOX &bb1, const TYPE_BBOX &bb2);
    TYPE_BBOX getInside(const TYPE_BBOX &bb);
    
    int cluster();
    
public:
    TLD(const Mat &img, const TYPE_BBOX &bb);
    ~TLD();
    
    void setNextFrame(const Mat &frame);
    int track(TRACK_TYPE track_type = MEDIANFLOW);
    int learning_detecting(int trackerStatus, TYPE_DETECTOR_SCANBB& trackerRet);
    TYPE_BBOX getBB();
};

#endif /* defined(__TLD__TLD__) */
