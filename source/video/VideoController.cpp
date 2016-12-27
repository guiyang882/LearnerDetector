//
//  VideoController.cpp
//  MedianFlow

#include "VideoController.h"

VideoController::VideoController(const string &path, bool isCorp):
    curr(0), frame(0), cameraMode(false), imageMode(false), videoMode(true), retWindowName("TLD")
{
    videoCapture = new VideoCapture(path);
    Mat tmp;
    if(videoCapture->isOpened() && videoCapture->read(tmp)) {
        int width = tmp.cols, height = tmp.rows;
        if(isCorp) {
            _videoROIRect = Rect(0, 0, width/2, height/4);
            _frameSize = Size(width/2, height/4);
            frames[curr] = tmp(_videoROIRect);
        } else {
            _frameSize = Size(width, height);
            frames[curr] = tmp;
        }
    } else {
        cerr << "Unable to read next frame." << endl;
        cerr << "Exiting..." << endl;
        exit(EXIT_FAILURE);
    }
}

VideoController::VideoController(const string &_path, const string &_append):
    curr(0), frame(0), cameraMode(false), imageMode(true), append(_append), path(_path), retWindowName("TLD")
{
    string frameFilename(path + "framenum.txt");
    FILE *fin = fopen(frameFilename.c_str(), "r");
    fscanf(fin, "%d", &totalFrame);
    fclose(fin);
    path += "source/";
    Mat tmp = imread(path + "0001" + append);
    _frameSize = tmp.size();
}

VideoController::VideoController(int camera):
    curr(0), frame(0), cameraMode(true), imageMode(false), retWindowName("TLD")
{
    videoCapture = new VideoCapture(camera);

    videoCapture->set(CV_CAP_PROP_FRAME_WIDTH, CAM_CAP_IMG_WIDTH);
    videoCapture->set(CV_CAP_PROP_FRAME_HEIGHT, CAM_CAP_IMG_HEIGHT);
    _frameSize = Size(CAM_CAP_IMG_WIDTH, CAM_CAP_IMG_HEIGHT);
}

VideoController::~VideoController() {
    if(cameraMode) {
        delete videoCapture;
        videoCapture = NULL;
    } else if(videoMode) {
        delete videoCapture;
        videoCapture = NULL;
    }
}

Mat VideoController::getCurrFrame() {
    return frames[curr];
}

Mat VideoController::getPrevFrame() {
    return frames[curr ^ 1];
}

bool VideoController::readNextFrame() {
    curr ^= 1;

    if(imageMode) {
        if(++frame <= totalFrame) {
            char filename[20];
            sprintf(filename, "%04d", frame);
            string fullPath = path + filename + append;
            frames[curr] = imread(fullPath);
            frames[curr].copyTo(cache);
            return true;
        }
        return false;
    } else if(cameraMode) {
        bool f = videoCapture -> read(frames[curr]);
        frames[curr].copyTo(cache);
        return f;
    } else if(videoMode) {
        Mat tmpFrame;
        bool f = videoCapture->read(tmpFrame);
        frames[curr] = tmpFrame(_videoROIRect);
        frames[curr].copyTo(cache);
        return f;
    }
    return false;
}

Size VideoController::frameSize()
{
    return _frameSize;
}

int VideoController::frameNumber() {
    return frame;
}

void VideoController::jumpToFrameNum(int num) {
    while(frameNumber() < num)
        readNextFrame();
}

void VideoController::showCache(const string &winName) {
    if(winName.size())
        imshow(winName, cache);
    else
        imshow(retWindowName, cache);
}

void VideoController::drawRect(const Rect_<float> &rect, const Scalar &color, int thickness) {
    rectangle(cache, rect, color, thickness);
}

Rect VideoController::getRect() {
    namedWindow(retWindowName, CV_WINDOW_AUTOSIZE);
    imshow(retWindowName, getCurrFrame());

    Rect rect(0, 0, CAM_CAP_IMG_WIDTH, CAM_CAP_IMG_HEIGHT);
    pair<void*, void*> p(&rect, this);

    bool selectValid = false;

    pair<pair<void*, void*>, bool*> pp(p, &selectValid);
    setMouseCallback(retWindowName, onMouse, &pp);

    if(cameraMode) {
        while(!selectValid) {
            readNextFrame();
            drawRect(rect);
            imshow(retWindowName, cache);
            if(waitKey(5) == 27) {
                break;
            }
        }
    } else {
        while(!selectValid) {
            waitKey();
        }
    }
    destroyWindow(retWindowName);

    return rect;
}
