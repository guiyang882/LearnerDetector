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
		if(info.size() < 7) {
			continue;
		}
		ss.clear();
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

/*
主要是根据给定的的图像尺寸，将产生不同的等级的候选窗口
 */
void CandidateWindow::makeCandidateWindows(const Mat& img, unordered_map<double, vector<Rect>> &windows, const Rect &minWin, int sampleStep) {
	const vector<double> scales = {1.0, 1.5, 1.75, 2.0};
	int numThreads = getNumThreads();
#pragma omp parallel for num_threads(numThreads)	
	for(int i=0;i<scales.size();i++) {
		const double scale = scales[i];
		vector<Rect> tmpRes;
		makeCandidateWindowsWithScale(img, tmpRes, minWin, scale, sampleStep);

#pragma omp critical
		windows[scale] = tmpRes;
	}
}

void CandidateWindow::makeCandidateWindowsWithScale(const Mat &img, vector<Rect> &windows, 
													const Rect &minWin, const double scale, int sampleStep) {
	const int width = img.size().width;
	const int height = img.size().height;
	const int minWidth = minWin.width * scale;
	const int minHeight = minWin.height * scale;
#ifdef OUTPUT_STD
	cout << "width: " << width << ", height: " << height << ", minWidth:" << minWidth << ", minHeight: " << minHeight << endl;
#endif
	int numThreads = getNumThreads();
    if(sampleStep == -1) {
        sampleStep = min(minHeight/2, minWidth/2);
    }
#pragma omp parallel for num_threads(numThreads)
	for(int r=0;r<=height-minHeight;r+=sampleStep) {
		vector<Rect> tmpRes;
		for(int c=0;c<=width-minWidth;c+=sampleStep) {
			int cWidth = min(minWidth, width-c);
			int cHeight = min(minHeight, height-c);
            if(cWidth == minWidth && cHeight == minHeight) {
                Rect cW(r, c, cWidth, cHeight);
                tmpRes.push_back(cW);
            }
		}

#pragma omp critical
		copy(tmpRes.begin(), tmpRes.end(), back_inserter(windows));
	}
}

void CandidateWindow::makePositiveWindows(const vector<Rect> &posWindows, vector<Rect> &genPosWindows, 
										  const Rect &maxRect, const double overlapRatio) {
	const int maxWidth = maxRect.size().width;
	const int maxHeight = maxRect.size().height;
	const double ratio = sqrt(overlapRatio);
	int numThreads = getNumThreads();

#pragma omp parallel for num_threads(numThreads)
	for(int i=0;i<posWindows.size();i++) {
		const int r0 = posWindows[i].tl().x;
		const int c0 = posWindows[i].tl().y;
		const int width = posWindows[i].width;
		const int height = posWindows[i].height;
		const int r_low = max(0, int(r0 - (1 - ratio) * height));
		const int r_up = r0 + (1 - ratio) * height;
		const int c_low = max(0, int(c0 - (1 - ratio) * width));
		const int c_up = c0 + (1 - ratio) * height;

		vector<Rect> tmpRes;
		for(int r=r_low; r<=r_up; r++) {
			for(int c=c_low; c<=c_up; c++) {
				Rect cW(r, c, min(width, maxWidth-c), min(height, maxHeight-r));
				tmpRes.push_back(cW);
			}
		}

#pragma omp critical
		copy(tmpRes.begin(), tmpRes.end(), back_inserter(genPosWindows));
	}
}

void CandidateWindow::makeCandidateWindowsROIMat(const Mat& img, unordered_map<double, vector<Mat>> &windows, 
								 const Rect &minWin, int sampleStep) {
	const vector<double> scales = {1.0, 1.5, 1.75, 2.0};
	int numThreads = getNumThreads();
#pragma omp parallel for num_threads(numThreads)	
	for(int i=0;i<scales.size();i++) {
		const double scale = scales[i];
		vector<Mat> tmpRes;
		makeCandidateWindowsWithScaleROIMat(img, tmpRes, minWin, scale, sampleStep);

#pragma omp critical
		windows[scale] = tmpRes;
	}
}

void CandidateWindow::makeCandidateWindowsWithScaleROIMat(const Mat &img, vector<Mat> &windows, 
										  const Rect &minWin, const double scale, int sampleStep) {
	const int width = img.size().width;
	const int height = img.size().height;
	const int minWidth = minWin.width * scale;
	const int minHeight = minWin.height * scale;
#ifdef OUTPUT_STD
	cout << "width: " << width << ", height: " << height << ", minWidth:" << minWidth << ", minHeight: " << minHeight << endl;
#endif
	int numThreads = getNumThreads();
    if(sampleStep == -1) {
        sampleStep = min(minHeight/2, minWidth/2);
    }
#pragma omp parallel for num_threads(numThreads)
	for(int r=0;r<=height-minHeight;r+=sampleStep) {
		vector<Mat> tmpRes;
		for(int c=0;c<=width-minWidth;c+=sampleStep) {
			int cWidth = min(minWidth, width-c);
			int cHeight = min(minHeight, height-c);
            if(cWidth == minWidth && cHeight == minHeight) {
                Rect cW(r, c, cWidth, cHeight);
                tmpRes.push_back(img(cW));
            }
		}

#pragma omp critical
		copy(tmpRes.begin(), tmpRes.end(), back_inserter(windows));
	}
}



