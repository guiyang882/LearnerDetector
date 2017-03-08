#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <queue>

#include <opencv2/opencv.hpp>
#include <unistd.h>

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