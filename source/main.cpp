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

    Mat curImg; vector<Rect> rectVec;
    while(reader.readNextImage(curImg, rectVec)) {
        clock_t st = clock();
        unordered_map<double, vector<Rect>> windows;
        const Rect minSize(0, 0, 20, 20);
        CandidateWindow::makeCandidateWindows(curImg, windows, minSize);
        clock_t ed = clock();
        cout << "Time : " << (double)(ed - st) / CLOCKS_PER_SEC * 1000 << "ms" << endl;
        for(auto item:windows) {
            cout << item.first << " : " << item.second.size() << endl;
        }
        break;
    }
}

int main(int argc, char *argv[]) {
    testOnTLDDataset();
    return 0;
}


