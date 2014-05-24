#ifndef LINERECONSTRUCTION_H
#define LINERECONSTRUCTION_H


#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>
using namespace cv;
using namespace std;

bool isEndPoint(int);
int getCode(uchar, uchar,uchar,uchar,uchar,uchar,uchar,uchar);
Point nextPoint(int, int , int , Mat);
int getDir(int, int , Mat);
double dirEndPoint(int, int , int, Mat);
int changeDir(int);
bool deletePoints(int, int);
int getChainDir(int);
void execLineReconstruction(cv::Mat&, cv::Mat&, int);
#endif // LINERECONSTRUCTION_H
