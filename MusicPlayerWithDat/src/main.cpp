#include "ofMain.h"
#include "ofApp.h"

/*
	Modified from the ofGLSetup() method, but uses a ofGLFWWindowSettings
	to have access to a resizable field that can be set to false.
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

int main() {
	modifiedSetup(1200, 425, OF_WINDOW);
	ofRunApp(new ofApp());
}
