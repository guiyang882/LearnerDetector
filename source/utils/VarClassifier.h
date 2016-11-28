//
//  VarClassifier.h
//  TLD

#ifndef __TLD__VarClassifier__
#define __TLD__VarClassifier__

#include <iostream>
#include <opencv2/opencv.hpp>
#include "../TLDSystemStruct.h"

using namespace std;
using namespace cv;

class VarClassifier {
private:
    Mat sum, sqsum;
    
public:
    VarClassifier(){}
    VarClassifier(const Mat &img);
    
    ~VarClassifier(){}
    
    float getVar(const Rect &bb);
    bool getClass(TYPE_DETECTOR_SCANBB &bb, float patternVar);
};

#endif /* defined(__TLD__VarClassifier__) */
