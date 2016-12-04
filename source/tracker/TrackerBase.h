//
// Created by 贵阳 on 2016/11/30.
//

#ifndef TLD_TRACKERBASE_H
#define TLD_TRACKERBASE_H
/*
 * Tracker算法的基类
 * 其他算法继承这个类，进行继承和改进
 */

#include <iostream>
#include <cmath>
#include "../TLDSystemStruct.h"
#include "../video/ViewController.h"

using namespace std;

class TrackerBase {
public:
    TrackerBase();
    ~TrackerBase();

public:
    static bool compare(const pair<float, int> &a, const pair<float, int> &b);

    bool isPointInside(const TYPE_MF_PT &pt, const TYPE_MF_COORD border = 0);
    bool isBoxUsable(const TYPE_MF_BB &rect);
    TYPE_MF_BB calcRect(const TYPE_MF_BB &rect, const vector<TYPE_MF_PT> &pts, const vector<TYPE_MF_PT> &FPts,  const vector<TYPE_MF_PT> &FBPts, const vector<int> &rejected, int &status);
    void updateImage(Mat prev, Mat next);

public:
    virtual TYPE_MF_BB trackBox(const TYPE_MF_BB &inputBox, int &status) = 0;

protected:
    Mat prevImg, nextImg;
    ViewController *viewController;
};


#endif //TLD_TRACKERBASE_H
