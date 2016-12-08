//
//  main.cpp
//  TLD

#include <iostream>
#include <opencv2/opencv.hpp>
#include "video/VideoController.h"
#include "utils/RandomFernsClassifier.h"
#include "utils/NNClassifier.h"
#include "detector/Detector.h"
#include "TLD.h"
#include "TLDSystemStruct.h"

using namespace std;
using namespace cv;

void testOnTLDDataset() {
    string dir("/Users/liuguiyang/Documents/CodeProj/ConsoleProj/TLD/data/Car24/");

    string initFilename(dir + "init.txt");
    string retFilename(dir + "program.txt");

    FILE *fin = fopen(initFilename.c_str(), "r");
    FILE *fout = fopen(retFilename.c_str(), "w");

    VideoController videoController(dir, ".jpg");

    videoController.readNextFrame();

    int tlx, tly, brx, bry;
    fscanf(fin, "%d,%d,%d,%d", &tlx, &tly, &brx, &bry);
    fclose(fin);

    fprintf(fout, "%d,%d,%d,%d\n", tlx, tly, brx, bry);

//    Rect rect = Rect(Point2d(tlx, tly), Point2d(brx, bry));
    Rect rect = Rect(tlx, tly, brx, bry);
    cout << "Input Rect : " <<  rect << endl;

    videoController.drawRect(rect, COLOR_BLUE);
    videoController.showCache();
    waitKey();

    TLD tld(videoController.getCurrFrame(), rect);

    while(videoController.readNextFrame()) {
        tld.setNextFrame(videoController.getCurrFrame());

        TYPE_DETECTOR_RET bbDetect;

        clock_t st = clock();
        tld.track();
        clock_t ed = clock();
        cout << "Time : " << (double)(ed - st) / CLOCKS_PER_SEC * 1000 << "ms" << endl;

        videoController.drawRect(tld.getBB(), COLOR_GREEN, 2);
        videoController.showCache();
        if(waitKey(1) == 27) {
            break;
        }

        Rect retBB = tld.getBB();
        if(retBB == Rect(Point2d(-1, -1), Point2d(-1, -1))) {
            fprintf(fout, "NaN,NaN,NaN,NaN\n");
        } else {
            fprintf(fout, "%d,%d,%d,%d\n", retBB.tl().x, retBB.tl().y, retBB.width, retBB.height);
        }
    }

    fclose(fout);
}

//void testOnVideo() {
//    string filename("/Users/Orthocenter/Developments/TLD/1.m4v");
//
//    VideoController videoController(filename);
//    ViewController viewController(&videoController);
//
//    videoController.readNextFrame();
//
//    Rect rect = viewController.getRect();
//    cout << "Input Rect : " <<  rect << endl;
//
//    viewController.refreshCache();
//    viewController.drawRect(rect, COLOR_BLUE);
//    viewController.showCache();
//    waitKey();
//
//    TLD tld(videoController.getCurrFrame(), rect);
//
//    while(videoController.readNextFrame()) {
//        cout << "Frame #" << videoController.frameNumber() << endl;
//        tld.setNextFrame(videoController.getCurrFrame());
//
//        Rect bbTrack;
//        TYPE_DETECTOR_RET bbDetect;
//
//        clock_t st = clock();
//
//        tld.track();
//
//        clock_t ed = clock();
//        cout << "Time : " << (double)(ed - st) / CLOCKS_PER_SEC * 1000 << "ms" << endl;
//
//        viewController.refreshCache();
//        viewController.drawRect(tld.getBB(), COLOR_GREEN, 2);
//        viewController.showCache();
//
//        cout << endl;
//    }
//
//}

void testOnCamera() {
    VideoController videoController(0);
    videoController.readNextFrame();
    Rect rect = videoController.getRect();
    cout << "Input Rect : " <<  rect << endl;
    
    TLD tld(videoController.getCurrFrame(), rect);

    namedWindow("TLD", CV_WINDOW_AUTOSIZE);
    imshow("TLD", videoController.getCurrFrame());

    while(videoController.readNextFrame()) {
//        cout << "Frame #" << videoController.frameNumber() << endl;
        tld.setNextFrame(videoController.getCurrFrame());
        
        clock_t st = clock();
        tld.track();
        clock_t ed = clock();
        cout << "Time : " << (double)(ed - st) / CLOCKS_PER_SEC * 1000 << "ms" << endl;

        videoController.drawRect(tld.getBB(), COLOR_GREEN, 2);
        videoController.showCache();
        if(waitKey(1) == 27) {
            break;
        }
    }
    destroyAllWindows();

}

int main(int argc, char *argv[]) {
    testOnTLDDataset();
//    testOnVideo();
//    testOnCamera();
    return 0;
}


