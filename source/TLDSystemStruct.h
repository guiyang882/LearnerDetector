//
//  systemStruct.h
//  MedianFlow

#ifndef MedianFlow_systemStruct_h
#define MedianFlow_systemStruct_h

#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

typedef float TYPE_OF_COORD;
typedef TYPE_OF_COORD TYPE_MF_COORD;

typedef Point_<TYPE_OF_COORD> TYPE_OF_PT;
typedef TYPE_OF_PT TYPE_MF_PT;
typedef Rect_<TYPE_MF_COORD> TYPE_MF_BB;

typedef pair<Mat, char> TYPE_TRAIN_DATA;
typedef vector<TYPE_TRAIN_DATA> TYPE_TRAIN_DATA_SET;

typedef pair<Point2f, Point2f> TYPE_FERN_LEAF; // save pixel comparision
typedef vector<vector<TYPE_FERN_LEAF> > TYPE_FERN_FERNS; // save all ferns

static const TYPE_OF_PT PT_ERROR = TYPE_OF_PT(-1, -1);
static const TYPE_MF_BB BB_ERROR = TYPE_MF_BB(PT_ERROR, PT_ERROR);

typedef Rect TYPE_BBOX;

class TYPE_DETECTOR_SCANBB : public TYPE_BBOX {
public:
    float overlap; //overlap
    char status;
    float var;
    float posterior;
    float Sp, Sn, Sr, Sc;
    int maxSPIdx, scaleId;
    
    TYPE_DETECTOR_SCANBB():
        TYPE_BBOX(TYPE_BBOX(-1, -1, 0, 0)), overlap(-1), status(-1), posterior(-1), Sp(-1), Sn(-1), Sr(-1), Sc(-1)
    {
    }

    
    TYPE_DETECTOR_SCANBB(const TYPE_BBOX &_bb):
        TYPE_BBOX(_bb), overlap(-1), status(-1), posterior(-1), Sp(-1), Sn(-1), Sr(-1), Sc(-1)
    {
    }
    
    static bool cmpOL(const TYPE_DETECTOR_SCANBB &a, const TYPE_DETECTOR_SCANBB &b) {
        return a.overlap > b.overlap;
    }
    
    static bool cmpP(const TYPE_DETECTOR_SCANBB &a, const TYPE_DETECTOR_SCANBB &b) {
        return a.posterior > b.posterior;
    }
};
typedef vector<TYPE_DETECTOR_SCANBB> TYPE_DETECTOR_SCANBBS;
typedef vector<TYPE_DETECTOR_SCANBB> TYPE_DETECTOR_RET;

static const bool OF_USE_OPENCV = 1;

static const int MF_HALF_PATCH_SIZE = 4; // NNC patch size
static const int MF_NPTS = 12; // number of points in the patch(both vertical and horizontal)
static const int MF_FB_ERROR_DIST = 10; // threshold of detecting confusing condition

static const int MF_TRACK_SUCCESS = 0;
static const int MF_TRACK_F_PTS = -1; // number of points after filtering is too little
static const int MF_TRACK_F_BOX = -2; // result box is out of bounds
static const int MF_TRACK_F_CONFUSION = -3; // tracking result is disordered
static const int MF_TRACK_F_BOX_SMALL = -4; // input box is too small

static const int MF_REJECT_OFERROR = 1 << 0; // filtered by OF error
static const int MF_REJECT_NCC = 1 << 1; // filtered by NCC
static const int MF_REJECT_FB = 1 << 2; // filtered by Forward-Backward

static const int TLD_TRACK_SUCCESS = 1;
static const int TLD_TRACK_FAILED = 0;

static const Scalar COLOR_GREEN = Scalar(156, 188, 26);
static const Scalar COLOR_BLUE = Scalar(219, 152, 52);
static const Scalar COLOR_BLACK = Scalar(94, 73, 52);
static const Scalar COLOR_WHITE = Scalar(241, 240, 236);
static const Scalar COLOR_YELLOW = Scalar(15, 196, 241);
static const Scalar COLOR_RED = Scalar(60, 76, 231);
static const Scalar COLOR_PURPLE = Scalar(182, 89, 155);

static const float BADBB_OL= 0.2;
static const float GOODBB_OL = 0.6;

static const int LEARNER_N_WARPED = 10; // number of warped images to generate
static const int LEARNER_N_GOOD_BB = 10; // number of bounding boxes which are teated as good bounding boxes
                                         // and will be sent to update detector

static const int LEARNER_N_NN_NEG = 100;

static const float VAR_FACTOR = 0.5;

static const float RF_TH_POS = 0.5; // if avgP(1 | x) >= FERN_TH_POS then x will be classified as Positive
static const float RF_TH_NEG = 0.5;

static const int NN_MODEL_SIZE = 500; // size of POS model and NEG model
static const int NN_PATCH_SIZE = 15; // all input image will be resize to (NN_PATCH_SIZE * NN_PATCH_SIZE)
static const float NN_TH_POS = 0.65; // if Sr(patch) >= NN_TH_POS then  patch will be classified as Positive
static const float NN_MARGIN = 0.1; // if Sr(patch) - NN_TH_POS  < NN_MARGIN then it will be used for update
                                    // i.e. patch is not similar as samples in current model

static const bool CLASS_POS = true;
static const bool CLASS_NEG = false;
static const char CLASS_TEST_NEG = 2;

static const int DETECTOR_NFERNS = 10;
static const int DETECTOR_NLEAVES = 13;
static const int DETECTOR_MIN_BB_SIZE = 15; // minimum scanning bounding box size

// scales of scanning grid will be GRID_SCALE_FAC ^ [-HALF_NUM , ... , 0 , ... , HALF_NUM]
static const float DETECTOR_GRID_SCALE_FAC = 1.2f;
static const float DETECTOR_GRID_SCALE_HALF_NUM = 10;
// for each scale of scanning grid, calculate width and height of it, and then x(next) = x + STEP_FAC * min(grid_width, grid_height), y(next) similarly
static const float DETECTOR_GRID_STEP_FAC = 0.1f;

static const float DETECTOR_WARP_NOISE = 5;
static const bool DETECTOR_WARP_BLUR = true;
static const float DETECTOR_WARP_SCALE = 0.02;
static const float DETECTOR_WARP_ANGLE = 20. / 180 * CV_PI;

// P-expert生成正样本时进行的warp参数
static const float DETECTOR_UPDATE_WARP_NOISE = 5;
static const bool DETECTOR_UPDATE_WARP_BLUR = true;
static const float DETECTOR_UPDATE_WARP_SCALE = 0.02;
static const float DETECTOR_UPDATE_WARP_ANGLE = 10. / 180 * CV_PI;

static const int DETECTOR_N_GOOD_BB = 10;
static const int DETECTOR_N_WARPED = 20;

// number of initial NN examples
static const int DETECTOR_N_INIT_NN_NEG = 100;
// size of initial NN test set
static const int DETECTOR_N_INIT_NNT_NEG = 100;

// max number of bounding boxes that can enter into NN classifier
static const int DETECTOR_MAX_ENTER_NN = 100;

static const char DETECTOR_ACCEPTED = 1;
static const char DETECTOR_REJECT_VAR = 2;
static const char DETECTOR_REJECT_RF = 3;
static const char DETECTOR_REJECT_NN = 4;

static const bool NCC_USE_OPENCV = 0; // 1(lower speed): use matchTemplate(), 0(faster)
static const bool NCC_FAST = 1; // 1 : my own implementation
static const bool RND_SHUFFLE_STD = 1;
static const bool QUIET_MODE = 0;
static const bool SHOW_NEW_NN_SAMPLES = 1;

static const float RF_FEA_SHIFT = 1.f / 5;
static const float RF_FEA_OFF = RF_FEA_SHIFT;

//物体跟踪的算法类型
enum TRACK_TYPE {
    MEDIANFLOW,
    CAMSHIFT,
    SIFT_KALMAN,
    VIBE_TRACKING
};

//使用摄像头进行视频跟踪时的图像尺寸的参数设置
const int CAM_CAP_IMG_WIDTH = 480;
const int CAM_CAP_IMG_HEIGHT = 320;

#endif
