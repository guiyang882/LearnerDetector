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
#include "vibe/vibe-background-sequential.h"

using namespace std;
using namespace cv;

void testOnTLDDataset() {
    string dir("/Users/liuguiyang/Documents/CodeProj/ConsoleProj/TLD/data/Bolt2/");

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
        if(waitKey() == 27) {
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

void testOnVideo() {
    string filename("/Volumes/Ubuntu/VideoData/Basic-Product-Samples-selected/Iris/Iris_Dubai_Sample.mp4");

    VideoController videoController(filename, true);

    videoController.readNextFrame();

    Rect rect = videoController.getRect();
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
    }

}

void testViBe() {
    string filename("/Volumes/Ubuntu/VideoData/Basic-Product-Samples-selected/Iris/Iris_Dubai_Sample.mp4");

    VideoController videoController(filename, true);
    Mat frame = videoController.getCurrFrame();
    Mat segmentationMap = Mat(frame.rows, frame.cols, CV_8UC1);
    vibeModel_Sequential_t *model = (vibeModel_Sequential_t*)libvibeModel_Sequential_New();
    libvibeModel_Sequential_AllocInit_8u_C3R(model, frame.data, frame.cols, frame.rows);

    /* Create GUI windows. */
    namedWindow("Frame");
    namedWindow("Segmentation by ViBe");

    while(videoController.readNextFrame()) {
        frame = videoController.getCurrFrame();
        imshow("Frame", frame);

        /* ViBe: Segmentation and updating. */
        libvibeModel_Sequential_Segmentation_8u_C3R(model, frame.data, segmentationMap.data);
        libvibeModel_Sequential_Update_8u_C3R(model, frame.data, segmentationMap.data);

        /* Post-processes the segmentation map. This step is not compulsory.
           Note that we strongly recommend to use post-processing filters, as they
           always smooth the segmentation map. For example, the post-processing filter
           used for the Change Detection dataset (see http://www.changedetection.net/ )
           is a 5x5 median filter. */
        medianBlur(segmentationMap, segmentationMap, 3); /* 3x3 median filtering */

        /* Shows the current frame and the segmentation map. */
        imshow("Segmentation by ViBe", segmentationMap);
        char keyboard = waitKey(10);
        if(keyboard == 'q' || keyboard == 27) {
            break;
        }
    }
    /* Frees the model. */
    libvibeModel_Sequential_Free(model);
    destroyAllWindows();
}

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
//    testOnTLDDataset();
//    testOnVideo();
//    testOnCamera();
    testViBe();
    return 0;
}


