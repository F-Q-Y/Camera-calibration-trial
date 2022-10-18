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

        //findCirclesGrid可以实现对于圆的圆心的提取,而findChessboardCorners可以用于棋盘角点的识别。
        success= cv::findChessboardCorners(gray, cv::Size(CHECKERBOARD[0],CHECKERBOARD[1]),corner_pts,
                        CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FAST_CHECK | CALIB_CB_NORMALIZE_IMAGE);

        cout << success << endl;
        //如果检测到了所有的棋盘上的角点，我们将细化像素坐标并将其显示在棋盘图像上
        if(success){
            cout << "success" << endl;
            //这个类是用来定义迭代算法终止标准的类。
            cv::TermCriteria criteria(TermCriteria::EPS | TermCriteria::Type::MAX_ITER, 30, 0.001);

            //为给定的二维点细化像素坐标，不断的迭代，只到次数大于30或者是精读变化小于0.001
            cv::cornerSubPix(gray, corner_pts, cv::Size(11,11),cv::Size(-1,-1),criteria);

            //在棋盘上显示检测到的角点
            cv::drawChessboardCorners(frame, cv::Size(CHECKERBOARD[0],CHECKERBOARD[1]), corner_pts, success);

            // frame.show();
            cv::imshow("名字",frame);
            cv::imwrite("./ans/"+to_string(i)+".jpg",frame);
            system("wait");
            cv::waitKey(1);
            //目前我对objp的理解不是很充分。
            objpoints.push_back(objp);
            imgpoints.push_back(corner_pts);
        }
    }

    cv::destroyAllWindows();

    cv::Mat cameraMatrix, distCoeffs, R, T;
    cv::calibrateCamera(objpoints, imgpoints, cv::Size(gray.rows,gray.cols), cameraMatrix, distCoeffs, R, T);

    // //内参矩阵
    std::cout << "cameraMatrix : " << cameraMatrix << std::endl;

    // //透镜畸变畸变系数
    std::cout << "distCoeffs : " << distCoeffs << std::endl;

    // //R
    std::cout << "Rotation vector : " << R << std::endl;

    // //T
    std::cout << "Translation vector : "<< T << std::endl;

    return 0;
}