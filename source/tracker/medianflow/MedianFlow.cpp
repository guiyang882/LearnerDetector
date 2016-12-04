//  MedianFlow.cpp
//  MedianFlow

#include "MedianFlow.h"

MedianFlow* MedianFlow::instanceMedianFlowPointer = NULL;

MedianFlow* MedianFlow::getInstance() {
    if(instanceMedianFlowPointer == NULL) {
        instanceMedianFlowPointer = new MedianFlow();
    }
    return instanceMedianFlowPointer;
}

MedianFlow::MedianFlow() {
    opticalFlow = new OpticalFlow();
    opticalFlowSwap = new OpticalFlow();
}

MedianFlow::MedianFlow(const Mat &prevImg, const Mat &nextImg, ViewController *_viewController) {
    this->prevImg = prevImg;
    this->nextImg = nextImg;
    
    opticalFlow = new OpticalFlow(this->prevImg, this->nextImg);
    opticalFlowSwap = new OpticalFlow(this->nextImg, this->prevImg);
    
    viewController = _viewController;
}

void MedianFlow::updateImage(Mat prev, Mat next) {
    this->prevImg = prev;
    this->nextImg = next;

    opticalFlow->updateImage(this->prevImg, this->nextImg);
    opticalFlowSwap->updateImage(this->prevImg, this->nextImg);
}

MedianFlow::~MedianFlow() {
    delete opticalFlow;
    opticalFlow = NULL;
    
    delete opticalFlowSwap;
    opticalFlowSwap = NULL;

    delete instanceMedianFlowPointer;
    instanceMedianFlowPointer = NULL;
}

void MedianFlow::generatePts(const TYPE_MF_BB &_box, vector<TYPE_MF_PT> &ret) {
    TYPE_MF_PT tl(max(0.f, _box.tl().x), max(0.f, _box.tl().y));
    TYPE_MF_PT br(min((float)prevImg.cols, _box.br().x), min((float)prevImg.rows, _box.br().y));
    TYPE_MF_BB box(tl, br);
    
    float stepX = (float)(box.width - 2 * MF_HALF_PATCH_SIZE) / (MF_NPTS - 1);
    float stepY = (float)(box.height - 2 * MF_HALF_PATCH_SIZE) / (MF_NPTS - 1);
    int x0 = box.x + MF_HALF_PATCH_SIZE;
    int y0 = box.y + MF_HALF_PATCH_SIZE;
    
    if(!ret.empty()) ret.clear();
    
    for(int fx = 0; fx < MF_NPTS; fx++) {
        for(int fy = 0; fy < MF_NPTS; fy++) {
            ret.push_back(TYPE_MF_PT(x0 + fx * stepX, y0 + fy * stepY));
        }
    }
}

void MedianFlow::filterOFError(const vector<TYPE_MF_PT> &pts, const vector<uchar> &status, vector<int> &rejected) {
    for(int i = 0; i < pts.size(); i++) {
        if(status[i] == 0) rejected[i] |= MF_REJECT_OFERROR;
    }
}

void MedianFlow::filterFB(const vector<TYPE_MF_PT> &initialPts, const vector<TYPE_MF_PT> &FBPts, vector<int> &rejected) {
    int size = int(initialPts.size());
    vector<pair<float, int> > V;
    
    for(int i = 0; i < size; i++) {
        if(rejected[i] & MF_REJECT_OFERROR) continue;
        
        float dist = norm(Mat(initialPts[i]), Mat(FBPts[i]));
        V.push_back(make_pair(dist, i));
    }
    
    sort(V.begin(), V.end(), compare);
    
    for(int i = (int)V.size() / 2; i < V.size(); i++) {
        rejected[V[i].second] |= MF_REJECT_FB;
    }
}

float MedianFlow::calcNCC(const cv::Mat &img0, const cv::Mat &img1) {
    if(NCC_USE_OPENCV) {
        Mat nccMat;
        matchTemplate(img0, img1, nccMat, CV_TM_CCORR_NORMED);
        
        return nccMat.at<float>(0);
    } else {
        Mat v0, v1; // convert image to 1 dimension vector
        
        img0.convertTo(v0, CV_32F);
        img1.convertTo(v1, CV_32F);
        
        v0 = v0.reshape(0, v0.cols * v0.rows);
        v1 = v1.reshape(0, v1.cols * v1.rows);
        
        Scalar mean, stddev;
        meanStdDev(v0, mean, stddev);
        v0 -= mean.val[0];
        meanStdDev(v1, mean, stddev);
        v1 -= mean.val[0];
        
        Mat v01 = v0.t() * v1;
        
        float norm0, norm1;
        
        norm0 = norm(v0);
        norm1 = norm(v1);
        
        return v01.at<float>(0) / norm0 / norm1;
    }
}

void MedianFlow::filterNCC(const vector<TYPE_MF_PT> &initialPts, const vector<TYPE_MF_PT> &FPts, vector<int> &rejected) {
    int size = int(initialPts.size());
    vector<pair<float, int> > V;
    
    for(int i = 0; i < size; i++) {
        if(rejected[i] & MF_REJECT_OFERROR) continue;
        
        if(!isPointInside(initialPts[i], MF_HALF_PATCH_SIZE)) continue;
        if(!isPointInside(FPts[i], MF_HALF_PATCH_SIZE)) continue;
        
        Point2d win(MF_HALF_PATCH_SIZE, MF_HALF_PATCH_SIZE);
        Point2d pt1(initialPts[i].x, initialPts[i].y);
        Point2d pt2(FPts[i].x, FPts[i].y);
        
        // must be int
        Rect_<int> rect0(pt1 - win, pt1 + win);
        Rect_<int> rect1(pt2 - win, pt2 + win);
        
        float ncc = calcNCC(this->prevImg(rect0), this->nextImg(rect1));
        
        V.push_back(make_pair(ncc, i));
    }
    
    sort(V.begin(), V.end(), compare);
    
    for(int i = int(V.size()) / 2; i < V.size(); i++) {
        rejected[V[i].second] |= MF_REJECT_NCC;
    }
}

TYPE_MF_BB MedianFlow::trackBox(const TYPE_MF_BB &inputBox, int &status) {
    if(!isBoxUsable(inputBox)) {
        status = MF_TRACK_F_BOX;
        return BB_ERROR;
    }
    
    vector<TYPE_MF_PT> pts;
    generatePts(inputBox, pts);
    
    vector<TYPE_MF_PT> retF, retFB;
    vector<uchar> statusF, statusFB;
    
    retF = retFB = pts;
    
    opticalFlow->trackPts(pts, retF, statusF);
    opticalFlowSwap->trackPts(retF, retFB, statusFB);

    vector<int> rejected(MF_NPTS * MF_NPTS);
    
    filterOFError(retF, statusF, rejected);
    filterOFError(retFB, statusFB, rejected);
    
    filterFB(pts, retFB, rejected);
    filterNCC(pts, retF, rejected);
    
    TYPE_MF_BB ret;
    
    ret = calcRect(inputBox, pts, retF, retFB, rejected, status);
    
    if(status != MF_TRACK_SUCCESS) {
        return BB_ERROR;
    }
    
    return ret;
}
