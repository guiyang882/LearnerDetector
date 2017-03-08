#include <iostream>
#include <opencv2/opencv.hpp>

#include "utils/RandomFernsClassifier.h"
#include "utils/NNClassifier.h"
#include "detector/Detector.h"
#include "TLD.h"
#include "TLDSystemStruct.h"
#include "utils/datareader.h"

using namespace std;
using namespace cv;

void testOnTLDDataset() {
    string listpath = "/Users/liuguiyang/Desktop/airplane-remote/path.list";
    string groundpath = "/Users/liuguiyang/Desktop/airplane-remote/ground.list";
    InputReader reader(listpath, groundpath);

    TLD tld; Mat curImg; vector<Rect> rectVec;
    while(reader.readNextImage(curImg, rectVec)) {

        tld.setNextFrame(curImg);

        TYPE_DETECTOR_RET bbDetect;

        clock_t st = clock();
        tld.track();
        clock_t ed = clock();
        cout << "Time : " << (double)(ed - st) / CLOCKS_PER_SEC * 1000 << "ms" << endl;

        Rect retBB = tld.getBB();
        if(retBB == Rect(Point2d(-1, -1), Point2d(-1, -1))) {
            cout << "NaN,NaN,NaN,NaN" << endl;
        } else {
            cout << retBB.tl().x << retBB.tl().y << retBB.width << retBB.height << endl;
        }
    }
}

int main(int argc, char *argv[]) {
    testOnTLDDataset();
    return 0;
}


