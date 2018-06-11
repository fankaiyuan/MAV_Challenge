//Contributions by  Kyle Hounslow 2013

//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software")
//, to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
//and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//IN THE SOFTWARE.

#include <algorithm>    // std::sort
#include <math.h> 
#include <sstream>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <objective.h>
#include <vector>
#include <thread>
extern "C" {
#include <xdo.h>
}
//#include <Windows.h>

int H_MIN = 0, S_MIN = 0, V_MIN = 0; 
int H_MAX = 256, S_MAX = 256, V_MAX = 256;
int BorderH_MIN = 0, BorderS_MIN = 0, BorderV_MIN = 0; 
int BorderH_MAX = 256, BorderS_MAX = 256, BorderV_MAX = 256;
int DelivH_MIN = 0, DelivS_MIN = 0, DelivV_MIN = 0; 
int DelivH_MAX = 256, DelivS_MAX = 256, DelivV_MAX = 256;
int BlH_MIN = 0, BlS_MIN = 0, BlV_MIN = 0; 
int BlH_MAX = 256, BlS_MAX = 256, BlV_MAX = 256;
int MAX = 256;
int Bordertoggle = 0;
int Delivtoggle = 0;
int Blacktoggle = 0;
int Kill = 0;
int Autonomous = 1;
int Cali = 0;
int setter = 1;
bool threadded = true;
vector <objective> vborders;
vector <objective> vlandings;
vector <objective> vpickups;
pthread_mutex_t lock;
//pthread_mutex_t Vector2;
//pthread_mutex_t Vector3;
using namespace std;


using namespace cv;
//initial min and max HSV filter values.
//these will be changed using trackbars
//default capture width and height
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;
//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS=50;
//minimum and maximum object area
const int MIN_OBJECT_AREA = 20*20;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH/1.5;
//names that will appear at the top of each window
const string windowName = "Original Image";
const string windowName1 = "HSV Image";
const string windowName2 = "Thresholded Image";
const string trackbarWindowName = "Trackbars";
void on_trackbar( int, void* )
{//This function gets called whenever a
	// trackbar position is changed
}


string intToString(int number){


	std::stringstream ss;
	ss << number;
	return ss.str();
}


void BorderHSVFunct(int, void*){
		BorderH_MIN = H_MIN;
		BorderH_MAX = H_MAX;
		BorderS_MIN = S_MIN;
		BorderS_MAX = S_MAX;
		BorderV_MIN = V_MIN;
		BorderV_MAX = V_MAX;
		H_MIN = 0;
		H_MAX = 256;
		S_MIN = 0; 
		S_MAX = 256;
		V_MIN = 0; 
		V_MAX = 256;
}

void DelivHSVFunct(int, void*){
		DelivH_MIN = H_MIN;
		DelivH_MAX = H_MAX;
		DelivS_MIN = S_MIN;
		DelivS_MAX = S_MAX;
		DelivV_MIN = V_MIN;
		DelivV_MAX = V_MAX;
		H_MIN = 0;
		H_MAX = 256;
		S_MIN = 0; 
		S_MAX = 256;
		V_MIN = 0; 
		V_MAX = 256;
}
void BlHSVFunct(int, void*){
		BlH_MIN = H_MIN;
		BlH_MAX = H_MAX;
		BlS_MIN = S_MIN;
		BlS_MAX = S_MAX;
		BlV_MIN = V_MIN;
		BlV_MAX = V_MAX;
		H_MIN = 0;
		H_MAX = 256;
		S_MIN = 0; 
		S_MAX = 256;
		V_MIN = 0; 
		V_MAX = 256;
}


void createTrackbars(){
	//create window for trackbars
	namedWindow(trackbarWindowName,0);
	//create memory to store trackbar name on window
	//char TrackbarName[50];
	//sprintf( TrackbarName, "H_MIN", H_MIN);
	//sprintf( TrackbarName, "H_MAX", H_MAX);
	//sprintf( TrackbarName, "S_MIN", S_MIN);
	//sprintf( TrackbarName, "S_MAX", S_MAX);
	//sprintf( TrackbarName, "V_MIN", V_MIN);
	//sprintf( TrackbarName, "V_MAX", V_MAX);
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH), 
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->      
	
	createTrackbar( "H_MIN", trackbarWindowName, &H_MIN, MAX, on_trackbar );
	createTrackbar( "H_MAX", trackbarWindowName, &H_MAX, MAX, on_trackbar );
	createTrackbar( "S_MIN", trackbarWindowName, &S_MIN, MAX, on_trackbar );
	createTrackbar( "S_MAX", trackbarWindowName, &S_MAX, MAX, on_trackbar );
	createTrackbar( "V_MIN", trackbarWindowName, &V_MIN, MAX, on_trackbar );
	createTrackbar( "V_MAX", trackbarWindowName, &V_MAX, MAX, on_trackbar );
	
	
	createTrackbar( "Set Border", trackbarWindowName, &Bordertoggle, setter, BorderHSVFunct);
	createTrackbar( "Set Red Target", trackbarWindowName, &Delivtoggle, setter, DelivHSVFunct);
	createTrackbar( "Set Black Target", trackbarWindowName, &Blacktoggle, setter, BlHSVFunct);
	createTrackbar( "Hide Calibration", trackbarWindowName, &Cali, setter, on_trackbar);
	
}

void drawObject(vector<objective> Targets,Mat &frame){
	for(int i = 0; i < Targets.size(); i++){
		cv::circle(frame,cv::Point(Targets.at(i).getxPos(),Targets.at(i).getyPos()),10,cv::Scalar(0,0,255));
		cv::putText(frame,intToString(Targets.at(i).getxPos())+ " , " + intToString(Targets.at(i).getyPos()),cv::Point(Targets.at(i).getxPos(),Targets.at(i).getyPos()+20),1,1,Scalar(0,255,0));
		cv::putText(frame, Targets.at(i).getType(), cv::Point(Targets.at(i).getxPos(),Targets.at(i).getyPos()-20),1,2,Targets.at(i).getColor());
	}
	
}


void morphOps(Mat &thresh){
	//create structuring element that will be used to "dilate" and "erode" image.
	//the element chosen here is a 3px by 3px rectangle
	Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
	//dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement( MORPH_RECT,Size(8,8));
	erode(thresh,thresh,erodeElement);
	erode(thresh,thresh,erodeElement);
	dilate(thresh,thresh,dilateElement);
	dilate(thresh,thresh,dilateElement);
}


void trackFilteredObject(Mat threshold,Mat HSV, Mat &cameraFeed){
	
	vector <objective> borders;
	Mat temp;
	threshold.copyTo(temp);
	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//find contours of filtered image using openCV findContours function
	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
	//use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();
		//if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
		if(numObjects<MAX_NUM_OBJECTS){
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {

				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;

				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				//we only want the object with the largest area so we safe a reference area each
				//iteration and compare it to the area in the next iteration.
				if(area>MIN_OBJECT_AREA){
					
					objective border;
					
					border.setxPos(moment.m10/area);
					border.setyPos(moment.m01/area);

					borders.push_back(border);

					objectFound = true;

				}else objectFound = false;


			}
			//let user know you found an object
			if(objectFound ==true){
				//draw object location on screen
				drawObject(borders,cameraFeed);}
		}else putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
	}
}


void trackFilteredObject(objective Targets, Mat threshold,Mat HSV, Mat &cameraFeed, vector <objective> &objects){
	
	//vector <objective> objects;
	objects.clear();
	
	Mat temp;
	threshold.copyTo(temp);
	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//find contours of filtered image using openCV findContours function
	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
	//use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();
		//if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
		if(numObjects<MAX_NUM_OBJECTS){
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {

				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;

				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				//we only want the object with the largest area so we safe a reference area each
				//iteration and compare it to the area in the next iteration.
				if(area>MIN_OBJECT_AREA){
					
					objective object;
					
					object.setxPos(moment.m10/area);
					object.setyPos(moment.m01/area);
					object.setType(Targets.getType());
					object.setColor(Targets.getColor());

					objects.push_back(object);

					objectFound = true;

				}else objectFound = false;


			}
			//let user know you found an object
			if(objectFound ==true){
				//draw object location on screen
				drawObject(objects,cameraFeed);}

		}else putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
	}
}


//***********************************************************************************************************************

void Calculations(){
	vector<objective> vYellow;		//Vectors containing our points. Should never be overwritten.
	vector<objective> vRed;
	vector<objective> vBlack;
	
	int x = 0, y = 0, div = 0;		//Various int's used for calculation
	int div2 = 0, iteration =0;		//this set is used interchangably, and may have
	float m = 0, mAvg = 0, Summ = 0;		//different purposes in each section. 
	float m2 = 0, m2Avg = 0, Summ2 = 0;
	
	int forward = 0, backward = 0;   //Flight input trackers
	int right = 0, left = 0;
	int up = 0, down = 0;
	
	vector<float> slopes;		//vectors to accumilate slope calculations
	vector<float> slopes2;
	vector<float> slopesDev;	//Deviations
	vector<float> slopes2Dev;
	vector<bool> LineCounter; 	// This will store 1 for line, 0 for corner
	vector<int> RedXcounter;
	vector<int> RedYcounter;
	vector<int> BlackXcounter;
	vector<int> BlackYcounter;
	vector<int> xLine;			//Used to store x & y values of vYellow for easy sorting
	vector<int> yLine;
	
	
	//Begin Autonomous code: *******************************************************
	
	while(Autonomous){		//While autonomous mode is enabled:
	//sleep(1);
		pthread_mutex_lock(&lock);		//Wait for the main thread to unlock the value
		vYellow = vborders;				//Store our values in manipulatable vectors
		vRed = vpickups;
		vBlack = vlandings;
		pthread_mutex_unlock(&lock);
		
		
		
		//Begin corner calculations.
		if( (vYellow.size()) >= 2){
			for(int i = 0; i < (vYellow.size()); i++){
				for(int k = 0; k < (vYellow.size()); k++){
					if( i != k){           
						x = abs(vYellow.at(i).getxPos() - vYellow.at(k).getxPos());   //x1 - x2
						y = abs(vYellow.at(i).getyPos() - vYellow.at(k).getyPos());   // y1 - y2
					}
					if( x == 0){
						m = 0;				//Dividing by zero
					}else if(y == 0){
						m2=0;				//Dividing by zero again	
					}else{
						m = abs((float)y / (float)x);			//   y/x normal slope
						m2 = abs((float)x / (float)y);			//	x/y inverse slope
					}	
				mAvg = mAvg + m;
				m2Avg = m2Avg + m2;							//Take the average
				div++;										//divisor for later
				}
				mAvg = mAvg / div; 
				slopes.push_back(mAvg);
				m2Avg = m2Avg / div; 						//Here we find the average and store it
				slopes2.push_back(m2Avg);					//We want the average of every point!
				mAvg = 0;
				m2Avg =0;
				div = 0;
			}			
			std::sort (slopes.begin(), slopes.end());		//Sort them. 
			std::sort (slopes2.begin(), slopes2.end());
			for( int r = 0; r < slopes.size(); r++){		//Both are the same size
				if(slopes.at(r) < 25){						//Eliminate extremes. 
					mAvg = mAvg + slopes.at(r);				//Find the average slope
					div++;
				}
				if(slopes2.at(r) < 25){						//Eliminate extremes.
					m2Avg = m2Avg + slopes2.at(r);
					div2++;
				}
			}
			mAvg = (mAvg / div);
			m2Avg = (m2Avg / div2);							//These are the average slopes. 
			div = 0;
			div2 = 0;
			for( int r = 0; r < slopes.size(); r++){			//Calculating standard deviation
				slopesDev.push_back(mAvg - slopes.at(r));
				slopesDev.at(r) = slopesDev.at(r) * slopesDev.at(r);
				Summ = Summ + slopesDev.at(r);
			}
			for( int r = 0; r < slopes2.size(); r++){				//Again for inverse
				slopes2Dev.push_back(m2Avg - slopes2.at(r));
				slopes2Dev.at(r) = slopes2Dev.at(r) * slopes2Dev.at(r);
				Summ2 = Summ2 + slopes2Dev.at(r);
			}
			Summ = Summ / (slopesDev.size());						//Last step	
			Summ2 = Summ2 / (slopes2Dev.size());
			float StdDev = sqrt (Summ);								//Deviations
			float StdDev2 = sqrt (Summ2);
			
			if(StdDev > (.2) && StdDev2 > (.2)){  //is corner?
				LineCounter.push_back(0);		
			}
			if(StdDev < (.2) || StdDev2 < (.2)){	//is line?
				LineCounter.push_back(1);
			}

			Summ = 0;  			//Clear the used variables for next cycle. 
			mAvg = 0;
			div = 0;
			Summ2 = 0;
			m2Avg = 0;
			div2 = 0;
			slopesDev.clear();
			slopes2Dev.clear();
		
			//End corner detection*************************************************************
			//If line vs corner flight commands
			if(LineCounter.size() >= 150){
				for(int i =0; i < LineCounter.size(); i++){ // This will store 1 for line, 0 for corner
					if(LineCounter.at(i) == 1){
						Summ++;							//reusing for simplicity
					}
				}
				if(Summ >= 50){   //If it's a line, we must determine if the line is straight
					//cout << "line" << endl;
					Summ = 0;
					for(int i = 0; i < (vYellow.size()); i++){
							xLine.push_back(vYellow.at(i).getxPos());
							yLine.push_back(vYellow.at(i).getyPos());
					}
					std::sort(xLine.begin(), xLine.end());
					std::sort(yLine.begin(), yLine.end());

					if(xLine.at(xLine.size()-1)-25 <= xLine.at(0)){		//Is the line vertical?
						//cout << "Vertical" << endl;
						for(int i = 0; i < xLine.size();i++){			//Take the average
								Summ2 = Summ2 + xLine.at(i);			//Max X = 640
						}												//Half X = 320
						mAvg = (Summ2 / (xLine.size()));
						if( 270 <= mAvg && mAvg <= 370){							//If it's somewhere near center
							if(mAvg >= 320){
								left++;										//If it's slightly right, turn left (away).
								forward++;
							}else if(mAvg < 320){
								right++;								//If it's slightly left, turn right (away).
								forward++;
							}
						}else if(mAvg > 370){									//If it's on the far right
							//cout << "Right side" << endl;
							forward++;
						}else if(mAvg < 270){									//If it's on the far left
							//cout << "Left side" << endl;
							forward++;
						}

					}else if(yLine.at(yLine.size()-1)-25 <= yLine.at(0)){	//Is it horizontal?
						//cout << "Horizontal" << endl;
						for(int i = 0; i < yLine.size();i++){
								Summ2 = Summ2 + yLine.at(i);			//Max Y = 480
						}												//Half Y = 240
						mAvg = (Summ2 / (yLine.size()));
						//cout << "Hoz: " << mAvg << endl;
						if( 190 <= mAvg && mAvg <= 290){			//If this line is near the center				
							if(mAvg >= 240){
								left++;								//Behind. Left and forward		
								forward++;
							}else if(mAvg < 240){
								right++;							//In front. Right and back. 	
								backward++;
							}
						}else if(mAvg > 290){						//Line is to the rear.	
							//cout << "Right side" << endl;
							left++;									//turn an arbitrary direction
						}else if(mAvg < 190){						//Line is in front of me.			
							//cout << "Left side" << endl;			//Directions are opposite to avoid extreme spins.
							right++;								//turn an arbitrary direction
						}

					}else{ //The line isn't vertical or hoz....
						//Let's re-use the averaging above, but combine X & Y together.
						//Summ == X, Summ2 == Y
						Summ = 0, Summ2 = 0;

						for(int i = 0; i < xLine.size();i++){			//Take the average
								Summ = Summ + xLine.at(i);			//Max X = 640
						}												//Half X = 320
						for(int i = 0; i < yLine.size();i++){
								Summ2 = Summ2 + yLine.at(i);			//Max Y = 480
						}
						Summ = (Summ / (xLine.size()));
						Summ2 = (Summ2 / (yLine.size()));



						if( 270 <= Summ && Summ <= 370){		//If it's somewhere near center on X
							if(Summ >= 320){						//Near center, slightly right.
								if( 190 <= Summ2 && Summ2 <= 290){			//If this line is near the center on the right				
									if(Summ2 >= 240){
										forward++;							//Move away (near center, slightly behind).
									}else if(Summ2 < 240){
										backward++;							//Move away (near center, slightly in front).
									}
								}else if(Summ2 > 290){						//Line is near center, but behind
									right++;								//turn towards it
								}else if(Summ2 < 190){						//Line is near center, but in front				
									left++;								//Turn left 
								}

							}else if(Summ < 320){						//Near center, slightly left
								if( 190 <= Summ2 && Summ2 <= 290){			//If this line is near the center on the right				
									if(Summ2 >= 240){
										forward++;							//Move away (near center, slightly behind).
									}else if(Summ2 < 240){
										backward++;							//Move away (near center, slightly in front).
									}
								}else if(Summ2 > 290){						//Line is near center, but behind		
									left++;									//turn left
								}else if(Summ2 < 190){						//Line is near center, but in front				
									right++;								//Turn right
								}
							}
						}else if(Summ > 370){									//If it's on the far right X
							//cout << "Right side" << endl;
							if( 190 <= Summ2 && Summ2 <= 290){			//If this line is near the center on the right				
								if(Summ2 >= 240){
									left++;								//Conditions for upper center on right	
									forward++;
								}else if(Summ2 < 240){
									right++;							// lower center on the right. 	
									forward++;
								}
							}else if(Summ2 > 290){						//Line is in the lower right	
								right++;								//turn towards it
							}else if(Summ2 < 190){						//Line is in the upper right			
								left++;								//Turn left 
							}
						}else if(Summ < 270){					//If it's on the far left X
							//cout << "Left side" << endl;
							if( 190 <= Summ2 && Summ2 <= 290){			//If this line is near the center on the left				
								if(Summ2 >= 240){
									right++;								//Conditions for upper center on left	
									forward++;
								}else if(Summ2 < 240){
									left++;							// lower center on the left. 	
									forward++;
								}
							}else if(Summ2 > 290){						//Line is in the lower left	
								left++;									//turn towards it
							}else if(Summ2 < 190){						//Line is in the upper left			
								right++;								//Turn right 
							}
						}

						//Quick cleanup
						Summ = 0, Summ2 = 0;
						mAvg = 0, m2Avg = 0;
					}//End lines.***********

				}else{				//Else it's a corner, which way do we turn?
					//cout << "corner" << endl;
					for(int i = 0; i < (vYellow.size()); i++){				//Gather the points
							xLine.push_back(vYellow.at(i).getxPos());
							yLine.push_back(vYellow.at(i).getyPos());
					}

					//For this section we are going to use quadrants to determine where the points reside.
					//Points shouls only reside in two or three quadrants, so we can use that to determine which 
					//direction to turn.
					int q1=0, q2=0, q3=0, q4=0;
					for(int i = 0; i < xLine.size(); i++){  //X and Y are the same size.
						if(xLine.at(i) < 320){					//Max X = 640
							if(yLine.at(i) <= 240){				//Max Y = 480						
								q1++;	//Top left
							}else{
								q3++;	//Bottom left
							}
						}else{
							if(yLine.at(i) <= 240){				//Max Y = 480						
								q2++;	//Top right
							}else{
								q4++;	//Bottom right
							}
						}						
					}	//End point counting.

				//Now our quadrants contain the number of points in each. 
					if(q1 <= 2){   //Clean up low accuracy values.
						q1 = 0;
					}
					if(q2 <= 2){
						q2 = 0;
					}
					if(q3 <= 2){
						q3 = 0;
					}
					if(q4 <= 2){
						q4 = 0;
					}	

					if(q1 > 0  && q2 > 0 && q3 > 0){   //Corner across upper left
						right++;
						backward++;
					}else if(q4 > 0  && q2 > 0 && q3 > 0){//Corner across lower right
						forward++;
					}else if(q1 > 0  && q4 > 0 && q3 > 0){//Corner across lower left
						right++; 
					}else if(q1 > 0  && q2 > 0 && q4 > 0){//Corner across upper right
						left++;
						backward++;

					//Now for two-quadrant conditions.
					}else if(q1 > 0 && q2 > 0){   //Top two
						right++;
						backward++;
					}else if(q1 > 0 && q3 > 0){		//Two left
						right++;	
					}else if(q2 > 0 && q4 > 0){		//Two right
						left++;
					}else if(q3 > 0 && q4 > 0){		//tow bottom
						right++;
						forward++;
					}

				}
				LineCounter.clear();
				xLine.clear();
				yLine.clear();
				slopes.clear();
				slopes2.clear();
			}
		}   //End Yellow Calculations!***********
		
		
		if((vRed.size()) >= 1){   //Red tolerances should be very low as we should only have one target.
			Summ = 0, Summ2 = 0, div=0; 
			for(int i = 0; i < (vRed.size()); i++){				//Get the average X and Y
				Summ = Summ + (vRed.at(i).getxPos());
				Summ2 = Summ2 + (vRed.at(i).getyPos());
				div++;
			}
		Summ = Summ / div;
		Summ = Summ2 /div;
		RedXcounter.push_back(Summ);
		RedYcounter.push_back(Summ2);
		Summ = 0, Summ2 = 0, div = 0;
			
		if(RedXcounter.size() >= 120){		//This runs every second or so....
			for(int i=0; i < RedXcounter.size(); i++){
				Summ = Summ + (RedXcounter.at(i));
				Summ2 = Summ2 + (RedYcounter.at(i));
				div++;
			}
			Summ = Summ /div;
			Summ2 = Summ2 / div;
			//We are going to re-use some of the line calculations here. 
				if( 260 <= Summ && Summ <= 380){		//If it's somewhere near center on X
						if( 180 <= Summ2 && Summ2 <= 300){			//If this point is near the center				
							down++;
						}else if(Summ2 > 300){						//point is near center, but behind
							backward++;								
						}else if(Summ2 < 180){						//point is near center, but in front		
							forward++;								
						}
				}else if(Summ > 380){									//If it's on the far right X
					//cout << "Right side" << endl;
						if( 180 <= Summ2 && Summ2 <= 300){			//If this point is near the center				
							right++;
						}else if(Summ2 > 300){						//point is near center, but behind
							right++;
							backward++;								
						}else if(Summ2 < 180){						//point is near center, but in front		
							right++;
							forward++;								
						}
				}else if(Summ < 260){					//If it's on the far left X
					//cout << "Left side" << endl;
						if( 180 <= Summ2 && Summ2 <= 300){			//If this point is near the center				
							left++;
						}else if(Summ2 > 300){						//point is near center, but behind
							left++;
							backward++;								
						}else if(Summ2 < 180){						//point is near center, but in front		
							left++;
							forward++;								
						}
				}
			Summ = 0, Summ2 = 0, div =0;		//Cleanup
			RedXcounter.clear();
			RedYcounter.clear();
			}
		}  //End Red Calculations!**************************
		
		
		if((vBlack.size()) >= 1){  //this needs to be lightweight as we will register with the line...
			for(int i = 0; i < vBlack.size(); i++){				//Gather the points
				xLine.push_back(vBlack.at(i).getxPos());
				yLine.push_back(vBlack.at(i).getyPos());
			}
			std::sort(xLine.begin(), xLine.end());
			std::sort(yLine.begin(), yLine.end());
			
			for(int i = 0; i < (xLine.size() - 1); i++){	//We need to find points with no neighbors
				if(xLine.at(i) < xLine.at(i+1) + 10 || xLine.at(i) > xLine.at(i+1) - 10){
					xLine.at(i) = 0;	
					yLine.at(i) = 0;
				}
				if(yLine.at(i) < yLine.at(i+1) + 10 || yLine.at(i) > yLine.at(i+1) - 10){
					xLine.at(i) = 0;	
					yLine.at(i) = 0;
				}
			}
			//Average those items.
			for(int i = 0; i < (xLine.size()); i++){				//Get the average X and Y
				Summ = Summ + (xLine.at(i));
				Summ2 = Summ2 + (yLine.at(i));
			}
			Summ = Summ / div;
			Summ2 = Summ2 / div;
			BlackXcounter.push_back(Summ);
			BlackYcounter.push_back(Summ2);
			Summ = 0; Summ2 = 0; div = 0;
			
			
			if(BlackXcounter.size() >= 120){		//This runs every second or so....
				for(int i=0; i < BlackXcounter.size(); i++){
					Summ = Summ + (BlackXcounter.at(i));
					Summ2 = Summ2 + (BlackYcounter.at(i));
					div++;
				}
				Summ = Summ /div;
				Summ2 = Summ2 / div;
			}
			
			//Implement the same search as Red. 
			if( 260 <= Summ && Summ <= 380){		//If it's somewhere near center on X
					if( 180 <= Summ2 && Summ2 <= 300){			//If this point is near the center				
						down++;
					}else if(Summ2 > 300){						//point is near center, but behind
						backward++;								
					}else if(Summ2 < 180){						//point is near center, but in front		
						forward++;								
					}
			}else if(Summ > 380){									//If it's on the far right X
				//cout << "Right side" << endl;
					if( 180 <= Summ2 && Summ2 <= 300){			//If this point is near the center				
						right++;
					}else if(Summ2 > 300){						//point is near center, but behind
						right++;
						backward++;								
					}else if(Summ2 < 180){						//point is near center, but in front		
						right++;
						forward++;								
					}
			}else if(Summ < 260){					//If it's on the far left X
				//cout << "Left side" << endl;
					if( 180 <= Summ2 && Summ2 <= 300){			//If this point is near the center				
						left++;
					}else if(Summ2 > 300){						//point is near center, but behind
						left++;
						backward++;								
					}else if(Summ2 < 180){						//point is near center, but in front		
						left++;
						forward++;								
					}
			}
		Summ = 0, Summ2 = 0, div =0;		//Cleanup
		BlackXcounter.clear();
		BlackYcounter.clear();		
		
		
		 }  //End Black.
		
		
		//Default functionality
		iteration++;
		if(up > 500){
			iteration = 0;
			up++;
			forward++;
		}
		
		if(forward > 15){
			cout << "Forward" << endl;
			xdo_t *xdo = xdo_new(NULL);
			xdo_send_keysequence_window(xdo, CURRENTWINDOW, "W", 0);
			forward = 0;
		}
		if(backward > 15){
			cout << "Backward" << endl;
			xdo_t *xdo = xdo_new(NULL);
			xdo_send_keysequence_window(xdo, CURRENTWINDOW, "S", 0);
			backward = 0;
		}
		if(left > 15){
			cout << "Left" << endl;
			xdo_t *xdo = xdo_new(NULL);
			xdo_send_keysequence_window(xdo, CURRENTWINDOW, "A", 0);
			left = 0;
		}
		if(right > 15){
			cout << "Right" << endl;
			xdo_t *xdo = xdo_new(NULL);
			xdo_send_keysequence_window(xdo, CURRENTWINDOW, "D", 0);
			right = 0;
		}
		if(up > 15){
			cout << "Up" << endl;
			xdo_t *xdo = xdo_new(NULL);
			xdo_send_keysequence_window(xdo, CURRENTWINDOW, "Q", 0);
			up = 0;
		}
		if(down > 15){
			cout << "Down" << endl;
			xdo_t *xdo = xdo_new(NULL);
			xdo_send_keysequence_window(xdo, CURRENTWINDOW, "E", 0);
			down = 0;
		}
		
		
		
		
		slopes.clear();		//reset for the next cycle.
		slopes2.clear();
		vYellow.clear(); 
		vRed.clear();
		vBlack.clear();	
		div = 0, div2 = 0;
		mAvg = 0, m2Avg = 0;
		Summ = 0, Summ2 = 0;
		
		
	} //End While(autonomous) Loop****************************************************************
	
	threadded = true;	//Reset before closing the thread. 
}




//************************************************************************************************************




//***********************************************************************************************************************



int main(int argc, char* argv[])
{
	//if we would like to calibrate our filter values, set to true.
	bool calibrationMode = true;
	bool webcammode = true;
	threadded = true;
	int killcount = 0;
	
	//Matrix to store each frame of the webcam feed
	Mat cameraFeed;
	Mat threshold;
	Mat HSV;
	string address;

	
	createTrackbars();
	
	//video capture object to acquire webcam feed
	VideoCapture capture;
	if(webcammode){
			//open capture object at location zero (default location for webcam)
			capture.open(0);
	}
	else{
	cout << "Please input the Remote IP address: ";
	cin >> address;
	capture.open("http://" + address + ":8080/?action=stream");
	}	
	
	//set height and width of capture frame
	capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);
	//start an infinite loop where webcam feed is copied to cameraFeed matrix
	//all of our operations will be performed within this loop
	while(1){
		//store image to matrix

		capture.read(cameraFeed);
		//convert frame from BGR to HSV colorspace
		cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
		if(Cali == 0){
			//if in calibration mode, we track objects based on the HSV slider values.
			cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
			inRange(HSV,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX),threshold);
			morphOps(threshold);
			imshow(windowName2,threshold);
			trackFilteredObject(threshold,HSV,cameraFeed);
		}else if(Cali == 1 && calibrationMode == true){
			destroyWindow(trackbarWindowName);
			destroyWindow(windowName2);
			calibrationMode = false;
		}
		
			objective border("Border");	// Declare new objects here;
			objective delivery("Delivery");
			objective landing("L.Z.");
			
		//Implement searching block code here. 
		
		pthread_mutex_lock(&lock);		//Lock our vectors from the autonomous code
		
		//Yellow search
		cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
		if(Bordertoggle == 1){	
		inRange(HSV,Scalar(BorderH_MIN,BorderS_MIN,BorderV_MIN),Scalar(BorderH_MAX,BorderS_MAX,BorderV_MAX),threshold);
		}else{
		inRange(HSV,border.getHSVmin(),border.getHSVmax(),threshold);
		}
		morphOps(threshold);
		trackFilteredObject(border, threshold,HSV,cameraFeed, vborders);	
		
		//Red search
		cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
		if(Delivtoggle == 1){	
		inRange(HSV,Scalar(DelivH_MIN,DelivS_MIN,DelivV_MIN),Scalar(DelivH_MAX,DelivS_MAX,DelivV_MAX),threshold);
		}else{
		inRange(HSV,delivery.getHSVmin(),delivery.getHSVmax(),threshold);
		}
		morphOps(threshold);
		trackFilteredObject(delivery, threshold,HSV,cameraFeed, vpickups);	
		
		//Black search
		cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
		if(Blacktoggle == 1){	
		inRange(HSV,Scalar(BlH_MIN,BlS_MIN,BlV_MIN),Scalar(BlH_MAX,BlS_MAX,BlV_MAX),threshold);
		}else{
		inRange(HSV,landing.getHSVmin(),landing.getHSVmax(),threshold);
		}
		morphOps(threshold);
		trackFilteredObject(landing,threshold,HSV,cameraFeed, vlandings);	
		
		
		pthread_mutex_unlock(&lock); 		//unlock our vectors for autonomous
		
			
		imshow(windowName,cameraFeed);
		createTrackbar( "Autonomous", windowName, &Autonomous, setter, on_trackbar );
		createTrackbar( "Kill", windowName, &Kill, setter, on_trackbar );
		
		vborders.clear();
		vpickups.clear();
		vlandings.clear();
		
		
		if(threadded){			//Make sure one isn't running already
			if(Autonomous){			//Start thread
				std::thread Robotic (Calculations);
				Robotic.detach();
				threadded = 0;			//Mark as running
			}
		}
		
		if(Kill){		//Killswitch implelemtation
			xdo_t *xdo = xdo_new(NULL);
  			xdo_send_keysequence_window(xdo, CURRENTWINDOW, "P", 0);
			killcount++;
		}
		//delay 30ms so that screen can refresh.
		//image will not appear without this waitKey() command
		waitKey(30);
		
		if(killcount > 10){
			return 0;
		}
	}
		
	return 0;
}




