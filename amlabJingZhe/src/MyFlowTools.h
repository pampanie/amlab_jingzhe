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

enum drawModeEnum{
	DRAW_NOTHING = 0,
	DRAW_COMPOSITE,
	//	DRAW_FLUID_DENSITY,
	DRAW_PARTICLES,
	//	DRAW_VELDOTS,
	DRAW_FLUID_FIELDS,
	//	DRAW_FLUID_VELOCITY,
	//	DRAW_FLUID_PRESSURE,
	//	DRAW_FLUID_TEMPERATURE,
	//	DRAW_FLUID_DIVERGENCE,
	//	DRAW_FLUID_VORTICITY,
	//	DRAW_FLUID_BUOYANCY,
	DRAW_FLUID_OBSTACLE,
	//	DRAW_FLOW_MASK,
	//	DRAW_OPTICAL_FLOW,
	DRAW_SOURCE,
	DRAW_MOUSE // not use yet ================  TODO
};

class MyFlowTools {
public:
	MyFlowTools();
	~MyFlowTools();
	void setup(int _w,int _h,float _ratio);
	void update(ofFbo *_fboForFluidP,ofFbo *_obstacleFboP);
	void draw();
	void exit();
	
	
	void setParticleColor(ofColor _color);
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

//	// not sure if need this in class
//	ftFbo				cameraFbo;
//	ofFbo				fboForFluid;
//	ofFbo				fboForObstacle;
	int 				drawMode;
	
private:
	float ratio;
	int drawWidth;
	int drawHeight;
	int flowWidth;
	int flowHeight;
	
	
	
};

#endif /* MyFlowTools_hpp */
