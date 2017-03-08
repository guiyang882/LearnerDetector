#include <iostream>
#include <opencv2/opencv.hpp>
#include "utils/RandomFernsClassifier.h"
#include "utils/NNClassifier.h"
#include "detector/Detector.h"
#include "TLD.h"
#include "TLDSystemStruct.h"

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

int main(int argc, char *argv[]) {
   testOnTLDDataset();
    return 0;
}


