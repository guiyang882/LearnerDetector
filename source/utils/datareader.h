#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <queue>
#include <unordered_map>

#include <opencv2/opencv.hpp>
#include <unistd.h>

#ifdef _OPENMP

#if defined(__clang__)
#include <omp.h>
#elif defined(__GNUG__) || defined(__GNUC__)
#include <omp.h>
#endif

#endif

using namespace std;
using namespace cv;

class InputReader {
public:
	// 主要读取图像文件
	InputReader(string listpath);
	// 主要读取图像文件和文件文件对应的xml文件
	InputReader(string listpath, string groundpath);
	~InputReader();

public:
	bool readNextImage(Mat &img);
	bool readNextImage(Mat &img, vector<Rect> &rectVec);

public:
	// 主要是用来读取物体标记出的目标的位置信息，返回一个vector<Rect>
	static bool readGroundFile(const string &path, vector<Rect> &vals);

private:
	string m_listpath;
	string m_groundpath;
	queue<string> m_queImgsPath; // 用来保存图像列表
	queue<string> m_queGroundPath; // 用来保存图像对应的label文本信息
};

class CandidateWindow {
public:
	// 对于给定的图像，产生候选框，返回在windows中
	static void makeCandidateWindows(const Mat& img, unordered_map<double, vector<Rect>> &windows, 
									 const Rect &minWin, int sampleStep = -1);

	// 对于给定的图像和缩放的比例，产生给定scale的候选框，返回在windows中
	static void makeCandidateWindowsWithScale(const Mat &img, vector<Rect> &windows, 
											  const Rect &minWin, const double scale, int sampleStep = -1);

	// 对于给定的图像，产生候选框，并且将对应的图像中的区域写入到windows中，返回在windows中
	static void makeCandidateWindowsROIMat(const Mat& img, unordered_map<double, vector<Mat>> &windows, 
									 const Rect &minWin, int sampleStep = -1);
	
	// 对于给定的图像和缩放的比例，产生给定scale的候选框，返回在windows中
	static void makeCandidateWindowsWithScaleROIMat(const Mat &img, vector<Mat> &windows, 
											  const Rect &minWin, const double scale, int sampleStep = -1);

	// 对于给定的正样本框，产生同正样本重合度为指定比例的候选框
	// 对于产生的新的样本单独存放在genPosWindows中
	static void makePositiveWindows(const vector<Rect> &posWindows, vector<Rect> &genPosWindows, 
									const Rect &maxRect, const double overlapRatio);
};