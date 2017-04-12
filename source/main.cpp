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

void demoLearning() {
    string listpath = "/Users/liuguiyang/Documents/CodeProj/PyProj/TinyObjectDetection/trainmodel/data/JL1th/image.list";
    string groundpath = "/Users/liuguiyang/Desktop/airplane-remote/ground.list";
    InputReader reader(listpath);

#ifdef OUTPUT_STD
    cout << "Init InputReader !" << endl;
    cout << "Reader from " << listpath << endl;
#endif

    Mat curImg;
    while(reader.readNextImage(curImg)) {

#ifdef OUTPUT_STD
        cout << curImg.size().width << ", " << curImg.size().height << endl;
#endif
        bool isMultiScale = false;
        clock_t st = clock();
        if(isMultiScale) {
            unordered_map<double, vector<Rect>> windows;
            const Rect minSize(0, 0, 32, 32);
            CandidateWindow::makeCandidateWindows(curImg, windows, minSize, 3);
            clock_t ed = clock();

            cout << "Use Time : " << (double)(ed - st) / CLOCKS_PER_SEC * 1000 << "ms" << endl;
            cout << "Direct Search nums :" << endl;
            for(auto item:windows) {
                cout << "\t" << item.first << ": " << item.second.size() << endl;
            }
        } else {
            vector<Rect> windows;
            const Rect minSize(0, 0, 32, 32);
            CandidateWindow::makeCandidateWindowsWithScale(curImg, windows, minSize, 1.0, 3);
            clock_t ed = clock();

            cout << "Use Time : " << (double)(ed - st) / CLOCKS_PER_SEC * 1000 << "ms" << endl;
            cout << "Direct Search nums :" << endl;
            cout << "\t" << windows.size() << endl;
        }
        cout << "Test for Crop Images ROI ..........." << endl;
        st = clock();
        if(isMultiScale) {
            unordered_map<double, vector<Mat>> windows;
            const Rect minSize(0, 0, 32, 32);
            CandidateWindow::makeCandidateWindowsROIMat(curImg, windows, minSize, 3);
            clock_t ed = clock();

            cout << "Use Time : " << (double)(ed - st) / CLOCKS_PER_SEC * 1000 << "ms" << endl;
            cout << "Direct Search nums :" << endl;
            for(auto item:windows) {
                cout << "\t" << item.first << ": " << item.second.size() << endl;
            }
        } else {
            vector<Mat> windows;
            const Rect minSize(0, 0, 32, 32);
            CandidateWindow::makeCandidateWindowsWithScaleROIMat(curImg, windows, minSize, 1, 3);
            clock_t ed = clock();

            cout << "Use Time : " << (double)(ed - st) / CLOCKS_PER_SEC * 1000 << "ms" << endl;
            cout << "Direct Search nums :" << endl;
            cout << "\t" << windows.size() << endl;
#ifdef CV_SHOW
            for(auto item:windows) {
                imshow("Crop ROI Image", item);
                waitKey();
            }
#endif
        }
        clock_t ed = clock();
        break;
    }
}

int main(int argc, char *argv[]) {
    demoLearning();
    return 0;
}


