#include "ofMain.h"
#include "ofApp.h"

//========================================================================

/* 
	Modified from the setup method, but uses a ofGLFWWindowSettings,
	instead of the other one because it has a resizeable field...
*/
void modifiedSetup(int width, int height, ofWindowMode winMode) {
	ofGLFWWindowSettings settings;
	settings.glVersionMajor = 2;
	settings.glVersionMinor = 1;
	settings.resizable = false;
	settings.width = width;
	settings.height = height;
	settings.windowMode = winMode;
	ofCreateWindow(settings);
}

int main(){
	modifiedSetup(1000, 600, OF_WINDOW);
	ofRunApp(new ofApp());
}
