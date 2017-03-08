#include "datareader.h"

InputReader::InputReader(string listpath) {
	m_listpath = listpath;
	if(access(listpath.c_str(), 0) != 0) {
		throw std::runtime_error("InputReader image file list not Found !");
	}
	//read from listfile image lists
	ifstream fin(m_listpath);
	string tmp = "";
	while(fin >> tmp) {
		m_queImgsPath.push(tmp);
	}
	fin.close();
}

InputReader::InputReader(string listpath, string groundpath) {
	if(access(listpath.c_str(), 0) != 0 || access(groundpath.c_str(), 0) != 0) {
		throw std::runtime_error("InputReader image file list not Found !");
	}

	m_listpath = listpath;
	m_groundpath = groundpath;
	ifstream fin(m_listpath);
	string tmp = "";
	while(fin >> tmp) {
		m_queImgsPath.push(tmp);
	}
	fin.close();

	ifstream gin(m_groundpath);
	while(gin >> tmp) {
		m_queGroundPath.push(tmp);
	}
	gin.close();
}

InputReader::~InputReader() {

}

bool InputReader::readNextImage(Mat &img) {
	while(!m_queImgsPath.empty()) {
		string path = m_queImgsPath.front();
		m_queImgsPath.pop();
		img = cv::imread(path);
		return true;
	}
	return false;
}

bool InputReader::readNextImage(Mat &img, vector<Rect> &rectVec) {
	while(!m_queImgsPath.empty()) {
		string imgPath = m_queImgsPath.front();
		m_queImgsPath.pop();
		string rectPath = m_queGroundPath.front();
		m_queGroundPath.pop();
		img = cv::imread(imgPath);
		return InputReader::readGroundFile(rectPath, rectVec);
	}
	return false;
}

bool InputReader::readGroundFile(const string &path, vector<Rect> &vals) {
	if(access(path.c_str(), 0) != 0) {
		throw std::runtime_error("GroundTruth File Not Found " + path);
		return false;
	}
	vals.clear();
	ifstream fin(path);
	string info = "";
	stringstream ss;
	ss.clear();
	while(getline(fin, info)) {
		ss.str(info);
		string a1, a2, a3, a4;
		getline(ss, a1, ',');
		getline(ss, a2, ',');
		getline(ss, a3, ',');
		getline(ss, a4, ',');
		Rect rect(stoi(a1), stoi(a2), stoi(a3), stoi(a4));
		vals.push_back(rect);
	}
	return true;
}
