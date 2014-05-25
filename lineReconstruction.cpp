
#include <iostream>
#include "lineReconstruction.h"
#include <math.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;


int pos[20000][6];

void execLineReconstruction(cv::Mat& src, cv::Mat& dst, int h)
{
    //Mat dst2 = imread("c:/img/result.png", CV_LOAD_IMAGE_COLOR);
    //int h = 10;
    //Mat src = imread("c:/img/result.png", CV_LOAD_IMAGE_GRAYSCALE);
    Mat src2 = src.clone();
    Mat src3 = src.clone();
    Mat src4 = src.clone();
    Mat src1 = src.clone();
    dst = src.clone();


        int nPoints = 0;

        for(int j=1;j<(src.rows);j++)
        {
            for (int i=1;i<(src.cols);i++)
            {

               uchar pk =  src1.at<uchar>(j,i);
               uchar p =  src1.at<uchar>(j,i) == 255   ? 0:1;
               //cout<<(int)pk<<"+-"<<i<<endl;
               if(pk==0)
               {

                    uchar p1 = src1.at<uchar>(j-1,i)== 255   ? 0:1;
                    uchar p2 = src1.at<uchar>(j-1,i+1)== 255   ? 0:1;
                    uchar p3 = src1.at<uchar>(j,i+1)== 255   ? 0:1;
                    uchar p4 = src1.at<uchar>(j+1,i+1)== 255   ? 0:1;
                    uchar p5 = src1.at<uchar>(j+1,i)== 255   ? 0:1;
                    uchar p6 = src1.at<uchar>(j+1,i-1)== 255   ? 0:1;
                    uchar p7 = src1.at<uchar>(j,i-1)== 255   ? 0:1;
                    uchar p8 = src1.at<uchar>(j-1,i-1)== 255   ? 0:1;

                    int val = getCode(p1,p2,p3,p4,p5,p6,p7,p8);
                   // cout<<val<<endl;
                    if(isEndPoint(val))
                    {
                        //circle(src12,Point(i,j),0,CV_RGB(0,255,0),1);
                        pos[nPoints][0] = j;
                        pos[nPoints][1] = i;
                        pos[nPoints][2] = val;
                        pos[nPoints][3] = 0;
                        pos[nPoints][4] = 0;
                        pos[nPoints][5] = 0;
                        //cout<<"no"<<endl;
                        nPoints++;
                    }

               }


            }
        }

        for(int k=0; k<nPoints; k++)
        {
            int y0 = pos[k][0];
            int x0 = pos[k][1];
            int dir = pos[k][2];
            int flag = pos[k][3];
            int val = pos[k][2];
            int a = val;
            stringstream ss;
            ss << a;
            string str = ss.str();

            if(flag != 1)
            {


                pos[k][3] = 1;
                Point p = nextPoint(x0, y0, 20, src2);
                for(int m=0; m<nPoints; m++)
                {
                    int y1 = pos[m][0];
                    int x1 = pos[m][1];
                    bool a1 = pos[m][3]!= 1;
                    bool a2 = (y1 >= (y0-h) && y1 <= (y0+h));
                    bool a3 = (x1 >= (x0-h) && x1 <= (x0+h));
                    bool a4 = x1 != p.x && y1 != p.y;
                    if( a1 && a2  && a3 && a4)
                    {
                        pos[m][3] = 1;
                        pos[m][4] = y0;
                        pos[m][5] = x0;
                        int a1 = pos[m][2];
                        stringstream ss;
                        ss << a1;
                        string str2 = ss.str();

                    }
                }
            }


        }


        for(int k=0; k<nPoints; k++)
        {
            int y0 = pos[k][0];
            int x0 = pos[k][1];
            int flag = pos[k][3];
            double v1 = dirEndPoint(x0, y0, 10, src3);
            pos[k][2] = (int ) changeDir(round(v1));
            int dir = pos[k][2];
            int xd = -1;
            int yd = -1;
            double max = INFINITY;
            for(int m=0; m<nPoints; m++)
            {
                int yi = pos[m][0];
                int xi = pos[m][1];
                int flag = pos[m][3];
                double vi = dirEndPoint(xi, yi, 10, src4);
                pos[m][2] = (int ) changeDir(round(vi));
                int y1 = pos[m][4];
                int x1 = pos[m][5];
                int dir1 = pos[m][2];
                if(y1==y0 && x1==x0 && (yi != y0) && (xi != x0))
                {
                    if(!deletePoints(dir, dir1))
                    {
                        pos[m][2] = 0;

                    }
                    if(abs(y0-y1)< max)
                    {
                        max = abs(y0-y1);
                        yd = yi;
                        xd = xi;

                    }


                }
            }

            if(xd != -1 && yd != -1)
            {
                line(dst,Point(x0,y0),Point(xd,yd),CV_RGB(0,255,0),1,1);
            }





        }


         imwrite("c:/img/result2.png", dst);




}

bool isEndPoint(int x)
{
    return x==128 || x==2 || x==8 || x==32 || x==1 || x == 4|| x==16 || x== 32 || x==64;
}

int getCode(uchar p1, uchar p2, uchar p3, uchar p4, uchar p5, uchar p6, uchar p7, uchar p8)
{
    return (p8*64)+(p1*128)+(p2*1)+(p3*2)+ (p4*4) + (p5*8)+ (p6*16)+ (p7*32);
}

Point nextPoint(int x, int y, int limit, Mat dst)
{

    for(int i=0; i<limit; i++)
    {
       int val = getDir(x,y, dst);
       int a = i;
       stringstream ss;
       ss << a;
       string str = ss.str();
       dst.at<uchar>(y,x) = 255;
       int x0 = x;
       int y0 = y;

       switch(val)
       {
           case 1:
               y = y-1;
               x = x+1;
           break;
           case 2:
               x = x+1;
           break;
           case 4:
               y = y+1;
               x = x+1;
           break;
           case 8:
               y=y+1;
           break;
           case 16:
               y=y+1;
               x=x-1;
           break;
           case 32:
               x= x-1;
           break;
           case 64:
               y = y-1;
               x = x-1;
           break;
           case 128:
               y = y - 1;
           break;
       }
       dst.at<uchar>(y0,x0) = 0;
       int dir =getDir(x,y, dst);
       if(isEndPoint(dir))
       {

           return Point(x,y);
       }
       dst.at<uchar>(y0,x0) = 255;

    }

    return Point(-1,-1);
}

double dirEndPoint(int x, int y, int limit, Mat dst)
{
    double dir = 0.0;
    int cont = 0;
    for(int i=0; i<limit; i++)
    {
        if(dst.at<uchar>(y,x) == 0)
        {
            int val = getDir(x,y, dst);
            cout<<val<<endl;
            dir += getChainDir(val);
            int a = i;
            stringstream ss;
            ss << a;
            string str = ss.str();
            dst.at<uchar>(y,x) = 255;
            int x0 = x;
            int y0 = y;

            switch(val)
            {
                case 1:
                    y = y-1;
                    x = x+1;
                break;
                case 2:
                    x = x+1;
                break;
                case 4:
                    y = y+1;
                    x = x+1;
                break;
                case 8:
                    y=y+1;
                break;
                case 16:
                    y=y+1;
                    x=x-1;
                break;
                case 32:
                    x= x-1;
                break;
                case 64:
                    y = y-1;
                    x = x-1;
                break;
                case 128:
                    y = y - 1;
                break;
            }

            cont++;
        }


    }

        if(dir!= 0)
        {
            return dir/cont;
        }
        return 0;
}

int getDir(int x, int y, Mat dst)
{
       uchar p1 = dst.at<uchar>(y-1,x)== 255   ? 0:1;


       uchar p2 = dst.at<uchar>(y-1,x+1)== 255   ? 0:1;

       uchar p3 = dst.at<uchar>(y,x+1)== 255   ? 0:1;


       uchar p4 = dst.at<uchar>(y+1,x+1)== 255   ? 0:1;

       uchar p5 = dst.at<uchar>(y+1,x)== 255   ? 0:1;

       uchar p6 = dst.at<uchar>(y+1,x-1)== 255   ? 0:1;

       uchar p7 = dst.at<uchar>(y,x-1)== 255   ? 0:1;

       uchar p8 = dst.at<uchar>(y-1,x-1)== 255   ? 0:1;

       int val = getCode(p1,p2,p3,p4,p5,p6,p7,p8);

       return val;
}

int getChainDir(int dir)
{
    switch(dir)
    {
        case 1:
            return 1;
        case 2:
            return 2;
        case 4:
            return 3;
        case 8:
          return 4;

        case 16:
            return 5;

        case 32:
            return 6;
        case 64:
            return 7;
        case 128:
            return 0;
    }
}

int changeDir(int val)
{
    switch(val)
    {
        case 0:
            return 7;
        case 6:
            return 5;
        case 4:
            return 3;
        case 2:
            return 1;
        default:
            return val;
    }

}

bool deletePoints(int d1, int d2)
{
    switch(d1)
    {
        case 1:
            if(d2 == 5 || d2 == 3 || d2 == 7)
            {
                return true;
            }
        break;
        case 3:
            if(d2 == 7 || d2 == 5 || d2 == 1)
            {
                return true;
            }
        break;
        case 5:
            if(d2 == 7 || d2 == 3 || d2 == 1)
            {
                return true;
            }
        break;
        case 7:
            if(d2 == 5 || d2 == 3 || d2 == 1)
            {
                return true;
            }
        break;


    }

    return false;
}
