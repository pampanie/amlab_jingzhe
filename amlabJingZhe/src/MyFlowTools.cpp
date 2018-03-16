//
//  MyFlowTools.cpp
//  amlabJingZhe
//
//  Created by Pampa Nie on 10/03/2018.
//

#include "MyFlowTools.h"

//struct physicParameters
//{
//	physicParameters():{}
//	
//};

enum drawModeEnum{
	DRAW_COMPOSITE = 0,
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




MyFlowTools::MyFlowTools(){
	
}

MyFlowTools::~MyFlowTools(){
	
}

void MyFlowTools::setup(int _w,int _h,float _ratio){
	drawWidth = _w;
	drawHeight = _h;
	flowWidth = _w / _ratio;
	flowHeight = _h / _ratio;
	ratio = _ratio;
	
	drawMode = 0; // default draw composite
	
}

void MyFlowTools::update(){
	
}

void MyFlowTools::draw(){
	switch(drawMode) {
		case DRAW_COMPOSITE: drawComposite(); break;
		case DRAW_PARTICLES: drawParticles(); break;
		case DRAW_FLUID_FIELDS: drawFluidFields(); break;
			//			case DRAW_FLUID_DENSITY: drawFluidDensity(); break;
			//			case DRAW_FLUID_VELOCITY: drawFluidVelocity(); break;
			//			case DRAW_FLUID_PRESSURE: drawFluidPressure(); break;
			//			case DRAW_FLUID_TEMPERATURE: drawFluidTemperature(); break;
			//			case DRAW_FLUID_DIVERGENCE: drawFluidDivergence(); break;
			//			case DRAW_FLUID_VORTICITY: drawFluidVorticity(); break;
			//			case DRAW_FLUID_BUOYANCY: drawFluidBuoyance(); break;
		case DRAW_FLUID_OBSTACLE: drawFluidObstacle(); break;
			//			case DRAW_FLOW_MASK: drawMask(); break;
			//			case DRAW_OPTICAL_FLOW: drawOpticalFlow(); break;
		case DRAW_SOURCE: drawSource(); break;
			//			case DRAW_MOUSE: drawMouseForces(); break;
			//			case DRAW_VELDOTS: drawVelocityDots(); break;
	}
	
}

void MyFlowTools::exit(){
	
}

void MyFlowTools::setFlowToDrawRatio(float _ratio){
	ratio = _ratio;
	flowWidth = drawWidth / ratio;
	flowHeight = drawHeight / ratio;
}
float MyFlowTools::getFlowToDrawRatio(){
	return ratio;
}

void MyFlowTools::setDrawWidth(int _w){
	drawWidth = _w;
	flowWidth = drawWidth / ratio;
}
int MyFlowTools::getDrawWidth(){
	return drawWidth;
}

void MyFlowTools::setDrawHeight(int _h){
	drawHeight = _h;
	flowHeight = drawHeight / ratio;
}
int MyFlowTools::getDrawHeight(){
	return drawHeight;
}

int MyFlowTools::getFlowWidth(){
	return flowWidth;
}

int MyFlowTools::getFlowHeight(){
	return flowHeight;
}

void MyFlowTools::drawComposite(int _x, int _y, int _width, int _height){
	ofPushStyle();
	
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	fluidSimulation.draw(_x, _y, _width, _height);
	
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	if (particleFlow.isActive())
		particleFlow.draw(_x, _y, _width, _height);
	ofPopStyle();
}

void MyFlowTools::drawParticles(int _x, int _y, int _width, int _height){
	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	if (particleFlow.isActive())
		particleFlow.draw(_x, _y, _width, _height);
	ofPopStyle();
}

void MyFlowTools::drawFluidFields(int _x, int _y, int _width, int _height){
	ofPushStyle();
	
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	pressureField.setPressure(fluidSimulation.getPressure());
	pressureField.draw(_x, _y, _width, _height);
	velocityTemperatureField.setVelocity(fluidSimulation.getVelocity());
	velocityTemperatureField.setTemperature(fluidSimulation.getTemperature());
	velocityTemperatureField.draw(_x, _y, _width, _height);
	temperatureField.setTemperature(fluidSimulation.getTemperature());
	
	ofPopStyle();
}

void MyFlowTools::drawSource(int _x, int _y, int _width, int _height){
	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_DISABLED);
	cameraFbo.draw(_x, _y, _width, _height);
	ofPopStyle();
}

