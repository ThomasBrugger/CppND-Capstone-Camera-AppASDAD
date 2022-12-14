#include "opencv2\opencv.hpp"
#include "opencv2\highgui\highgui_c.h"
#include "Camera.h"
#include <thread>
#include <mutex>


Camera::Camera(MyFrame* myFrame) : m_pMyFrame(myFrame) {};
void Camera::startCameraThread(){
	cameraThread = std::thread{&Camera::openCamera, this};
}

void Camera::openCamera(){
	cv::Mat image;
	m_bCameraClosed = false;
	m_bIsCameraOn = true;

	m_sWindowName = "Image";
	cv::namedWindow(m_sWindowName);
	
	cv::VideoCapture cap(0);

	if (!cap.isOpened()) {
		std::cout << "cannot open camera";
	}

	//Maybe a callback Function or Event would be a better choice
	m_pMyFrame -> setButtonLayout();
	while (!isCameraClosed()) {
		cap >> image;
		if (isSnapping()) {
			//In the future it should passed by pointer to the app, not saved as image
			cv::imwrite("image.jpg", image);
			//Maybe a callback Function or Event would be a better choice
			//m_pMyFrame->ImageReady();
			snap(false);
		}
		else {
			imshow(m_sWindowName, image);
		}
		cv::waitKey(25);
	}
	cv::destroyWindow(m_sWindowName);
	m_pMyFrame->setButtonLayout();
	m_bIsCameraOn = false;
}

bool Camera::isCameraOn() {
	return m_bIsCameraOn;
}

bool Camera::isCameraClosed(){
	std::scoped_lock<std::mutex> lock(mtxClosed);
	return m_bCameraClosed;
}

bool Camera::isSnapping(){
	std::scoped_lock<std::mutex> lock(mtxSnap);
	std::cout << "Camera is snapping:" << m_bSnap << std::endl;
	return m_bSnap;
}
void Camera::closeCamera() {
	std::scoped_lock<std::mutex> lock(mtxClosed);
	m_bCameraClosed = true;
	cameraThread.join();
}

void Camera::snap(bool input) {
	std::scoped_lock<std::mutex> lock(mtxSnap);
	std::cout << "Camera::snap:" << m_bSnap << std::endl;
	m_bSnap = input;
}

std::string Camera::getWindowName() {
	return m_sWindowName;
}

void Camera::setWindowName(std::string s) {
	m_sWindowName = s;
}
