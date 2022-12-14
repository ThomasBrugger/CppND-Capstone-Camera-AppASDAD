#ifndef CAMERA_H_
#define CAMERA_H_

#include <string>
#include <thread>
#include <mutex>

#include "App.h"

//Forward Declaration
class MyFrame;

	class Camera {

	public:
		Camera(MyFrame* myFrame);
		
		bool isCameraOn();
		bool isCameraClosed();
		bool isSnapping();

		
		void startCameraThread();
		void openCamera();
		void closeCamera();
		void snap(bool input);

		std::string getWindowName();
		void setWindowName(std::string s);

	private:
		std::string m_sWindowName;
		bool m_bSnap{ false };
		bool m_bCameraClosed{ false };
		bool m_bIsCameraOn{ false };

		std::thread cameraThread;
		MyFrame* m_pMyFrame;
		std::mutex mtxSnap;
		std::mutex mtxClosed;

	};

#endif /* CAMERA_H_ */