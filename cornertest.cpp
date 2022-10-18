#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
//https://blog.csdn.net/LuohenYJ/article/details/104697062
int CHECKERBOARD[2]{6,9};

int main(){
    //创建矢量以储存每个棋盘图像的三维点矢量。
    std::vector<std::vector<Point3f>> objpoints;

    //创建矢量以存储每个棋盘格图像的二维点矢量。
    std::vector<std::vector<cv::Point2f>> imgpoints;

    //为三维点定义世界坐标系(这个目前还不是很理解)
    std::vector<cv::Point3f> objp;
    for(int i=0;i<CHECKERBOARD[1];i++){
        for(int j=0;j<CHECKERBOARD[0];j++){
            objp.push_back(Point3f(j,i,0));
        }
    }

    //提取储存在给定目录中的单个图像的路径
    std::vector<cv::String> images;
    
    //包含棋盘图像的文件夹的路径
    std::string path = "./grids/*.png";

    //使用glob函数读取所有图像的路径(感觉这个函数很好用，以后可以多用)
    cv::glob(path,images);

    cv::Mat frame,gray;

    // 存储检测到的棋盘转角像素坐标的矢量
    std::vector<Point2f> corner_pts;
    bool success;

    //循环读取图像
    for(int i=0;i<images.size();i++){
        frame = cv::imread(images[i]);
        if(frame.empty()){//这张图像有问题
            continue;
        }
        cout << "the current image is "<< i << "th" << endl;
        //将原图转换为对应的灰度图像。
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    }
    //findCirclesGrid可以实现对于圆的圆心的提取,而findChessboardCorners可以用于棋盘角点的识别。
    success= cv::findChessboardCorners(gray, cv::Size(CHECKERBOARD[0],CHECKERBOARD[1]),corner_pts,
                 CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FAST_CHECK | CALIB_CB_NORMALIZE_IMAGE);

    if(success){

    }
    return 0;
}