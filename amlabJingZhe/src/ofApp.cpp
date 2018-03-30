#include "ofApp.h"



//--------------------------------------------------------------
void ofApp::setup(){
	
	ofSetVerticalSync(false);
	ofSetLogLevel(OF_LOG_NOTICE);
	ofBackground(255, 255, 255);
	//setup kinect 1
	// enable depth->video image calibration
//	kinect1.setRegistration(true);
	
//	kinect1.init();
	//kinect.init(true); // shows infrared instead of RGB video image
	//kinect.init(false, false); // disable video image (faster fps)
	
//	kinect1.open();		// opens first available kinect
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
	
	int ratio = 4;
	
	// CAMERA
	simpleCam.setup(640, 480, true);
	didCamUpdate = false;
	cameraFbo.allocate(640, 480);
	cameraFbo.black();
	
	cameraFrameImg.allocate(640, 480, OF_IMAGE_GRAYSCALE);
	
	// cameara fbo for fluid
//	camFboForFluid.allocate(fboForFluidW, fboForFluidH);
	
	// video fbo for fuild
	// fixed w h for test only ==============  TODO
	videoFboForFluid.allocate(fboForFluidW,fboForFluidH);
	fboForFluid.allocate(fboForFluidW, fboForFluidH,GL_RGB);
	// obstacle fbo
	fboForObstacle.allocate(drawWidth, drawHeight);
	
	lastTime = ofGetElapsedTimef();
	
	// syphon
	syphonFbo.allocate(drawWidth,drawHeight,GL_RGBA);

//	syphonPixels.allocate(drawWidth, drawHeight, OF_PIXELS_RGB);
//	syphonTex.allocate(syphonPixels);
//	mainOutputSyphonServer.setName("A.M Lab -- Jing Zhe");

	syphonClient.setup();
	syphonClient.setServerName("Video");


	animationSyphonClient.setup();
//	animationSyphonClient.setServerName("AnimateSyphonServer");
	animationSyphonClient.setServerName("animateoutputsyphon");

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
	
	
	// GUI
	setupGui();
	
	
	// int MyFLowTools
	myFlowTools1.setup(drawWidth, drawHeight, ratio,"id1");
//	myFlowTools2.setup(drawWidth, drawHeight, ratio,"id2");

}

//--------------------------------------------------------------
void ofApp::setupGui() {
	
	gui.setup("settings");
	gui.setDefaultBackgroundColor(ofColor(0, 0, 0, 127));
	gui.setDefaultFillColor(ofColor(160, 160, 160, 160));
	gui.add(guiFPS.set("average FPS", 0, 0, 60));
	gui.add(guiMinFPS.set("minimum FPS", 0, 0, 60));
//	gui.add(toggleGuiDraw.set("show gui (G)", false));
//	gui.add(doFlipCamera.set("flip camera", true));
//	gui.add(doDrawCamBackground.set("draw camera (C)", true));
	
	
	gui.add(minArea.set("Min area", 10, 1, 100));
	gui.add(maxArea.set("Max area", 200, 1, 500));
	gui.add(threshold.set("Threshold", 128, 0, 255));
	gui.add(holes.set("Holes", false));
	
	
	
	// seva setting with give name
	if (!ofFile("settings.xml"))
		gui.saveToFile("settings.xml");
	
	gui.loadFromFile("settings.xml");
	
	
	
	
}

//--------------------------------------------------------------
void ofApp::update(){
	
//	kinect1.update();
//
//	if(kinect1.isFrameNew()) {
//		grayKinect1Image.setFromPixels(kinect1.getDepthPixels());
//		// we do two thresholds - one for the far plane and one for the near plane
//		// we then do a cvAnd to get the pixels which are a union of the two thresholds
//		if(bThreshWithOpenCV) {
//			grayThreshNear = grayKinect1Image;
//			grayThreshFar = grayKinect1Image;
//			grayThreshNear.threshold(nearThreshold, true);
//			grayThreshFar.threshold(farThreshold);
//			cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayKinect1Image.getCvImage(), NULL);
//		} else {
//
//			// or we do it ourselves - show people how they can work with the pixels
//			ofPixels & pix = grayKinect1Image.getPixels();
//			int numPixels = pix.size();
//			for(int i = 0; i < numPixels; i++) {
//				if(pix[i] < nearThreshold && pix[i] > farThreshold) {
//					pix[i] = 255;
//				} else {
//					pix[i] = 0;
//				}
//			}
//		}
//
//		grayKinect1Image.flagImageChanged();
//
//
//
//	};
//
//	deltaTime = ofGetElapsedTimef() - lastTime;
//	lastTime = ofGetElapsedTimef();
	simpleCam.update();

	if (simpleCam.isFrameNew()) {
		contourFinder.setMinAreaRadius(minArea);
		contourFinder.setMaxAreaRadius(maxArea);
		contourFinder.setThreshold(threshold);
		contourFinder.findContours(simpleCam);
		contourFinder.setFindHoles(holes);
		
		ofPushStyle();
		ofEnableBlendMode(OF_BLENDMODE_DISABLED);

		
		cameraFbo.begin();

//		if (doFlipCamera)
//			simpleCam.draw(cameraFbo.getWidth(), 0, -cameraFbo.getWidth(), cameraFbo.getHeight());  // Flip Horizontal
//		else
//			simpleCam.draw(0, 0, cameraFbo.getWidth(), cameraFbo.getHeight());

		
		cameraFbo.end();
		ofPopStyle();

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
	}
////
//
//
//	// animation/fbo for making fluid/particles
////	fboForFluid.begin();
////	ofSetColor(0, 200, 0);
////	ofDrawRectangle(0, 0, 640, 20);
////	fboForFluid.end();
////
////	opticalFlow.setSource(fboForFluid.getTexture());
////	opticalFlow.update();
////	velocityMask.setDensity(fboForFluid.getTexture());
////	velocityMask.setVelocity(opticalFlow.getOpticalFlow());
////	velocityMask.update();
//
//
//	// set source from syphon client ===============================  TODO
	fboForFluid.begin();
	
	syphonClient.draw(0,0, fboForFluidW, fboForFluidH);

//	animationSyphonClient.draw(0, 0, fboForFluidW, fboForFluidH);

	fboForFluid.end();
//
//
//
	// obstacle fbo
	fboForObstacle.begin();
	ofClear(0,0);
	ofSetColor(255, 255, 255);
//	ofDrawRectangle(100, 100, 600, 200);
//	grayKinect1Image.draw(0, 0, drawWidth,drawHeight);
	
	for (int i = 0; i < contourFinder.getBoundingRects().size(); i++) {
		int x = contourFinder.getBoundingRects().at(i).x;
		int y = contourFinder.getBoundingRects().at(i).y;
		int w = contourFinder.getBoundingRects().at(i).width;
		int h = contourFinder.getBoundingRects().at(i).height;
		int scale = drawWidth / fboForFluidW;
		ofDrawRectangle(x * scale, y * scale, w * scale, h * scale);
	}
	fboForObstacle.end();
//
//
//	// MyFlowTools update
	myFlowTools1.update(&fboForFluid, &fboForObstacle);
//	myFlowTools2.update(&fboForFluid, &fboForObstacle);

	
	
	
	
//	cout << animationSyphonClient.getTexture().getHeight() << endl;
//	fboForFluid.begin();
//	ofClear(0,0);
//	animationSyphonClient.draw(0,0);
//	fboForFluid.end();
//	myFlowTools1.update(&fboForFluid, &fboForObstacle);

	
	
}
//--------------------------------------------------------------

void ofApp::exit() {
	
	// clean up
//	midiIn.closePort();
//	midiIn.removeListener(this);
//
//	kinect1.setCameraTiltAngle(0); // zero the tilt on exit
//	kinect1.close();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
		case 'S':
		case 's':
			useAnimateClient = !useAnimateClient;
			break;
		case 'G':
		case 'g': toggleGuiDraw = !toggleGuiDraw; break;
		case 'f':
		case 'F': doFullScreen.set(!doFullScreen.get()); break;
		case 'c':
		case 'C': doDrawCamBackground.set(!doDrawCamBackground.get()); break;
			
		case 'r':
		case 'R':
//			fluidSimulation.reset();
//			fluidSimulation.addObstacle(flowToolsLogoImage.getTexture());
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
void ofApp::draw(){
//	syphonFbo.begin();

	ofClear(0,0);
	
//	simpleCam.draw(0, 0);
//	contourFinder.draw();
	
	
	myFlowTools1.draw();
//	myFlowTools2.draw();

//	syphonFbo.end();
//	mainOutputSyphonServer.publishTexture(&syphonFbo.getTexture());

	drawGui();
	
	
	// test obstacle fbo
//	obstacleFbo.draw(0, 0);
	
//	 test syphon client
//	syphonClient.draw(0,0);
	
	// test animate syphon client
//	animationSyphonClient.draw(0, 0);
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
	
	gui.draw();
	
	myFlowTools1.drawGui();
//	myFlowTools2.drawGui();
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

