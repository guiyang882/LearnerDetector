//
//  MedianFlow.h
//  MedianFlow

#ifndef __MedianFlow__MedianFlow__
#define __MedianFlow__MedianFlow__



#include <cmath>
#include <iostream>

#include "OpticalFlow.h"
#include "../../TLDSystemStruct.h"
#include "../TrackerBase.h"

using namespace std;
using namespace cv;

class MedianFlow : public TrackerBase {
private:
    OpticalFlow *opticalFlow, *opticalFlowSwap;

    void generatePts(const TYPE_MF_BB &box, vector<TYPE_MF_PT> &ret);
    float calcNCC(const Mat &img0, const Mat &img1);
    
    void filterOFError(const vector<TYPE_MF_PT> &pts, const vector<uchar> &retF, vector<int> &rejected);
    void filterFB(const vector<TYPE_MF_PT> &initialPts, const vector<TYPE_MF_PT> &FBPts, vector<int> &rejected);
    void filterNCC(const vector<TYPE_MF_PT> &initialPts, const vector<TYPE_MF_PT> &FPts, vector<int> &rejected);
    
public:
    MedianFlow();
 
    // prevImg & nextImg should be CV_8U
    // viewController for showing MedianFlow results
    MedianFlow(const Mat &prevImg, const Mat &nextImg, ViewController *viewController = NULL);
    ~MedianFlow();
    
    TYPE_MF_BB trackBox(const TYPE_MF_BB &inputBox, int &status);
};

#endif /* defined(__MedianFlow__MedianFlow__) */
