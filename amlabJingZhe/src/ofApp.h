#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxGui.h"
#include "ofxFlowTools.h"
#include "ofxSyphon.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxCv.h"
#include "MyFlowTools.h"

//using namespace flowTools;
//
//enum drawModeEnum{
//	DRAW_NOTHING = 0,
//	DRAW_COMPOSITE,
////	DRAW_FLUID_DENSITY,
//	DRAW_PARTICLES,
////	DRAW_VELDOTS,
//	DRAW_FLUID_FIELDS,
////	DRAW_FLUID_VELOCITY,
////	DRAW_FLUID_PRESSURE,
////	DRAW_FLUID_TEMPERATURE,
////	DRAW_FLUID_DIVERGENCE,
////	DRAW_FLUID_VORTICITY,
////	DRAW_FLUID_BUOYANCY,
//	DRAW_FLUID_OBSTACLE,
////	DRAW_FLOW_MASK,
////	DRAW_OPTICAL_FLOW,
//	DRAW_SOURCE,
//	DRAW_MOUSE
//};

class ofApp : public ofBaseApp,public ofxMidiListener {

public:
	void	setup();
	void	update();
	void	draw();
	void 	exit();
	
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
	
	MyFlowTools		myFlowTools1;
	MyFlowTools		myFlowTools2;

	
	// Kinect 1
	ofxKinect 		kinect1;
	bool 			bThreshWithOpenCV;
	bool			bDrawPointCloud;
	
	int 			nearThreshold;
	int 			farThreshold;
	
	int 			angle;
	
	ofxCvGrayscaleImage grayKinect1Image; // grayscale depth image
	ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
	ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
	
	
	
	
	// Camera
	ofVideoGrabber		simpleCam;
	bool				didCamUpdate;
	ftFbo				cameraFbo;
	ofParameter<bool>	doFlipCamera;
	
	// Time
	float				lastTime;
	float				deltaTime;
	
	// FlowTools
	int					flowWidth;
	int					flowHeight;
	int					drawWidth;
	int					drawHeight;
	
	// GUI
	ofxPanel			gui;
	void				setupGui();
	void				keyPressed(int key);
	void 				drawGui();
	ofParameter<bool>	toggleGuiDraw;
	ofParameter<float>	guiFPS;
	ofParameter<float>	guiMinFPS;
	deque<float>		deltaTimeDeque;
	ofParameter<bool>	doFullScreen;
	void				setFullScreen(bool& _value) { ofSetFullscreen(_value);}
	
	// DRAW
	ofParameter<bool>	doDrawCamBackground;

	
	
	// syphon
	ofxSyphonServer 	mainOutputSyphonServer;
//	ofTexture 			syphonTex;
//	ofPixels 			syphonPixels;
	ofFbo				syphonFbo;
	ofxSyphonClient 	syphonClient;

	ofxSyphonClient		animationSyphonClient;
	bool				useAnimateClient = false;
	
	// midi
	ofxMidiIn 			midiIn;
	void 				newMidiMessage(ofxMidiMessage& eventArgs);
	stringstream 		text;
	ofxMidiMessage 		midiMessage;

	
	// camera vbo for fluid
	ofFbo				camFboForFluid;
	
	// video for making fluid
	ofFbo				videoFboForFluid;
	// fbo for optical flow
	ofFbo				fboForFluid;
	int					fboForFluidW;
	int					fboForFluidH;
	
	// obstacle fbo
	ofFbo				fboForObstacle;
	int					fboForObstacleW;
	int					fboForObstacleH;
	
	
	// local video player
	ofVideoPlayer 		videoPlayer;
	
};

