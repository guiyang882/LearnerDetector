#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

#include "utils/RandomFernsClassifier.h"
#include "utils/NNClassifier.h"
#include "detector/Detector.h"
#include "TLD.h"
#include "TLDSystemStruct.h"
#include "utils/datareader.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

using namespace std;
using namespace cv;

char* serverip = "127.0.0.1";
const int serverport = 9999;
int sockfd = -1;
struct sockaddr_in serv_addr;

bool initCommunication() {
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("\n Error : Could not create socket \n");
        return false;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(serverip);
    serv_addr.sin_port = htons(serverport);
    return true;
}

int sendImage2Server(const vector<Mat>& bufImgs)  {
    char buf[10] = {};
    if(sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) {
        ;
    }
    return 0;
}

void saveImage2Disk(const vector<Mat>& bufImgs, int frameid) {
    /*主要将一批次图像，存储在一个二进制文件中，文件名字就是对应的帧的标号*/
    vector<vector<uchar> >array(bufImgs.size(), vector<uchar>(bufImgs[0].rows * bufImgs[0].cols, '\0'));
    for(int i=0;i<bufImgs.size();i++) {
        if(bufImgs[i].isContinuous()) {
            array[i].assign(bufImgs[i].datastart, bufImgs[i].dataend);
        } else {
            for(int k=0;k<bufImgs[i].rows;k++) {
                array[i].insert(array[i].end(), bufImgs[i].ptr<uchar>(k), bufImgs[i].ptr<uchar>(k) + bufImgs[i].cols);
            }
        }
    }
    int totalImgs = bufImgs.size();
    int rows = bufImgs[0].rows;
    int cols = bufImgs[0].cols;
    char buf[100] = {'\0'};
    sprintf(buf, "%04d", frameid);
    string filepath = string(buf) + "_" + to_string(totalImgs) + "_" + to_string(rows) + "_" + to_string(cols) + ".bin";
    cout << "save path is " << filepath << endl;
    ofstream fout(filepath, ios::binary);
    if(!fout) return ;
    for(int i=0;i<array.size();i++) {
        for(int j=0;j<array[i].size();j++) {
            fout << array[i][j];
        }
    }
    fout.close();
}

void demoLearning() {
    string listpath = "/Users/liuguiyang/Documents/CodeProj/PyProj/TinyObjectDetection/trainmodel/data/JL1th/image.list";
    string groundpath = "/Users/liuguiyang/Desktop/airplane-remote/ground.list";
    InputReader reader(listpath);

#ifdef OUTPUT_STD
    cout << "Init InputReader !" << endl;
    cout << "Reader from " << listpath << endl;
#endif

    Mat curImg; int frameid = 1; int mode = 1; // means gray scale
    while(reader.readNextImage(curImg, mode)) {

#ifdef OUTPUT_STD
        cout << curImg.size().width << ", " << curImg.size().height << ", " << curImg.channels() << endl;
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
            saveImage2Disk(windows, frameid);
            frameid++;
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


