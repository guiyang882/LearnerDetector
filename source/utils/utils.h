//
// Created by 贵阳 on 2016/12/4.
//

#ifndef TLD_UTILS_H
#define TLD_UTILS_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

#include "../TLDSystemStruct.h"

using namespace std;
using namespace cv;

void drawCircles(Mat &img, const vector<Point2f> &pts, const Scalar &color = COLOR_WHITE, int radius = 3);
void drawLines(Mat &img, const vector<Point2f> &firstPts, const vector<Point2f> &secondPts, const Scalar &color = COLOR_RED, int thickness = 1);
void s_drawRect(Mat &img, const Rect_<float> &rect, const Scalar &color = COLOR_WHITE, int thickness = 1);
void onMouse(int event, int x, int y, int, void *);

#endif //TLD_UTILS_H
