#ifndef THINNING_H
#define THINNING_H

#include "imageviewer.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void thinningGuoHallIteration(cv::Mat&, int);
void thinningGuoHall(cv::Mat&);
void execThinningGuoHall(cv::Mat&, cv::Mat&);

void thinningIterationZhang(cv::Mat&, int);
void thinningZhang(cv::Mat&);
void execThinningZhang(cv::Mat&,cv::Mat&);

#endif // THINNING_H
