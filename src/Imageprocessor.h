#ifndef IMAGEPROCESSOR_H_
#define IMAGEPROCESSOR_H_

#include <string>
#include <vector>

struct FacePosition{
  int x; 
  int y;
  int height;
  int width;
};

class Imageprocessor {

	public:
        std::vector<FacePosition> detectFace(std::string fileName, bool showWnd);
        void makeRobotFace(std::string fileName, bool showWnd, bool saveImg);
	private:

	};

#endif /* IMAGEPROCESSOR_H_ */