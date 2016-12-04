//
// Created by 贵阳 on 2016/11/30.
//

#include "TrackerBase.h"

TrackerBase::TrackerBase() {

}

TrackerBase::~TrackerBase() {

}

// caution : prefix static can only be specified inside the class definition
bool TrackerBase::compare(const pair<float, int> &a, const pair<float, int> &b) {
    return a.first < b.first;
}

bool TrackerBase::isPointInside(const TYPE_MF_PT &pt, const TYPE_MF_COORD alpha) {
    int width = prevImg.cols, height = prevImg.rows;
    return (pt.x >= 0 + alpha) && (pt.y >= 0 + alpha) && (pt.x <= width - alpha) && (pt.y <= height - alpha);
}

bool TrackerBase::isBoxUsable(const TYPE_MF_BB &rect) {
    int width = prevImg.cols, height = prevImg.rows;

    // bounding box is too large
    if(rect.width > width || rect.height > height) return false;

    // intersection between rect and img is too small
    TYPE_MF_COORD tlx = max((TYPE_MF_COORD)rect.tl().x, (TYPE_MF_COORD)0);
    TYPE_MF_COORD tly = max((TYPE_MF_COORD)rect.tl().y, (TYPE_MF_COORD)0);
    TYPE_MF_COORD brx = min((TYPE_MF_COORD)rect.br().x, (TYPE_MF_COORD)width);
    TYPE_MF_COORD bry = min((TYPE_MF_COORD)rect.br().y, (TYPE_MF_COORD)height);

    TYPE_MF_BB bb(tlx, tly, brx - tlx, bry - tly);
    if(bb.width <= 2 * MF_HALF_PATCH_SIZE || bb.height <= 2 * MF_HALF_PATCH_SIZE) return false;

    // otherwise
    return true;
}

TYPE_MF_BB TrackerBase::calcRect(const TYPE_MF_BB &rect,
                                const vector<TYPE_MF_PT> &pts,
                                const vector<TYPE_MF_PT> &FPts,
                                const vector<TYPE_MF_PT> &FBPts,
                                const vector<int> &rejected,
                                int &status) {
    const int size = int(pts.size());

    vector<TYPE_MF_COORD> dxs, dys;

    for(int i = 0; i < size; i++) {
        if(rejected[i]) continue;

        dxs.push_back(FPts[i].x - pts[i].x);
        dys.push_back(FPts[i].y - pts[i].y);
    }

    if(dxs.size() <= 1) {
        status = MF_TRACK_F_PTS;
        return BB_ERROR;
    }

    sort(dxs.begin(), dxs.end());
    sort(dys.begin(), dys.end());

    TYPE_MF_COORD dx = dxs[dxs.size() / 2];
    TYPE_MF_COORD dy = dys[dys.size() / 2];
    TYPE_MF_PT delta(dx, dy);

    vector<float> ratios;
    vector<float> absDist;

    for(int i = 0; i < size; i++) {
        if(rejected[i]) continue;

        for(int j = i + 1; j < size; j++) {
            if(rejected[j]) continue;

            float dist0 = norm(Mat(pts[i]), Mat(pts[j]));
            float dist1 = norm(Mat(FPts[i]), Mat(FPts[j]));
            float ratio = dist1 / dist0;

            ratios.push_back(ratio);
        }
    }

    sort(ratios.begin(), ratios.end());
    float ratio = ratios[ratios.size() / 2];

    TYPE_MF_BB ret(delta + rect.tl(), delta + rect.br());

    TYPE_MF_PT center((float)(ret.tl().x + ret.br().x) / 2, (float)(ret.tl().y + ret.br().y) / 2);
    TYPE_MF_PT tl(center.x - ret.width / 2 * ratio, center.y - ret.height / 2 * ratio);
    TYPE_MF_PT br(center.x + ret.width / 2 * ratio, center.y + ret.height / 2 * ratio);

    ret = TYPE_MF_BB(tl, br);

    for(int i = 0; i < size; i++) {
        if(rejected[i] == MF_REJECT_OFERROR) continue;

        float dist = norm(Mat(pts[i]), Mat(FBPts[i]));

        absDist.push_back(dist);
    }

    sort(absDist.begin(), absDist.end());

    float medianAbsDist = absDist[(int)absDist.size() / 2];

    //caution : must add '&'
    for(auto &i : absDist) {
        i = abs(i - medianAbsDist);
    }

    sort(absDist.begin(), absDist.end());

    if(medianAbsDist > MF_FB_ERROR_DIST) {
        status = MF_TRACK_F_CONFUSION;
        return BB_ERROR;
    }

    if(!isBoxUsable(ret)) {
        status = MF_TRACK_F_BOX;
        return BB_ERROR;
    }

    status = MF_TRACK_SUCCESS;
    return ret;
}

void TrackerBase::updateImage(Mat prev, Mat next) {
    this->prevImg = prev;
    this->nextImg = next;
}