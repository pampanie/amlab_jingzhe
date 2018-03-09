#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(false);
	ofSetLogLevel(OF_LOG_NOTICE);
	
	drawWidth = 1024;
	drawHeight = 768;
	
	// process all but the density on 16th resolution
	flowWidth = drawWidth / 4;
	flowHeight = drawHeight / 4;
	
	// FLOW & MASK
	opticalFlow.setup(flowWidth, flowHeight);
	velocityMask.setup(drawWidth, drawHeight);
	
	// FLUID & PARTICLES
	fluidSimulation.setup(flowWidth, flowHeight, drawWidth, drawHeight);
	particleFlow.setup(flowWidth, flowHeight, drawWidth, drawHeight);
	
	//	flowToolsLogoImage.load("flowtools.png");
	//	flowToolsLogoImage.load("ershou_logo_bw2.png");
	//	flowToolsLogoImage.load("tupian1.png");
	
	//	fluidSimulation.addObstacle(flowToolsLogoImage.getTexture());
	
	
	// CAMERA
	simpleCam.setup(640, 480, true);
	didCamUpdate = false;
	cameraFbo.allocate(640, 480);
	cameraFbo.black();
	showCameraSource = true;
	
	lastTime = ofGetElapsedTimef();
	
	// particles
	showParticles = true;
}
//--------------------------------------------------------------
void ofApp::setupGui(){
	parameters.setName("parameters");
	//	parameters.add(radius.set("radius",50,1,100));
	//	parameters.add(color.set("color",100,ofColor(0,0),255));
	gui.setup(parameters);
	ofSetBackgroundColor(0);
}

//--------------------------------------------------------------
void ofApp::update(){
	deltaTime = ofGetElapsedTimef() - lastTime;
	lastTime = ofGetElapsedTimef();
	
	simpleCam.update();
	
	if (simpleCam.isFrameNew()) {
		
		ofPushStyle();
		ofEnableBlendMode(OF_BLENDMODE_DISABLED);
		cameraFbo.begin();
		simpleCam.draw(0, 0, cameraFbo.getWidth(), cameraFbo.getHeight());
		cameraFbo.end();
		ofPopStyle();
		
		opticalFlow.setSource(cameraFbo.getTexture());
		
		// opticalFlow.update(deltaTime);
		// use internal deltatime instead
		opticalFlow.update();
		
	}
	
	
	fluidSimulation.addVelocity(opticalFlow.getOpticalFlowDecay());
	fluidSimulation.addDensity(velocityMask.getColorMask());
	fluidSimulation.addTemperature(velocityMask.getLuminanceMask());
	
	fluidSimulation.update();
	
	if (particleFlow.isActive()) {
		particleFlow.setSpeed(fluidSimulation.getSpeed());
		particleFlow.setCellSize(fluidSimulation.getCellSize());
		particleFlow.addFlowVelocity(opticalFlow.getOpticalFlow());
		particleFlow.addFluidVelocity(fluidSimulation.getVelocity());
		//		particleFlow.addDensity(fluidSimulation.getDensity());
		particleFlow.setObstacle(fluidSimulation.getObstacle());
		// particle add obstacle self
		// particleFlow.addObstacle(flowToolsLogoImage.getTexture());
		
	}
	particleFlow.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofClear(0,0);
	drawCameraSource();
	drawParticles();
}

//--------------------------------------------------------------
void ofApp::exit(){
	
}
//--------------------------------------------------------------
void ofApp::drawCameraSource(){
	if (showCameraSource){
		ofPushStyle();
		ofEnableBlendMode(OF_BLENDMODE_DISABLED);
		cameraFbo.draw(0,0, drawWidth, drawHeight);
		ofPopStyle();
	}
}
//--------------------------------------------------------------
void ofApp::drawParticles(){
	if(particleFlow.isActive() & showParticles){
		ofPushStyle();
		ofEnableBlendMode(OF_BLENDMODE_ALPHA);
		particleFlow.draw(0,0, drawWidth, drawHeight);
		ofPopStyle();
	}
}
//--------------------------------------------------------------

void ofApp::drawGui(ofEventArgs & args){
	gui.draw();
}
//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage& msg) {
	
	// make a copy of the latest message
	//	midiMessage = msg;
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
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
