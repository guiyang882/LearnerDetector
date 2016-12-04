//
// Created by 贵阳 on 2016/12/4.
//

#include "CamMeanShift.h"

CamMeanShift* CamMeanShift::instanceMeanShift = NULL;

CamMeanShift::CamMeanShift() {

}

CamMeanShift::~CamMeanShift() {
    if(instanceMeanShift) {
        delete instanceMeanShift;
        instanceMeanShift = NULL;
    }
}

CamMeanShift* CamMeanShift::getInstance() {
    if(instanceMeanShift == NULL) {
        instanceMeanShift = new CamMeanShift();
    }
    return instanceMeanShift;
}

TYPE_MF_BB CamMeanShift::trackBox(const TYPE_MF_BB &inputBox, int &status) {
    if(!isBoxUsable(inputBox)) {
        status = MF_TRACK_F_BOX;
        return BB_ERROR;
    }

    // write your strategy

    // end your strategy

    if(status != MF_TRACK_SUCCESS) {
        return BB_ERROR;
    }
}