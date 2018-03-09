#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxGui.h"
#include "ofxFlowTools.h"
using namespace flowTools;

class ofApp : public ofBaseApp,public ofxMidiListener {
	
public:
	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
	// midi -------------------------------------
	void newMidiMessage(ofxMidiMessage& eventArgs);

	// global settings -----------------------
	// FlowTools
	int	flowWidth;
	int	flowHeight;
	int	drawWidth;
	int	drawHeight;
	
	// seperate gui window -------------------
	void setupGui();
	void drawGui(ofEventArgs & args);

	ofParameterGroup 		parameters;
	ofParameter<float> 		radius;
	ofParameter<ofColor> 	color;
	ofxPanel 				gui;
	
	
	// Camera -----------------------------
	ofVideoGrabber		simpleCam;
	bool				didCamUpdate;
	ftFbo				cameraFbo;
	ofParameter<bool>	doFlipCamera;
	bool				showCameraSource;
	void				drawCameraSource();

	// Time -----------------------------------
	float				lastTime;
	float				deltaTime;
	
	// particles ------------------------------------
	ftOpticalFlow		opticalFlow;
	ftVelocityMask		velocityMask;
	ftFluidSimulation	fluidSimulation;
	ftParticleFlow		particleFlow;
	bool				showParticles;
	void				drawParticles();
	
	
};
