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
    string retFilename(dir + "myRet7.txt");

    FILE *fin = fopen(initFilename.c_str(), "r");
    FILE *fout = fopen(retFilename.c_str(), "w");

    VideoController videoController(dir, ".jpg");

    videoController.readNextFrame();

    int tlx, tly, brx, bry;
    fscanf(fin, "%d,%d,%d,%d", &tlx, &tly, &brx, &bry);
    fprintf(fout, "%d,%d,%d,%d\n", tlx, tly, brx, bry);

    Rect rect = Rect(Point2d(tlx, tly), Point2d(brx, bry));
    cout << "Input Rect : " <<  rect << endl;

    videoController.drawRect(rect, COLOR_BLUE);
    videoController.showCache();
    waitKey();

    TLD tld(videoController.getCurrFrame(), rect);

    while(videoController.readNextFrame()) {
        cout << "Frame #" << videoController.frameNumber() << endl;
        tld.setNextFrame(videoController.getCurrFrame());

        Rect bbTrack;
        TYPE_DETECTOR_RET bbDetect;

        clock_t st = clock();

        tld.track();

        clock_t ed = clock();
        cout << "Time : " << (double)(ed - st) / CLOCKS_PER_SEC * 1000 << "ms" << endl;

        videoController.drawRect(tld.getBB(), COLOR_GREEN, 2);
        videoController.showCache();

        cout << endl;

        Rect retBB = tld.getBB();
        if(retBB == Rect(Point2d(-1, -1), Point2d(-1, -1))) {
            fprintf(fout, "NaN,NaN,NaN,NaN\n");
        } else {
            fprintf(fout, "%d,%d,%d,%d\n", retBB.tl().x, retBB.tl().y, retBB.br().x, retBB.br().y);
        }
    }

    fclose(fin);
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

//void stabilize() {
//    string dir("/home/cyx/Developments/OpenTLD/_input/06_car/");
//    VideoController videoController("/Users/Orthocenter/Developments/TLD/4.mov");
//    ViewController viewController(&videoController);
//
//    videoController.readNextFrame();
//
//    Rect rect;
//    bool drawBBox = true;
//    if(drawBBox) {
//        rect = viewController.getRect();
//    } else {
//        string initFilename(dir + "init.txt");
//        FILE *fin = fopen(initFilename.c_str(), "r");
//        int tlx, tly, brx, bry;
//        fscanf(fin, "%d,%d,%d,%d", &tlx, &tly, &brx, &bry);
//
//        rect = Rect(Point2d(tlx, tly), Point2d(brx, bry));
//        fclose(fin);
//    }
//
//    cout << "Input Rect : " <<  rect << endl;
//
//    TLD tld(videoController.getCurrFrame(), rect);
//
//    int width = videoController.frameSize().width;
//    int height = videoController.frameSize().height;
//
//    Point2i centerOut;
//    centerOut.x = width / 2;
//    centerOut.y = height / 2;
//
//    while(videoController.readNextFrame()) {
//        tld.setNextFrame(videoController.getCurrFrame());
//        int status = tld.track();
//
//        if(status == TLD_TRACK_SUCCESS) {
//            Point2i centerIn;
//            centerIn.x = (tld.getBB().tl().x + tld.getBB().br().x) / 2;
//            centerIn.y = (tld.getBB().tl().y + tld.getBB().br().y) / 2;
//
//            Mat output(videoController.frameSize(), CV_8UC3, Scalar::all(0));
//
//            int dx = centerOut.x - centerIn.x;
//            int dy = centerOut.y - centerIn.y;
//
//            Point2i inTl(max(0, -dx), max(0, -dy));
//            Point2i inBr(min(width, width - dx), min(height, height - dy));
//            Point2i outTl(max(0, dx), max(0, dy));
//            Point2i outBr(min(width, width + dx), min(height, height + dy));
//            Rect ROIIn(inTl, inBr);
//            Rect ROIOut(outTl, outBr);
//
//            videoController.getCurrFrame()(ROIIn).copyTo(output(ROIOut));
//            imshow("Stabilize", output);
//            waitKey(1);
//        } else {
//            Mat output(videoController.frameSize(), CV_8U, Scalar::all(0));
//
//            imshow("Stabilize", output);
//            waitKey(1);
//        }
//
//        viewController.refreshCache();
//        viewController.drawRect(tld.getBB(), COLOR_GREEN, 2);
//        viewController.showCache();
//    }
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
    
    //stabilize();
    return 0;
}


