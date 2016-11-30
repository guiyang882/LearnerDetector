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

    static bool compare(const pair<float, int> &a, const pair<float, int> &b);

protected:
    Mat prevImg, nextImg;
    ViewController *viewController;
};


#endif //TLD_TRACKERBASE_H
