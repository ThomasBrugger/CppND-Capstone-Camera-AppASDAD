#include"Imageprocessor.h"   
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include"opencv2/objdetect/objdetect.hpp"
#include<string>
#include<vector>
#include<iostream>

std::vector<FacePosition> Imageprocessor::detectFace(std::string fileName, bool showWnd){
   cv::Mat image_with_humanface;
   image_with_humanface = cv::imread(fileName);
   std::string trained_classifier_location = "C:/opencv/minGW/install/etc/haarcascades/haarcascade_frontalface_alt.xml";
   cv::CascadeClassifier faceDetector;
   faceDetector.load(trained_classifier_location);
   std::vector<cv::Rect>faces;
   std::vector<cv::Rect>boundary;
   std::vector<FacePosition> FacePosContainer;
   faceDetector.detectMultiScale(image_with_humanface, faces, 1.1, 4, cv::CASCADE_SCALE_IMAGE, cv::Size(20, 20));
   for (size_t i = 0; i < faces.size(); i++){ 
      FacePosition facePos;
      cv::Mat faceROI = image_with_humanface(faces[i]);
      facePos.x = faces[i].x;
      facePos.y= faces[i].y;
      int point_w  = facePos.y + faces[i].height;
      int point_h = facePos.x + faces[i].width;
      facePos.height = faces[i].height;
      facePos.width = faces[i].width; 
      FacePosContainer.push_back(facePos);
      cv::rectangle(image_with_humanface, cv::Point(facePos.x, facePos.y), cv::Point(point_h, point_w), cv::Scalar(255, 0, 255), 2, 8, 0);
   }
   if (showWnd){
    cv::namedWindow("Face Detection");
    cv::imshow("Face Detection", image_with_humanface);
    cv::waitKey(0); //Window will only be closed by "x"
   }
   return FacePosContainer;
}

void Imageprocessor::makeRobotFace(std::string fileName, bool showWnd, bool saveImg){
    std::vector<FacePosition> FacePosContainer = detectFace(fileName,false);

    cv::Mat bigImage = cv::imread(fileName, -1);
    const cv::Mat smallImage = cv::imread("Robot.jpg", -1);
    
    for(FacePosition facePos : FacePosContainer){
        cv::Mat resizedImg;
        cv::resize(smallImage, resizedImg,cv::Size(facePos.height, facePos.width));

        const int x = facePos.x;
        const int y = facePos.y;

        cv::Mat destRoi;
        try {
            destRoi = bigImage(cv::Rect(x, y, resizedImg.cols, resizedImg.rows));
        }   catch (...) {
            std::cerr << "Trying to create roi out of image boundaries" << std::endl;
            return;
        }
        resizedImg.copyTo(destRoi);
    }
    if (showWnd){
        cv::namedWindow("Make Robot Face");
        cv::imshow("Make Robot Face", bigImage);
        cv::waitKey(0); //Window will only be closed by "x"
    }
    if (saveImg){
        cv::imwrite("image.jpg", bigImage);
    }
}