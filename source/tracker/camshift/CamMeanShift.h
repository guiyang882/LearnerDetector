//
// Created by 贵阳 on 2016/12/4.
//

#ifndef TLD_MEANSHIFT_H
#define TLD_MEANSHIFT_H

#include "../TrackerBase.h"

class CamMeanShift : public TrackerBase {
private:
    CamMeanShift();
    static CamMeanShift* instanceMeanShift;

public:
    ~CamMeanShift();
    static CamMeanShift* getInstance();

public:
    TYPE_MF_BB trackBox(const TYPE_MF_BB &inputBox, int &status);
};


#endif //TLD_MEANSHIFT_H
