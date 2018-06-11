#include "objective.h"

objective::objective(void){
}

objective::~objective(void){
}

objective::objective(string name){ //Duplicate this for more objects
	setType(name);
	if( name == "Border" ){
			setHSVmin(Scalar(0,117,033));
			setHSVmax(Scalar(255,255,218));
			setColor(Scalar(0,250,250));
	}
	if( name == "Delivery" ){
			//setHSVmin(Scalar(135,34,56));
			//setHSVmax(Scalar(187,173,256));
			setHSVmin(Scalar(255,255,255));
			setHSVmax(Scalar(255,255,255));
			setColor(Scalar(0,0,250));
	}
	if( name == "L.Z." ){
			//setHSVmin(Scalar(0,0,0));
			//setHSVmax(Scalar(231,171,47));
			setHSVmin(Scalar(255,255,255));
			setHSVmax(Scalar(255,255,255));
			setColor(Scalar(250,250,0));
	}
	
	
}


int objective::getxPos(){
	 return objective::xPos;
}

void objective::setxPos(int x){
	objective::xPos = x;	
}


int objective::getyPos(){
	 return objective::yPos;
}

void objective::setyPos(int y){
	objective::yPos = y;
}

Scalar objective::getHSVmin(){
	return HSVmin;
}


Scalar objective::getHSVmax(){
	return HSVmax;
}
	
void objective::setHSVmin(Scalar min){
	objective::HSVmin = min;
}


void objective::setHSVmax(Scalar max){
	objective::HSVmax = max;
}
