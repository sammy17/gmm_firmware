//
// Created by dilin on 10/23/17.
//

#ifndef TRACK_BGSDETECTOR_H
#define TRACK_BGSDETECTOR_H

#include <fstream>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "Detector.h"

#define BGS_DEBUG_MODE

using namespace std;
using namespace cv;

class Blob
{
public:
    // member variables ///////////////////////////////////////////////////////////////////////////
    Blob(std::vector<cv::Point> _contour);
    std::vector<cv::Point> currentContour;

    cv::Rect currentBoundingRect;

    std::vector<cv::Point> centerPositions;

    double dblCurrentDiagonalSize;
    double dblCurrentAspectRatio;

    bool blnCurrentMatchFoundOrNewBlob;

    bool blnStillBeingTracked;

    int intNumOfConsecutiveFramesWithoutAMatch;

    cv::Point predictedNextPosition;
};

enum
{
    BGS_MOVING_AVERAGE=1,
    BGS_GMM=2
};

struct DetectionRecord
{
    float data[8];
};

class BGSDetector : public Detector
{
public:
    explicit BGSDetector(double TH=30,
                         int method=BGS_MOVING_AVERAGE,
                         bool doGammaCorrection=false,
                         string coeffFilePath="",
                         bool trainingMode=false);
    std::vector<cv::Rect> detect(cv::Mat &img);
    vector<DetectionRecord> data;
    void trainDetector();

private:
    void backgroundSubstraction(cv::Mat &frame0, cv::Mat &frame1, cv::Mat &frame2
            , cv::Mat &bgModel, cv::Mat &mask, double TH=15);
    cv::Mat frames[3];
    cv::Mat bgModel;
    cv::Mat mask;
    uint8_t frameCount;
    double TH;
    void GammaCorrection(cv::Mat& src, cv::Mat& dst, float fGamma);
    int method;
    bool doGamaCorrection;
    string coeffFilePath;
    FileStorage coeffFile;
    bool trainingMode;
    double coeff[64];
    PCA pca;
    float detectorTH=0;
    Mat coeffMat;

};


#endif //TRACK_BGSDETECTOR_H
