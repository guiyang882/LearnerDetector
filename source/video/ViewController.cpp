//
//  ViewController.cpp
//  MedianFlow

#include "ViewController.h"

ViewController::~ViewController() {
}

ViewController::ViewController(VideoController *videoController):
    retWindowName("TLD") {
    this->videoController = videoController;
}

void ViewController::refreshCache() {
    if(videoController->cameraMode)
        videoController->readNextFrame();
    videoController->getCurrFrame().copyTo(cache);
}

void ViewController::drawLines(const vector<Point2f> &firstPts,
                               const vector<Point2f> &secondPts,
                               const Scalar &color, int thickness) {
    for(int i = 0; i < firstPts.size(); i++) {
        if(secondPts[i] == PT_ERROR)
            continue;
        line(cache, firstPts[i], secondPts[i], color, thickness);
    }
}

void ViewController::drawCircles(const vector<Point2f> &pts, const Scalar &color, int radius) {
    for(int i = 0; i < pts.size(); i++) {
        circle(cache, pts[i], radius, color);
    }
}

void ViewController::showCache(const string &winName) {
    if(winName.size())
        imshow(winName, cache);
    else
        imshow(retWindowName, cache);
}

void ViewController::drawRect(const Rect_<float> &rect, const Scalar &color, int thickness) {
    rectangle(cache, rect, color, thickness);
}

void ViewController::onMouse(int event, int x, int y, int flags, void* param) {
    pair<pair<void*, void*>, bool*> pp = *(pair<pair<void*, void*>, bool*>*)(param);
    pair<void*, void*> p = pp.first;
    
    bool &selectValid = *((bool*)pp.second);
    Rect &rect = *((Rect*)p.first);
    ViewController &viewController = *((ViewController*)p.second);
    int width = viewController.videoController->getCurrFrame().cols;
    int height = viewController.videoController->getCurrFrame().rows;

    static Point2i pre_pos = {0, 0}, cur_pos = {0, 0};

    if(event == CV_EVENT_LBUTTONDOWN && flags != CV_EVENT_FLAG_LBUTTON) {
        selectValid = false;
        pre_pos = Point2i(x, y);
    } else if(flags == CV_EVENT_FLAG_LBUTTON && event == CV_EVENT_LBUTTONDOWN) {
        cur_pos = Point2i(x, y);
        int x0 = min(pre_pos.x, cur_pos.x);
        int y0 = min(pre_pos.y, cur_pos.y);
        int x1 = max(pre_pos.x, cur_pos.x) - x0;
        int y1 = max(pre_pos.y, cur_pos.y) - y0;
        rect = Rect(x0, y0, x1, y1);
        selectValid = false;
        cout << "Mouse Move Here !" << endl;
    } else if(event == CV_EVENT_LBUTTONUP) {
        cur_pos = Point2i(x, y);
        int x0 = min(pre_pos.x, cur_pos.x);
        int y0 = min(pre_pos.y, cur_pos.y);
        int x1 = max(pre_pos.x, cur_pos.x) - x0;
        int y1 = max(pre_pos.y, cur_pos.y) - y0;
        rect = Rect(x0, y0, x1, y1);
        selectValid = true;
        cout << "Mouse Left Up !" << endl;
    }
}

Rect ViewController::getRect() {
    namedWindow(retWindowName, CV_WINDOW_AUTOSIZE);
    imshow(retWindowName, videoController->getCurrFrame());
    int width = videoController->getCurrFrame().cols;
    int height = videoController->getCurrFrame().rows;

    Rect rect(0, 0, width, height);
    pair<void*, void*> p(&rect, this);
    
    bool selectValid = false;
    
    pair<pair<void*, void*>, bool*> pp(p, &selectValid);
    setMouseCallback(retWindowName, ViewController::onMouse, &pp);

    if(videoController->cameraMode) {
        while(true) {
            refreshCache();
            drawRect(rect);
            imshow(retWindowName, cache);
            if(selectValid)
                break;
        }
    } else {
        while(!selectValid) {
            waitKey();
        }
    }
    destroyWindow(retWindowName);
    
    return rect;
}