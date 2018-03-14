//
//  MyFlowTools.hpp
//  amlabJingZhe
//
//  Created by Pampa Nie on 10/03/2018.
//
#pragma once

#ifndef MyFlowTools_h
#define MyFlowTools_h

#include <stdio.h>
#include "ofxFlowTools.h"

using namespace flowTools;

class MyFlowTools {
public:
	MyFlowTools();
	~MyFlowTools();
	void setup(int _w,int _h,float _ratio);
	void update();
	void draw();
	void exit();
	
	void drawComposite(){
		drawComposite(0,0,drawWidth,drawHeight);
	};
	void drawComposite(int _x, int _y, int _width, int _height);

	void drawParticles(){
		drawParticles(0,0,drawWidth,drawHeight);
	};
	void drawParticles(int _x, int _y, int _width, int _height);

	void drawFluidFields(){
		drawFluidFields(0,0,drawWidth,drawHeight);
	};
	void drawFluidFields(int _x, int _y, int _width, int _height);

	void drawFluidObstacle(){
		drawFluidObstacle(0,0,drawWidth,drawHeight);
	}
	void drawFluidObstacle(int _x, int _y, int _width, int _height);

	void drawSource(){
		drawSource(0,0,drawWidth,drawHeight);
	}
	void drawSource(int _x, int _y, int _width, int _height);
	
	int getDrawWidth();
	int getDrawHeight();
	int getFlowWidth();
	int getFlowHeight();
	void setDrawWidth(int _w);
	void setDrawHeight(int _h);
	void setFlowToDrawRatio(float _ratio);
	float getFlowToDrawRatio();
	
	ftOpticalFlow		opticalFlow;
	ftVelocityMask		velocityMask;
	ftFluidSimulation	fluidSimulation;
	ftParticleFlow		particleFlow;
	ftVelocityField		velocityField;
	ftTemperatureField	temperatureField;
	ftPressureField		pressureField;
	ftVTField			velocityTemperatureField;

	ftFbo				cameraFbo;
	ofFbo				fboForFluid;
	ofFbo				obstacleFbo;
	int 				drawMode;
	
private:
	float ratio;
	int drawWidth;
	int drawHeight;
	int flowWidth;
	int flowHeight;
	
	
	
};

#endif /* MyFlowTools_hpp */
