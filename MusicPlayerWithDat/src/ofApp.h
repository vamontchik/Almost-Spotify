#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "MusicPlayer.h"
#include "ModifiedWireframe.h"

class ofApp : public ofBaseApp {

private:
	ofxDatGuiScrollView* scroller_;
	ofxDatGuiTheme* theme_;
	MusicPlayer* player_;

private:
	~ofApp();
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	//void onButtonEvent(ofxDatGuiButtonEvent);
	void onScrollViewEvent(ofxDatGuiScrollViewEvent);
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
