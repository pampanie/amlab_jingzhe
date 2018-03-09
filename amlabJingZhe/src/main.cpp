#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){
	int mainW = 800;
	int mainH = 600;
	int guiW = 800;
	int guiH = 600;

	//	main window ===============
	ofGLFWWindowSettings settings;
	//	main window size
	settings.width = mainW;
	settings.height = mainH;
	//	main window position
	settings.setPosition(ofVec2f(guiW,0));
	//	settings.resizable = true;
	shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
	
	//	gui window size
	settings.width = guiW;
	settings.height = guiH;
	//	gui window position
	settings.setPosition(ofVec2f(0,0));
	settings.resizable = false;
	// uncomment next line to share main's OpenGL resources with gui
	//settings.shareContextWith = mainWindow;
	shared_ptr<ofAppBaseWindow> guiWindow = ofCreateWindow(settings);
	guiWindow->setVerticalSync(false);
	
	shared_ptr<ofApp> mainApp(new ofApp);
	//	self custome function setupGui
	mainApp->setupGui();
	//	self make function drawGui
	ofAddListener(guiWindow->events().draw,mainApp.get(),&ofApp::drawGui);
	
	ofRunApp(mainWindow, mainApp);
	ofRunMainLoop();
	
}
