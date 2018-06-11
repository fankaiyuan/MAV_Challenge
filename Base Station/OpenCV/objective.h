#pragma once
#include <string>
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <stdio.h>
#include <iostream>
#include <unistd.h>



using namespace cv;
using namespace std;

class objective{
public:
	objective(void);
	~objective(void);
	
	objective(string name);
	
	int getxPos();
	void setxPos(int x);
	
	int getyPos();
	void setyPos(int y);
	
	Scalar getHSVmin();
	Scalar getHSVmax();
	
	void setHSVmin(Scalar min);
	void setHSVmax(Scalar max);
		
	string getType(){return type;}
	void setType(string t){type = t;}
	Scalar getColor(){return Color;}
	void setColor(Scalar c){
		Color = c;
	}
	
	
private:
	
	int xPos, yPos;
	string type;
	Scalar HSVmin, HSVmax;
	Scalar Color;
};