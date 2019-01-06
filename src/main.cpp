
// Nicola Pisanti, GPLv3 License, 2016

#include "ofMain.h"
#include "ofApp.h"
#include "flags.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){

#ifdef __ARM_NEON__

	ofSetupOpenGL(WIDTH, HEIGHT, OF_FULLSCREEN);			// <-------- setup the GL context
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());
    
#else
	ofGLFWWindowSettings settings;
    settings.setSize( WIDTH + PROJECTOR_WIDTH, HEIGHT );
	settings.setPosition(ofVec2f(0,0));
	settings.resizable = false;
    settings.decorated = false;
    settings.multiMonitorFullScreen = true;
	shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);    
    

	shared_ptr<ofApp> mainApp(new ofApp);

	ofRunApp(mainWindow, mainApp);
	ofRunMainLoop();
#endif

}
