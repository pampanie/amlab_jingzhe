#include "ofApp.h"



//--------------------------------------------------------------
void ofApp::setup(){
	
	ofSetVerticalSync(false);
	ofSetLogLevel(OF_LOG_NOTICE);
	
	//setup kinect 1
	// enable depth->video image calibration
	kinect1.setRegistration(true);
	
	kinect1.init();
	//kinect.init(true); // shows infrared instead of RGB video image
	//kinect.init(false, false); // disable video image (faster fps)
	
	kinect1.open();		// opens first available kinect
	//kinect.open(1);	// open a kinect by id, starting with 0 (sorted by serial # lexicographically))
	//kinect.open("A00362A08602047A");	// open a kinect using it's unique serial #
	
	// print the intrinsic IR sensor values
	if(kinect1.isConnected()) {
		ofLogNotice() << "sensor-emitter dist: " << kinect1.getSensorEmitterDistance() << "cm";
		ofLogNotice() << "sensor-camera dist:  " << kinect1.getSensorCameraDistance() << "cm";
		ofLogNotice() << "zero plane pixel size: " << kinect1.getZeroPlanePixelSize() << "mm";
		ofLogNotice() << "zero plane dist: " << kinect1.getZeroPlaneDistance() << "mm";
	}
	
	nearThreshold = 245;
	farThreshold = 213;
	bThreshWithOpenCV = true;
	
	
	
	// setup render size
//	drawWidth = 1920;
//	drawHeight = 1080;
//
	drawWidth = 1280;
	drawHeight = 720;
	fboForFluidW = 640;
	fboForFluidH = 360;
	
	// process all but the density on 16th resolution
	flowWidth = drawWidth / 4;
	flowHeight = drawHeight / 4;
	
	// FLOW & MASK
	opticalFlow.setup(flowWidth, flowHeight);
	velocityMask.setup(drawWidth, drawHeight);
	
	// FLUID & PARTICLES
	fluidSimulation.setup(flowWidth, flowHeight, drawWidth, drawHeight);
//	velocityDots.setup(flowWidth / 4, flowHeight / 4);
	particleFlow.setup(flowWidth, flowHeight, drawWidth, drawHeight);
	
	// image obstacle
	flowToolsLogoImage.load("tupian.png");
//	fluidSimulation.addObstacle(flowToolsLogoImage.getTexture());
	showLogo = true;
	
	// fbo obstacle
	obstacleFboWidth = drawWidth;
	obstacleFboHeight = drawHeight;
	
	// VISUALIZATION
//	displayScalar.setup(flowWidth, flowHeight);
//	velocityField.setup(flowWidth / 4, flowHeight / 4);
//	temperatureField.setup(flowWidth / 4, flowHeight / 4);
//	pressureField.setup(flowWidth / 4, flowHeight / 4);
//	velocityTemperatureField.setup(flowWidth / 4, flowHeight / 4);
	
	// MOUSE DRAW
//	mouseForces.setup(flowWidth, flowHeight, drawWidth, drawHeight);
	
	// CAMERA
//	simpleCam.setup(640, 480, true);
//	didCamUpdate = false;
//	cameraFbo.allocate(640, 480);
//	cameraFbo.black();
	
	// cameara fbo for fluid
	camFboForFluid.allocate(640, 480);
	
	// video fbo for fuild
	// fixed w h for test only ==============  TODO
	videoFboForFluid.allocate(640,480);
	fboForFluid.allocate(640, 480);
	// obstacle fbo
	obstacleFbo.allocate(drawWidth, drawHeight);
	
	// GUI
	setupGui();
	
	lastTime = ofGetElapsedTimef();
	
	// syphon
	syphonFbo.allocate(drawWidth,drawHeight,GL_RGBA);

//	syphonPixels.allocate(drawWidth, drawHeight, OF_PIXELS_RGB);
//	syphonTex.allocate(syphonPixels);
	mainOutputSyphonServer.setName("A.M Lab -- Jing Zhe");

	syphonClient.setup();
	syphonClient.setServerName("Video");
	
	
	
	// midi
//	midiIn.listPorts(); // via instance
//	midiIn.openPort(0);
//	// add ofApp as a listener
//	midiIn.addListener(this);
//
//	midiIn.ignoreTypes(false, false, false);
//
//	// print received messages to the console
//	midiIn.setVerbose(true);
//
	
	
}

//--------------------------------------------------------------
void ofApp::setupGui() {
	
	gui.setup("settings");
	gui.setDefaultBackgroundColor(ofColor(0, 0, 0, 127));
	gui.setDefaultFillColor(ofColor(160, 160, 160, 160));
	gui.add(guiFPS.set("average FPS", 0, 0, 60));
	gui.add(guiMinFPS.set("minimum FPS", 0, 0, 60));
//	gui.add(doFullScreen.set("fullscreen (F)", false));
	doFullScreen.addListener(this, &ofApp::setFullScreen);
//	gui.add(toggleGuiDraw.set("show gui (G)", false));
	gui.add(doFlipCamera.set("flip camera", true));
	gui.add(doDrawCamBackground.set("draw camera (C)", true));
	gui.add(drawMode.set("draw mode", DRAW_COMPOSITE, DRAW_COMPOSITE, DRAW_MOUSE));
//	gui.add(drawMode.set("draw mode", DRAW_COMPOSITE, DRAW_COMPOSITE,DRAW_COMPOSITE));
	drawMode.addListener(this, &ofApp::drawModeSetName);
	gui.add(drawName.set("MODE", "draw name"));
	
	
	int guiColorSwitch = 0;
	ofColor guiHeaderColor[2];
	guiHeaderColor[0].set(160, 160, 80, 200);
	guiHeaderColor[1].set(80, 160, 160, 200);
	ofColor guiFillColor[2];
	guiFillColor[0].set(160, 160, 80, 200);
	guiFillColor[1].set(80, 160, 160, 200);
	
	gui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
	gui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
	guiColorSwitch = 1 - guiColorSwitch;
	gui.add(opticalFlow.parameters);
	
	gui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
	gui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
	guiColorSwitch = 1 - guiColorSwitch;
	gui.add(velocityMask.parameters);
	
	gui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
	gui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
	guiColorSwitch = 1 - guiColorSwitch;
	gui.add(fluidSimulation.parameters);
	
	gui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
	gui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
	guiColorSwitch = 1 - guiColorSwitch;
	gui.add(particleFlow.parameters);
	
//	gui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
//	gui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
//	guiColorSwitch = 1 - guiColorSwitch;
//	gui.add(mouseForces.leftButtonParameters);
	
//	gui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
//	gui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
//	guiColorSwitch = 1 - guiColorSwitch;
//	gui.add(mouseForces.rightButtonParameters);
	
//	visualizeParameters.setName("visualizers");
//	visualizeParameters.add(showScalar.set("show scalar", true));
//	visualizeParameters.add(showField.set("show field", true));
//	visualizeParameters.add(displayScalarScale.set("scalar scale", 0.15, 0.05, 1.0));
//	displayScalarScale.addListener(this, &ofApp::setDisplayScalarScale);
//	visualizeParameters.add(velocityFieldScale.set("velocity scale", 0.1, 0.0, 0.5));
//	velocityFieldScale.addListener(this, &ofApp::setVelocityFieldScale);
//	visualizeParameters.add(temperatureFieldScale.set("temperature scale", 0.1, 0.0, 0.5));
//	temperatureFieldScale.addListener(this, &ofApp::setTemperatureFieldScale);
//	visualizeParameters.add(pressureFieldScale.set("pressure scale", 0.02, 0.0, 0.5));
//	pressureFieldScale.addListener(this, &ofApp::setPressureFieldScale);
//	visualizeParameters.add(velocityLineSmooth.set("line smooth", false));
//	velocityLineSmooth.addListener(this, &ofApp::setVelocityLineSmooth);
//
//	gui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
//	gui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
//	guiColorSwitch = 1 - guiColorSwitch;
//	gui.add(visualizeParameters);
	
//	gui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
//	gui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
//	guiColorSwitch = 1 - guiColorSwitch;
//	gui.add(velocityDots.parameters);
	
	// if the settings file is not present the parameters will not be set during this setup
	if (!ofFile("settings.xml"))
		gui.saveToFile("settings.xml");
	
	gui.loadFromFile("settings.xml");
	
	gui.minimizeAll();
	toggleGuiDraw = true;
	
}

//--------------------------------------------------------------
void ofApp::update(){
	
	kinect1.update();

	if(kinect1.isFrameNew()) {
		grayKinect1Image.setFromPixels(kinect1.getDepthPixels());
		// we do two thresholds - one for the far plane and one for the near plane
		// we then do a cvAnd to get the pixels which are a union of the two thresholds
		if(bThreshWithOpenCV) {
			grayThreshNear = grayKinect1Image;
			grayThreshFar = grayKinect1Image;
			grayThreshNear.threshold(nearThreshold, true);
			grayThreshFar.threshold(farThreshold);
			cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayKinect1Image.getCvImage(), NULL);
		} else {
			
			// or we do it ourselves - show people how they can work with the pixels
			ofPixels & pix = grayKinect1Image.getPixels();
			int numPixels = pix.size();
			for(int i = 0; i < numPixels; i++) {
				if(pix[i] < nearThreshold && pix[i] > farThreshold) {
					pix[i] = 255;
				} else {
					pix[i] = 0;
				}
			}
		}
		
		grayKinect1Image.flagImageChanged();

		
		
	};
	
	deltaTime = ofGetElapsedTimef() - lastTime;
	lastTime = ofGetElapsedTimef();
	simpleCam.update();
//
//	if (simpleCam.isFrameNew()) {
//		ofPushStyle();
//		ofEnableBlendMode(OF_BLENDMODE_DISABLED);
//		cameraFbo.begin();
//
//		if (doFlipCamera)
//			simpleCam.draw(cameraFbo.getWidth(), 0, -cameraFbo.getWidth(), cameraFbo.getHeight());  // Flip Horizontal
//		else
//			simpleCam.draw(0, 0, cameraFbo.getWidth(), cameraFbo.getHeight());
//		cameraFbo.end();
//		ofPopStyle();
//
//		// set source from camera
//		opticalFlow.setSource(cameraFbo.getTexture());
//
//
//		// opticalFlow.update(deltaTime);
//		// use internal deltatime instead
//		opticalFlow.update();
//
//		velocityMask.setDensity(cameraFbo.getTexture());
//		velocityMask.setVelocity(opticalFlow.getOpticalFlow());
//		velocityMask.update();
//	}
//
	
	
	// animation/fbo for making fluid/particles
//	fboForFluid.begin();
//	ofSetColor(0, 200, 0);
//	ofDrawRectangle(0, 0, 640, 20);
//	fboForFluid.end();
//
//	opticalFlow.setSource(fboForFluid.getTexture());
//	opticalFlow.update();
//	velocityMask.setDensity(fboForFluid.getTexture());
//	velocityMask.setVelocity(opticalFlow.getOpticalFlow());
//	velocityMask.update();
	
	
	
	// set source from syphon client ===============================  TODO
	fboForFluid.begin();
	syphonClient.draw(0,0, 640, 360);
	fboForFluid.end();
	
	opticalFlow.setSource(fboForFluid.getTexture());
	opticalFlow.update();
	velocityMask.setDensity(fboForFluid.getTexture());
	velocityMask.setVelocity(opticalFlow.getOpticalFlow());
	velocityMask.update();

	
	
	
	// obstacle fbo
	obstacleFbo.begin();
//	ofSetColor(255, 255, 255);
//	ofDrawRectangle(100, 100, 600, 200);
	grayKinect1Image.draw(0, 0, drawWidth,drawHeight);

	obstacleFbo.end();
	
	
	// fluid
	fluidSimulation.addVelocity(opticalFlow.getOpticalFlowDecay());
	fluidSimulation.addDensity(velocityMask.getColorMask());
	fluidSimulation.addTemperature(velocityMask.getLuminanceMask());
//	fluidSimulation.addObstacle(obstacleFbo.getTexture());
	// try to add temporary obstacle
	fluidSimulation.addTempObstacle(obstacleFbo.getTexture());


	fluidSimulation.update();
	
	if (particleFlow.isActive()) {
		particleFlow.setSpeed(fluidSimulation.getSpeed());
		particleFlow.setCellSize(fluidSimulation.getCellSize());
		particleFlow.addFlowVelocity(opticalFlow.getOpticalFlow());
		particleFlow.addFluidVelocity(fluidSimulation.getVelocity());
		//		particleFlow.addDensity(fluidSimulation.getDensity());
		particleFlow.setObstacle(fluidSimulation.getObstacle());
	}
	particleFlow.update();

	
	
	
	
//	backup
//	mouseForces.update(deltaTime);

	//	for (int i=0; i<mouseForces.getNumForces(); i++) {
	//		if (mouseForces.didChange(i)) {
	//			switch (mouseForces.getType(i)) {
	//				case FT_DENSITY:
	//					fluidSimulation.addDensity(mouseForces.getTextureReference(i), mouseForces.getStrength(i));
	//					break;
	//				case FT_VELOCITY:
	//					fluidSimulation.addVelocity(mouseForces.getTextureReference(i), mouseForces.getStrength(i));
	//					particleFlow.addFlowVelocity(mouseForces.getTextureReference(i), mouseForces.getStrength(i));
	//					break;
	//				case FT_TEMPERATURE:
	//					fluidSimulation.addTemperature(mouseForces.getTextureReference(i), mouseForces.getStrength(i));
	//					break;
	//				case FT_PRESSURE:
	//					fluidSimulation.addPressure(mouseForces.getTextureReference(i), mouseForces.getStrength(i));
	//					break;
	//				case FT_OBSTACLE:
	//					fluidSimulation.addTempObstacle(mouseForces.getTextureReference(i));
	//				default:
	//					break;
	//			}
	//		}
	//	}
	
}
//--------------------------------------------------------------

void ofApp::exit() {
	
	// clean up
	midiIn.closePort();
	midiIn.removeListener(this);
	
	kinect1.setCameraTiltAngle(0); // zero the tilt on exit
	kinect1.close();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
		case 'G':
		case 'g': toggleGuiDraw = !toggleGuiDraw; break;
		case 'f':
		case 'F': doFullScreen.set(!doFullScreen.get()); break;
		case 'c':
		case 'C': doDrawCamBackground.set(!doDrawCamBackground.get()); break;
			
//		case '1': drawMode.set(DRAW_COMPOSITE); break;
//		case '2': drawMode.set(DRAW_FLUID_FIELDS); break;
//		case '3': drawMode.set(DRAW_FLUID_VELOCITY); break;
//		case '4': drawMode.set(DRAW_FLUID_PRESSURE); break;
//		case '5': drawMode.set(DRAW_FLUID_TEMPERATURE); break;
//		case '6': drawMode.set(DRAW_OPTICAL_FLOW); break;
//		case '7': drawMode.set(DRAW_FLOW_MASK); break;
//		case '8': drawMode.set(DRAW_MOUSE); break;
			
		case 'r':
		case 'R':
			fluidSimulation.reset();
			fluidSimulation.addObstacle(flowToolsLogoImage.getTexture());
//			mouseForces.reset();
			break;
			
			
		// control kinect1
		case OF_KEY_UP:
			angle++;
			if(angle>30) angle=30;
			kinect1.setCameraTiltAngle(angle);
			break;
			
		case OF_KEY_DOWN:
			angle--;
			if(angle<-30) angle=-30;
			kinect1.setCameraTiltAngle(angle);
			break;
		case '>':
		case '.':
			farThreshold ++;
			if (farThreshold > 255) farThreshold = 255;
			break;
			
		case '<':
		case ',':
			farThreshold --;
			if (farThreshold < 0) farThreshold = 0;
			break;
			
		case '+':
		case '=':
			nearThreshold ++;
			if (nearThreshold > 255) nearThreshold = 255;
			break;
			
		case '-':
			nearThreshold --;
			if (nearThreshold < 0) nearThreshold = 0;
			break;
		default: break;
	}
	
	
	
}

//--------------------------------------------------------------
void ofApp::drawModeSetName(int &_value) {
	switch(_value) {
		case DRAW_COMPOSITE:		drawName.set("Composite      (1)"); break;
		case DRAW_PARTICLES:		drawName.set("Particles      "); break;
		case DRAW_FLUID_FIELDS:		drawName.set("Fluid Fields   (2)"); break;
//		case DRAW_FLUID_DENSITY:	drawName.set("Fluid Density  "); break;
//		case DRAW_FLUID_VELOCITY:	drawName.set("Fluid Velocity (3)"); break;
//		case DRAW_FLUID_PRESSURE:	drawName.set("Fluid Pressure (4)"); break;
//		case DRAW_FLUID_TEMPERATURE:drawName.set("Fld Temperature(5)"); break;
//		case DRAW_FLUID_DIVERGENCE: drawName.set("Fld Divergence "); break;
//		case DRAW_FLUID_VORTICITY:	drawName.set("Fluid Vorticity"); break;
//		case DRAW_FLUID_BUOYANCY:	drawName.set("Fluid Buoyancy "); break;
		case DRAW_FLUID_OBSTACLE:	drawName.set("Fluid Obstacle "); break;
//		case DRAW_OPTICAL_FLOW:		drawName.set("Optical Flow   (6)"); break;
//		case DRAW_FLOW_MASK:		drawName.set("Flow Mask      (7)"); break;
		case DRAW_SOURCE:			drawName.set("Source         "); break;
//		case DRAW_MOUSE:			drawName.set("Left Mouse     (8)"); break;
//		case DRAW_VELDOTS:			drawName.set("VelDots        (0)"); break;
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	syphonFbo.begin();

	ofClear(0,0);
	if (doDrawCamBackground.get())
		drawSource();
	
	
	if (!toggleGuiDraw) {
		ofHideCursor();
		drawComposite();
	}
	else {
		ofShowCursor();
		switch(drawMode.get()) {
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
//		drawGui();
		
	}
	syphonFbo.end();
	mainOutputSyphonServer.publishTexture(&syphonFbo.getTexture());

	drawGui();
	
	// test obstacle fbo
//	obstacleFbo.draw(0, 0);
	
	// test syphon client
//	syphonClient.draw(0,0);
}

//--------------------------------------------------------------
void ofApp::drawComposite(int _x, int _y, int _width, int _height) {
	ofPushStyle();
	
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	fluidSimulation.draw(_x, _y, _width, _height);
	
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	if (particleFlow.isActive())
		particleFlow.draw(_x, _y, _width, _height);
	
	if (showLogo) {
		flowToolsLogoImage.draw(_x, _y, _width, _height);
	}
	
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawParticles(int _x, int _y, int _width, int _height) {
	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	if (particleFlow.isActive())
		particleFlow.draw(_x, _y, _width, _height);
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawFluidFields(int _x, int _y, int _width, int _height) {
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
//--------------------------------------------------------------

void ofApp::drawFluidObstacle(int _x, int _y, int _width, int _height) {
	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_DISABLED);
	fluidSimulation.getObstacle().draw(_x, _y, _width, _height);
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawSource(int _x, int _y, int _width, int _height) {
	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_DISABLED);
	cameraFbo.draw(_x, _y, _width, _height);
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawGui() {
	guiFPS = (int)(ofGetFrameRate() + 0.5);
	
	// calculate minimum fps
	deltaTimeDeque.push_back(deltaTime);
	
	while (deltaTimeDeque.size() > guiFPS.get())
		deltaTimeDeque.pop_front();
	
	float longestTime = 0;
	for (int i=0; i<deltaTimeDeque.size(); i++){
		if (deltaTimeDeque[i] > longestTime)
			longestTime = deltaTimeDeque[i];
	}
	
	guiMinFPS.set(1.0 / longestTime);
	
	
	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	gui.draw();
	
	// HACK TO COMPENSATE FOR DISSAPEARING MOUSE
	ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
	ofDrawCircle(ofGetMouseX(), ofGetMouseY(), ofGetWindowWidth() / 300.0);
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofDrawCircle(ofGetMouseX(), ofGetMouseY(), ofGetWindowWidth() / 600.0);
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage& msg) {
	
	// make a copy of the latest message
	midiMessage = msg;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
	
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
	
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
	
}














//======================================================
//======================================================

//--------------------------------------------------------------
//void ofApp::drawFluidDensity(int _x, int _y, int _width, int _height) {
//	ofPushStyle();
//
//	fluidSimulation.draw(_x, _y, _width, _height);
//
//	ofPopStyle();
//}

//--------------------------------------------------------------
//void ofApp::drawFluidVelocity(int _x, int _y, int _width, int _height) {
//	ofPushStyle();
//	if (showScalar.get()) {
//		ofClear(0,0);
//		ofEnableBlendMode(OF_BLENDMODE_ALPHA);
//		//	ofEnableBlendMode(OF_BLENDMODE_DISABLED); // altenate mode
//		displayScalar.setSource(fluidSimulation.getVelocity());
//		displayScalar.draw(_x, _y, _width, _height);
//	}
//	if (showField.get()) {
//		ofEnableBlendMode(OF_BLENDMODE_ADD);
//		velocityField.setVelocity(fluidSimulation.getVelocity());
//		velocityField.draw(_x, _y, _width, _height);
//	}
//	ofPopStyle();
//}

//--------------------------------------------------------------
//void ofApp::drawFluidPressure(int _x, int _y, int _width, int _height) {
//	ofPushStyle();
//	ofClear(128);
//	if (showScalar.get()) {
//		ofEnableBlendMode(OF_BLENDMODE_DISABLED);
//		displayScalar.setSource(fluidSimulation.getPressure());
//		displayScalar.draw(_x, _y, _width, _height);
//	}
//	if (showField.get()) {
//		ofEnableBlendMode(OF_BLENDMODE_ALPHA);
//		pressureField.setPressure(fluidSimulation.getPressure());
//		pressureField.draw(_x, _y, _width, _height);
//	}
//	ofPopStyle();
//}

//--------------------------------------------------------------
//void ofApp::drawFluidTemperature(int _x, int _y, int _width, int _height) {
//	ofPushStyle();
//	if (showScalar.get()) {
//		ofEnableBlendMode(OF_BLENDMODE_DISABLED);
//		displayScalar.setSource(fluidSimulation.getTemperature());
//		displayScalar.draw(_x, _y, _width, _height);
//	}
//	if (showField.get()) {
//		ofEnableBlendMode(OF_BLENDMODE_ADD);
//		temperatureField.setTemperature(fluidSimulation.getTemperature());
//		temperatureField.draw(_x, _y, _width, _height);
//	}
//	ofPopStyle();
//}

//--------------------------------------------------------------
//void ofApp::drawFluidDivergence(int _x, int _y, int _width, int _height) {
//	ofPushStyle();
//	if (showScalar.get()) {
//		ofEnableBlendMode(OF_BLENDMODE_DISABLED);
//		displayScalar.setSource(fluidSimulation.getDivergence());
//		displayScalar.draw(_x, _y, _width, _height);
//	}
//	if (showField.get()) {
//		ofEnableBlendMode(OF_BLENDMODE_ADD);
//		temperatureField.setTemperature(fluidSimulation.getDivergence());
//		temperatureField.draw(_x, _y, _width, _height);
//	}
//	ofPopStyle();
//}

//--------------------------------------------------------------
//void ofApp::drawFluidVorticity(int _x, int _y, int _width, int _height) {
//	ofPushStyle();
//	if (showScalar.get()) {
//		ofEnableBlendMode(OF_BLENDMODE_DISABLED);
//		displayScalar.setSource(fluidSimulation.getConfinement());
//		displayScalar.draw(_x, _y, _width, _height);
//	}
//	if (showField.get()) {
//		ofEnableBlendMode(OF_BLENDMODE_ADD);
//		ofSetColor(255, 255, 255, 255);
//		velocityField.setVelocity(fluidSimulation.getConfinement());
//		velocityField.draw(_x, _y, _width, _height);
//	}
//	ofPopStyle();
//}

//--------------------------------------------------------------
//void ofApp::drawFluidBuoyance(int _x, int _y, int _width, int _height) {
//	ofPushStyle();
//	if (showScalar.get()) {
//		ofEnableBlendMode(OF_BLENDMODE_DISABLED);
//		displayScalar.setSource(fluidSimulation.getSmokeBuoyancy());
//		displayScalar.draw(_x, _y, _width, _height);
//	}
//	if (showField.get()) {
//		ofEnableBlendMode(OF_BLENDMODE_ADD);
//		velocityField.setVelocity(fluidSimulation.getSmokeBuoyancy());
//		velocityField.draw(_x, _y, _width, _height);
//	}
//	ofPopStyle();
//}

//--------------------------------------------------------------

//--------------------------------------------------------------
//void ofApp::drawMask(int _x, int _y, int _width, int _height) {
//	ofPushStyle();
//	ofEnableBlendMode(OF_BLENDMODE_DISABLED);
//	velocityMask.draw(_x, _y, _width, _height);
//	ofPopStyle();
//}

//--------------------------------------------------------------
//void ofApp::drawOpticalFlow(int _x, int _y, int _width, int _height) {
//	ofPushStyle();
//	if (showScalar.get()) {
//		ofEnableBlendMode(OF_BLENDMODE_ALPHA);
//		displayScalar.setSource(opticalFlow.getOpticalFlowDecay());
//		displayScalar.draw(0, 0, _width, _height);
//	}
//	if (showField.get()) {
//		ofEnableBlendMode(OF_BLENDMODE_ADD);
//		velocityField.setVelocity(opticalFlow.getOpticalFlowDecay());
//		velocityField.draw(0, 0, _width, _height);
//	}
//	ofPopStyle();
//}

//--------------------------------------------------------------
//void ofApp::drawMouseForces(int _x, int _y, int _width, int _height) {
//	ofPushStyle();
//	ofClear(0,0);
//
//	for(int i=0; i<mouseForces.getNumForces(); i++) {
//		ofEnableBlendMode(OF_BLENDMODE_ADD);
//		if (mouseForces.didChange(i)) {
//			ftDrawForceType dfType = mouseForces.getType(i);
//			if (dfType == FT_DENSITY)
//				mouseForces.getTextureReference(i).draw(_x, _y, _width, _height);
//		}
//	}
//
//	for(int i=0; i<mouseForces.getNumForces(); i++) {
//		ofEnableBlendMode(OF_BLENDMODE_ALPHA);
//		if (mouseForces.didChange(i)) {
//			switch (mouseForces.getType(i)) {
//				case FT_VELOCITY:
//					velocityField.setVelocity(mouseForces.getTextureReference(i));
//					velocityField.draw(_x, _y, _width, _height);
//					break;
//				case FT_TEMPERATURE:
//					temperatureField.setTemperature(mouseForces.getTextureReference(i));
//					temperatureField.draw(_x, _y, _width, _height);
//					break;
//				case FT_PRESSURE:
//					pressureField.setPressure(mouseForces.getTextureReference(i));
//					pressureField.draw(_x, _y, _width, _height);
//					break;
//				default:
//					break;
//			}
//		}
//	}
//
//	ofPopStyle();
//}

//--------------------------------------------------------------
//void ofApp::drawVelocityDots(int _x, int _y, int _width, int _height) {
//	ofPushStyle();
//	ofEnableBlendMode(OF_BLENDMODE_ADD);
//	velocityDots.setVelocity(fluidSimulation.getVelocity());
//	velocityDots.draw(_x, _y, _width, _height);
//	ofPopStyle();
//}
