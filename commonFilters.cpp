#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void applyGaussianBlur(cv::Mat& src , cv::Mat& dst, int size, int desvX, int desvY)
{

    //smooth the image using Gaussian kernel in the "src" and save it to "dst"
    if(size>0)
    {
        GaussianBlur( src, dst, cv::Size( size, size ), 0, 0 );
    }
    else
    {
        dst = src.clone();
    }

}
