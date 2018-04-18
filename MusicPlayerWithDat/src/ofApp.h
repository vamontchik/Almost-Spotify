#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "MusicPlayer.h"

class ofApp : public ofBaseApp {

private:
	ofxDatGui* panel_;
	ofxDatGuiTheme* theme_;
	MusicPlayer* player_;
	bool changingGui_ = false;

private:
	~ofApp();
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void mousePressed(int x, int y, int button);
	void onButtonEvent(ofxDatGuiButtonEvent);
	void setupGUI();

	/*
	Default event listeners from base gui. Not used.

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
	*/	
};
